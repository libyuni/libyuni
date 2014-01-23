#ifndef __YUNI_UI_CONTROL_BUTTON_H__
# define __YUNI_UI_CONTROL_BUTTON_H__

# include "../../yuni.h"
# include "control.h"
# include "../font.h"
# include "../displaymode.h"

namespace Yuni
{
namespace UI
{
namespace Control
{


	//! A button is a clickable control that triggers an event
	class Button: public IControl
	{
	public:
		//! Type of bind for the button callback
		typedef Yuni::Bind<EventPropagation (IControl*)>  Callback;

	public:
		//! Callback for button click
		Callback onClick;

	public:
		Button()
		{}

		Button(int x, int y, uint width, uint height):
			IControl(x, y, width, height),
			pOffsetX(0),
			pOffsetY(0),
			pFillColor(0.0f, 0.0f, 0.0f, 0.0f),
			pDisplay(dmNone)
		{}

		Button(const Point2D<int>& position, const Point2D<uint>& size):
			IControl(position, size),
			pOffsetX(0),
			pOffsetY(0),
			pFillColor(0.0f, 0.0f, 0.0f, 0.0f),
			pDisplay(dmNone)
		{}

		//! Virtual destructor
		virtual ~Button() {}

		const String& text() const { return pText; }
		void text(const AnyString& text) { pText = text; invalidate(); }

		//! Launch a click event
		void click() { onClick(this); }

		//! Draw the button on the surface
		virtual void draw(DrawingSurface::Ptr& surface, bool root);

		//! Image shown on the button
		void image(const Gfx3D::Texture::Ptr& image) { pImage = image; }

		//! Image displayed while clicking (same as standard image if `null`)
		void imageClicking(const Gfx3D::Texture::Ptr& image) { pImageClicking = image; }

		//! Image displayed while hovering (same as standard image if `null`)
		void imageHovering(const Gfx3D::Texture::Ptr& image) { pImageHovering = image; }

		//! On mouse button down
		virtual void mouseDown(Input::IMouse::Button btn, int, int) override
		{
			if (btn != Input::IMouse::ButtonLeft)
				return;
			pBeingClicked = true;
			if (!(!pImageClicking))
				pModified = true;
		}

		//! On mouse button up
		virtual void mouseUp(Input::IMouse::Button btn, int, int) override
		{
			if (btn != Input::IMouse::ButtonLeft)
				return;
			if (pBeingClicked)
				onClick(this);
			pBeingClicked = false;
			pModified = true;
		}

		//! Get the fill color
		const Color::RGBA<float>& fillColor() const { return pFillColor; }
		//! Set the fill color
		void fillColor(const Color::RGBA<float>& color) { pFillColor = color; }

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
		void offset(int x, int y) { pDisplay = dmOffset; pOffsetX = x; pOffsetY = y; invalidate(); }
		//! Get current offset (might not be used if display mode is not podOffset)
		Point2D<int> offset() const { return Point2D<int>(pOffsetX, pOffsetY); }

	private:
		String pText;

		String pHoverText;

		Gfx3D::Texture::Ptr pImage;

		Gfx3D::Texture::Ptr pImageClicking;

		Gfx3D::Texture::Ptr pImageHovering;

		bool pBeingClicked;

		//! Offset of the rectangle over the image in X (only used in Offset display mode)
		int pOffsetX;

		//! Offset of the rectangle over the image in Y (only used in Offset display mode)
		int pOffsetY;

		//! Fill color for when a part of the overlay is empty
		Color::RGBA<float> pFillColor;

		//! Type of display of the image inside the rectangle
		DisplayMode pDisplay;

	}; // class Button



} // namespace Control
} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_CONTROL_BUTTON_H__
