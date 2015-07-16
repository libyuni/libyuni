/*
** YUNI's default license is the GNU Lesser Public License (LGPL), with some
** exclusions (see below). This basically means that you can get the full source
** code for nothing, so long as you adhere to a few rules.
**
** Under the LGPL you may use YUNI for any purpose you wish, and modify it if you
** require, as long as you:
**
** Pass on the (modified) YUNI source code with your software, with original
** copyrights intact :
**  * If you distribute electronically, the source can be a separate download
**    (either from your own site if you modified YUNI, or to the official YUNI
**    website if you used an unmodified version) – just include a link in your
**    documentation
**  * If you distribute physical media, the YUNI source that you used to build
**    your application should be included on that media
** Make it clear where you have customised it.
**
** In addition to the LGPL license text, the following exceptions / clarifications
** to the LGPL conditions apply to YUNI:
**
**  * Making modifications to YUNI configuration files, build scripts and
**    configuration headers such as yuni/platform.h in order to create a
**    customised build setup of YUNI with the otherwise unmodified source code,
**    does not constitute a derived work
**  * Building against YUNI headers which have inlined code does not constitute a
**    derived work
**  * Code which subclasses YUNI classes outside of the YUNI libraries does not
**    form a derived work
**  * Statically linking the YUNI libraries into a user application does not make
**    the user application a derived work.
**  * Using source code obsfucation on the YUNI source code when distributing it
**    is not permitted.
** As per the terms of the LGPL, a "derived work" is one for which you have to
** distribute source code for, so when the clauses above define something as not
** a derived work, it means you don't have to distribute source code for it.
** However, the original YUNI source code with all modifications must always be
** made available.
*/
#ifndef __YUNI_UI_VIEW_H__
# define __YUNI_UI_VIEW_H__

# include "../../yuni.h"
# include "../../core/dictionary.h"
# include "../../core/math.h"
# include "../../core/smartptr.h"
# include "../../uuid/uuid.h"
# include "../control/control.h"
# include "../eventpropagation.h"
# include "drawingsurface.h"
# include "shaderprogram.h"
# include "../textoverlay.h"
# include "../pictureoverlay.h"
# include <list>


namespace Yuni
{
namespace UI
{


	//! Forward declarations (for friend)
	class RenderWindow;
	class GLWindow;


	/*!
	** \brief A view is a rectangle where 3D data is rendered inside a window
	**
	** View position and size is given in pixels
	** There can be several views in a window, overlapping or not.
	** The main usage is split-screen games.
	**
	** For overlapping, views have a Z-order.
	** Higher Z-order means closer to the viewer, 0 is furthest, 255 is closest.
	** Default Z-order is 127 (at mid-way).
	**
	** \warning Overlapping is not recommended for the moment, because the implementation is sub-efficient.
	*/
	class View
	{
	public:
		//! Smart pointer
		typedef SmartPtr<View>  Ptr;
		//! Linked list
		typedef std::list<Ptr>  List;

	public:
		//! Constructor
		View(float x, float y, float w, float h, uint8 z = 127, bool visible = true);

		//! Destructor
		virtual ~View();

		//! Identifier of the view
		const UUID& id() const { return pID; }

		//! X position
		float x() const { return pX; }

		//! Y position
		float y() const { return pY; }

		//! Z order position, 0 is bottom / back, 255 is top / front
		uint8 z() const { return pZ; }
		void z(uint8 z) { pZ = z; }

		//! Width of the view
		float width() const { return pWidth; }

		//! Height of the view
		float height() const { return pHeight; }

		//! Move the view to an absolute position
		void moveTo(float x, float y) { pX = x; pY = y; }
		//! Move the view by a relative amount
		void moveBy(float x, float y) { pX += x; pY += y; }

		//! Resize the view
		void resize(float width, float height)
		{
			assert(height > 0 && "Resizing view to a null or negative height !");
			pWidth = width;
			pHeight = Math::Max(height, 1.0f);
		}

		//! Get the visibility of the view
		bool visible() const { return pVisible; }
		//! Set the visibility of the view
		void show() { pVisible = true; }
		void hide() { pVisible = false; }
		void show(bool isVisible) { pVisible = isVisible; }

		//! Overlay management
		void addOverlay(const TextOverlay::Ptr& text) { pTexts.push_back(text); }
		void addOverlay(const PictureOverlay::Ptr& picture) { pPictures.push_back(picture); }
		void clearOverlays() { pTexts.clear(); pPictures.clear(); }

		//! Add a UI control to the view
		void rootControl(const IControl::Ptr& control)
		{
			if (pControl != control)
			{
				pEnteredControls.clear();
				pControl = control;
			}
		}
		//! Get the UI controls
		const IControl::Ptr& rootControl() const { return pControl; }
		IControl::Ptr& rootControl() { return pControl; }

		//! Get the top-most control at given coordinates. (Mainly useful for clicking)
		IControl* getControlAt(float x, float y);

		/*!
		** \brief Draw the view
		**
		** \note If you see nothing in this view, make sure the camera is set and the view is visible
		*/
		virtual void draw(uint msMultiplier = 1) const;

	protected:
		EventPropagation doMouseMove(int x, int y);
		virtual EventPropagation doMouseDown(Input::IMouse::Button btn, int x, int y);
		EventPropagation doMouseUp(Input::IMouse::Button btn, int x, int y);
		EventPropagation doMouseDblClick(Input::IMouse::Button btn, int x, int y);
		EventPropagation doMouseScroll(float delta, int x, int y);
		EventPropagation doMouseHover(int x, int y);
		EventPropagation doMouseLeave();

		EventPropagation doKeyDown(Input::Key key, int x, int y);
		EventPropagation doKeyUp(Input::Key key, int x, int y);
		EventPropagation doCharInput(const AnyString& str, int x, int y);

		//! Draw a text overlay
		void drawOverlay(TextOverlay& text) const;

		//! Draw a picture overlay
		void drawOverlay(const PictureOverlay& text) const;

		//! Draw a texture at the given coordinates (in pixels)
		void drawPicture(const Gfx3D::Texture::Ptr& texture, float x, float y, float width,
			float height, bool flip = false, bool invert = false) const;

		//! Draw all 2D elements : overlays and UI
		void draw2D() const;

		//! Necessary shaders for overlay
		bool initShaders();

	protected:
		//! ID of the view
		UUID pID;

		//! X position in pixels (relative to the window)
		float pX;
		//! Y position in pixels (relative to the window)
		float pY;

		//! Width of the view in pixels
		float pWidth;

		//! Height of the view in pixels
		float pHeight;

		//! Z-order, the higher the closer to the viewer
		uint8 pZ;

		/*!
		** \brief Is the view visible or hidden ?
		**
		** \note This is a hiding feature and has nothing to do with whether the view is hidden by other views on top of it
		*/
		bool pVisible;

		//! UI Text overlays
		TextOverlay::Vector pTexts;

		//! UI Picture overlays
		PictureOverlay::Vector pPictures;

		//! UI Control root for this view
		IControl::Ptr pControl;

		//! UI Controls that currently under the mouse cursor
		IControl::Set pEnteredControls;

		//! Shaders for text rendering
		mutable Gfx3D::ShaderProgram::Ptr pTextShaders;

		//! Shaders for picture rendering
		mutable Gfx3D::ShaderProgram::Ptr pPictureShaders;

		//! Drawing surface for UI controls
		mutable DrawingSurface::Ptr pUISurface;

		//! Friend declarations
		friend class RenderWindow;
		friend class GLWindow;

	}; // class View



} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_VIEW_H__
