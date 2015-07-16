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
#ifndef __YUNI_UI_CONTROL_PICTURE_H__
# define __YUNI_UI_CONTROL_PICTURE_H__

# include "../../yuni.h"
# include "control.h"


namespace Yuni
{
namespace UI
{
namespace Control
{


	//! A picture is a control that displays an image in a quad
	class Picture: public IControl
	{
	public:
		//! Smart pointer
		typedef Ancestor::SmartPtrType<Picture>::Ptr  Ptr;

	public:
		Picture():
			pOffsetX(0.0f),
			pOffsetY(0.0f),
			pFillColor(0.0f, 0.0f, 0.0f, 0.0f),
			pDisplay(dmStretch)
		{}

		Picture(float x, float y, float width, float height):
			IControl(x, y, width, height),
			pOffsetX(0.0f),
			pOffsetY(0.0f),
			pFillColor(0.0f, 0.0f, 0.0f, 0.0f),
			pDisplay(dmStretch)
		{}

		Picture(const Point2D<float>& position, const Point2D<float>& size):
			IControl(position, size),
			pOffsetX(0.0f),
			pOffsetY(0.0f),
			pFillColor(0.0f, 0.0f, 0.0f, 0.0f),
			pDisplay(dmStretch)
		{}

		//! Virtual destructor
		virtual ~Picture() {}

		//! Draw the picture
		virtual void draw(DrawingSurface::Ptr& surface, float xOffset, float yOffset) const override;

		//! Image shown on the button
		void image(const Gfx3D::Texture::Ptr& image) { pImage = image; invalidate(); }

		//! Get the fill color
		const Color::RGBA<float>& fillColor() const { return pFillColor; }
		//! Set the fill color
		void fillColor(const Color::RGBA<float>& color) { pFillColor = color; invalidate(); }

		//! Current display mode
		DisplayMode display() const { return pDisplay; }
		//! Set display mode to Fit
		void fit() { pDisplay = dmFit; invalidate(); }
		//! Set display mode to Fill
		void fill() { pDisplay = dmFill; invalidate(); }
		//! Set display mode to Center
		void center() { pDisplay = dmCenter; invalidate(); }
		//! Set display mode to Stretch
		void stretch() { pDisplay = dmStretch; invalidate(); }
		//! Set display mode to Offset and set offset values
		void offset(float x, float y)
		{
			pDisplay = dmOffset;
			pOffsetX = x;
			pOffsetY = y;
			invalidate();
		}
		//! Get current offset (might not be used if display mode is not podOffset)
		Point2D<float> offset() const { return Point2D<float>(pOffsetX, pOffsetY); }

	private:
		//! Texture containing the image
		Gfx3D::Texture::Ptr pImage;

		//! Offset of the overlay over the image in X (only used in Offset display)
		float pOffsetX;

		//! Offset of the overlay over the image in Y (only used in Offset display)
		float pOffsetY;

		//! Fill color for when a part of the overlay is empty
		Color::RGBA<float> pFillColor;

		//! Type of display of the image inside the overlay
		DisplayMode pDisplay;

	}; // class Picture



} // namespace Control
} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_CONTROL_PICTURE_H__
