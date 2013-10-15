#ifndef __YUNI_UI_VIEW_H__
# define __YUNI_UI_VIEW_H__

# include <yuni/yuni.h>
# include <yuni/core/smartptr.h>
# include <yuni/uuid/uuid.h>
# include <list>
# include "../scene/camera.h"
# include "../control/control.h"
# include "../eventpropagation.h"
//# include "../scene/scene.h"
# include "shaderprogram.h"
# include "../textoverlay.h"
# include "../pictureoverlay.h"


namespace Yuni
{
namespace UI
{


	/*!
	** \brief A view is a rectangle where 3D data is rendered inside a window
	**
	** View position and size is given in pixels
	** There can be several views in a window, overlapping or not.
	** The main usage is split-screen games.
	**
	** For overlapping, views have a Z-order.
	** Higher Z-order means closer to the viewer, 0 is furthest.
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
		View(uint x, uint y, uint w, uint h, uint8 z = 127, bool visible = true);

		//! Destructor
		virtual ~View();

		//! Necessary shaders for overlay
		bool initShaders();

		//! Identifier of the view
		const UUID& id() const { return pID; }

		//! X position
		int x() const { return pX; }

		//! Y position
		int y() const { return pY; }

		//! Z order position, 0 is bottom / back, 255 is top / front
		uint8 z() const { return pZ; }

		//! Width of the view
		uint width() const { return pWidth; }

		//! Height of the view
		uint height() const { return pHeight; }

		//! Move the view to an absolute position
		void moveTo(uint x, uint y) { pX = x; pY = y; }
		//! Move the view by a relative amount
		void moveBy(int x, int y) { pX += x; pY += y; }

		//! Resize the view
		void resize(uint width, uint height) { pWidth = width; pHeight = height > 0 ? height : 1; }

		//! Get the visibility of the view : the view must have a camera to be visible !
		bool visible() const { return pVisible && !(!pCamera); }
		//! Set the visibility of the view
		void show() { pVisible = true; }
		void hide() { pVisible = false; }
		void show(bool isVisible) { pVisible = isVisible; }

		//! Get the current camera
		const Gfx3D::Camera::Ptr& camera() const { return pCamera; }
		Gfx3D::Camera::Ptr& camera() { return pCamera; }
		//! Set the current camera
		void camera(const Gfx3D::Camera::Ptr& newCamera) { pCamera = newCamera; }

		//! Get the current scene
		// const Gfx3D::Scene::Ptr& scene() const { return pScene; }
		//! Set the current scene
		// void scene(const Gfx3D::Scene::Ptr& scene) { pScene = scene; }

		//! Overlay management
		void addOverlay(const TextOverlay::Ptr& text) { pTexts.push_back(text); }
		void addOverlay(const PictureOverlay::Ptr& picture) { pPictures.push_back(picture); }
		void clearOverlays() { pTexts.clear(); pPictures.clear(); }

		//! Set the root UI control
		void rootControl(const IControl::Ptr& control) { pControl = control; }
		//! Get the root UI control
		const IControl::Ptr& rootControl() const { return pControl; }
		IControl::Ptr rootControl() { return pControl; }

		EventPropagation doMouseMove(int x, int y);
		virtual EventPropagation doMouseDown(Input::IMouse::Button btn, int x, int y);
		EventPropagation doMouseUp(Input::IMouse::Button btn, int x, int y);
		EventPropagation doMouseDblClick(Input::IMouse::Button btn, int x, int y);
		EventPropagation doMouseScroll(float delta, int x, int y);
		EventPropagation doMouseHover(int x, int y);

		EventPropagation doKeyDown(Input::Key key);
		EventPropagation doKeyUp(Input::Key key);

		//! Get the top-most control at given coordinates. (Mainly useful for clicking)
		IControl* getControlAt(int x, int y);

		/*!
		** \brief Draw the view
		**
		** \note If you see nothing in this view, make sure the camera is set and the view is visible
		*/
		virtual void draw() const;

	protected:
		//! Draw a text overlay
		void drawOverlay(TextOverlay& text) const;

		//! Draw a picture overlay
		void drawOverlay(const PictureOverlay& text) const;

		//! Draw a texture at the given coordinates (in pixels)
		void drawPicture(const Gfx3D::Texture::Ptr& texture, int x, int y, unsigned int width, unsigned int height) const;

		//! Draw all 2D elements : overlays and UI
		void draw2D() const;

	protected:
		//! ID of the view
		UUID pID;

		//! X position in pixels (relative to the window)
		int pX;
		//! Y position in pixels (relative to the window)
		int pY;

		//! Width of the view in pixels
		uint pWidth;

		//! Height of the view in pixels
		uint pHeight;

		//! Z-order, the higher the closer to the viewer
		uint8 pZ;

		/*!
		** \brief Is the view visible or hidden ?
		**
		** \note This is a hiding feature and has nothing to do with whether the view is hidden by other views on top of it
		*/
		bool pVisible;

		//! Camera for this view, if null, the view is forcefully hidden
		Gfx3D::Camera::Ptr pCamera;

		//! 3D Scene
		// Gfx3D::Scene::Ptr pScene;

		//! UI Text overlays
		TextOverlay::Vector pTexts;

		//! UI Picture overlays
		PictureOverlay::Vector pPictures;

		//! UI Control root for this view
		IControl::Ptr pControl;

		//! Shaders for text rendering
		mutable Gfx3D::ShaderProgram::Ptr pTextShaders;

		//! Shaders for picture rendering
		mutable Gfx3D::ShaderProgram::Ptr pPictureShaders;

		//! Drawing surface for UI controls
		// mutable DrawingSurface::Ptr pUISurface;

	}; // class View



} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_VIEW_H__
