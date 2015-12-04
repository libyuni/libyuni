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
#ifndef __YUNI_UI_PICTUREOVERLAY_H__
# define __YUNI_UI_PICTUREOVERLAY_H__

# include "../yuni.h"
# include "../core/smartptr.h"
# include <vector>
# include "gl/shaderprogram.h"
# include "gl/texture.h"
# include "displaymode.h"

namespace Yuni
{
namespace UI
{

	/*!
	** \brief A picture overlay is a 2D picture meant to be displayed on screen over a 3D view
	*/
	class PictureOverlay
	{
	public:
		//! Smart pointer
		typedef SmartPtr<PictureOverlay>  Ptr;
		//! Vector
		typedef std::vector<Ptr>  Vector;

	public:
		//! Empty constructor
		PictureOverlay();

		//! Constructor. Passing 0 as a dimension indicates to use the value from the texture.
		PictureOverlay(const Gfx3D::Texture::Ptr& texture, int x, int y, uint width = 0, uint height = 0);

		//! Destructor
		~PictureOverlay();

		//! Draw the picture
		void draw(const Gfx3D::ShaderProgram::Ptr& shader) const;

		//! Modify the picture data
		void texture(const Gfx3D::Texture::Ptr& texture);
		//! Get the picture data
		const Gfx3D::Texture::Ptr& texture() const;

		//! Move the picture to another position
		void move(int x, int y);

		//! Get the X coordinate
		int x() const;
		//! Get the Y coordinate
		int y() const;

		//! Set the size of the picture
		void resize(uint width, uint height);

		//! Scale both dimensions by a factor
		void scale(float factor);

		//! Width of the picture (in pixels)
		uint width() const;
		//! Height of the picture (in pixels)
		uint height() const;

		//! Enable visibility
		void show() { pVisible = true; }
		//! Disable visibility
		void hide() { pVisible = false; }
		//! Set visibility
		void show(bool visible) { pVisible = visible; }
		//! Get visibility
		bool visible() const { return pVisible; }

		//! Get the fill color
		const Color::RGBA<float>& fillColor() const { return pFillColor; }
		//! Set the fill color
		void fillColor(const Color::RGBA<float>& color) { pFillColor = color; }

		//! Current display mode
		DisplayMode display() const { return pDisplay; }
		//! Set display mode to Fit
		void fit() { pDisplay = dmFit; }
		//! Set display mode to Fill
		void fill() { pDisplay = dmFill; }
		//! Set display mode to Center
		void center() { pDisplay = dmCenter; }
		//! Set display mode to Stretch
		void stretch() { pDisplay = dmStretch; }
		//! Set display mode to Offset and set offset values
		void offset(int x, int y) { pDisplay = dmOffset; pOffsetX = x; pOffsetY = y; }
		//! Get current offset (might not be used if display mode is not podOffset)
		Point2D<int> offset() const { return Point2D<int>(pOffsetX, pOffsetY); }

	private:
		//! Picture to display
		Gfx3D::Texture::Ptr pTexture;

		/*!
		** \brief Is it hidden or visible ?
		**
		** \note This has nothing to do with being occluded, this is a mechanism to hide the overlay
		*/
		bool pVisible;

		//! X coordinate for the left of the picture (in pixels, left = 0, right = ScreenWidth)
		int pX;

		//! Y coordinate for the bottom of the picture (in pixels, top = 0, bottom = ScreenHeight)
		int pY;

		//! Width of the picture (in pixels)
		uint pWidth;

		//! Height of the picture (in pixels)
		uint pHeight;

		//! Offset of the overlay over the image in X (only used in Offset display)
		int pOffsetX;

		//! Offset of the overlay over the image in Y (only used in Offset display)
		int pOffsetY;

		//! Fill color for when a part of the overlay is empty
		Color::RGBA<float> pFillColor;

		//! Type of display of the image inside the overlay
		DisplayMode pDisplay;
	};


} // namespace UI
} // namespace Yuni

# include "pictureoverlay.hxx"

#endif // __YUNI_UI_PICTUREOVERLAY_H__
