#ifndef __YUNI_UI_CONTROL_TEXTEDITOR_H__
# define __YUNI_UI_CONTROL_TEXTEDITOR_H__

# include "../../yuni.h"
# include "../../core/color/rgb.h"
# include "../../core/color/rgba.h"
# include "control.h"
# include "../displaymode.h"
# include "../dimension.h"
# include "../font.h"
# include "../theme.h"

namespace Yuni
{
namespace UI
{
namespace Control
{


	//! A text is an multi-line area for text edition
	class TextEditor: public IControl
	{
	public:
		TextEditor(float x, float y, float maxWidth, float maxHeight):
			IControl(x, y, maxWidth, maxHeight),
			pText(),
			pFont(Theme::Current()->monoFont),
			// White by default
			pColor(Theme::Current()->textColor),
			pBackColor(Theme::Current()->windowColor),
			pAntiAliased(true),
			pCursorPos(0u, 0u),
			pTopLineNb(0u),
			pLineHeight(161.8_pcp),
			pHorizMargin(15),
			pVertMargin(10)
		{
			// We use percent parent on the font size (height) for line height calculation
			pConversion.unitPerPercentParent = pFont->size() * 96.0f / 72.0f / 100.0f;
		}

		TextEditor(const Point2D<float>& position, const Point2D<float>& maxSize):
			IControl(position, maxSize),
			pText(),
			pFont(Theme::Current()->monoFont),
			pColor(Theme::Current()->textColor),
			pBackColor(Theme::Current()->windowColor),
			pAntiAliased(true),
			pCursorPos(0u, 0u),
			pTopLineNb(0u),
			pLineHeight(161.8_pcp),
			pHorizMargin(20),
			pVertMargin(10)
		{
			// We use percent parent on the font size (height) for line height calculation
			pConversion.unitPerPercentParent = pFont->size() * 96.0f / 72.0f / 100.0f;
		}

		//! Virtual destructor
		virtual ~TextEditor() {}

		//! Draw the panel
		virtual void draw(DrawingSurface::Ptr& surface, bool root) const override;

		//! Clear the text
		String& clear() { invalidate(); pTopLineNb = 0u; return pText.clear(); }

		//! Get the text
		String& text() { invalidate(); pTopLineNb = 0u; return pText; }
		const String& text() const { return pText; }

		//! Modify the font used
		void font(const UI::FTFont::Ptr& font)
		{
			if (pFont != font)
			{
				pFont = font;
				pConversion.unitPerPercentParent = pFont->size() / 100.0f;
				invalidate();
			}
		}
		//! Get the font
		const UI::FTFont::Ptr& font() const { return pFont; }

		//! Modify the color used for text
		void color(float r, float g, float b) { pColor.assign(r, g, b); invalidate(); }
		void color(float r, float g, float b, float a) { pColor.assign(r, g, b, a); invalidate(); }
		void color(const Color::RGB<float>& color) { pColor = color; invalidate(); }
		void color(const Color::RGBA<float>& color) { pColor = color; invalidate(); }
		//! Modify the color used for background
		void backColor(float r, float g, float b) { pBackColor.assign(r, g, b); invalidate(); }
		void backColor(float r, float g, float b, float a) { pBackColor.assign(r, g, b, a); invalidate(); }
		void backColor(const Color::RGB<float>& color) { pBackColor = color; invalidate(); }
		void backColor(const Color::RGBA<float>& color) { pBackColor = color; invalidate(); }

		//! Is the text anti-aliased ?
		bool antiAliased() const { return pAntiAliased; }
		//! Set text anti-aliasing
		void antiAliased(bool newValue) { pAntiAliased = newValue; invalidate(); }

		//! Get line height
		const Dimension& lineHeight() const { return pLineHeight; }
		//! Get line height in pixels
		float lineHeightPixels() const { return pLineHeight(pConversion); }
		//! Set line height
		void lineHeight(const Dimension& newValue) { pLineHeight = newValue; }
		//! Set line height in pixels
		void lineHeightPixels(float newValue) { pLineHeight.reset(newValue, Dimension::uPixel); }

		virtual void mouseScroll(float delta, float x, float y) override;

	private:
		//! Text to display
		String pText;

		//! Font to use
		FTFont::Ptr pFont;

		//! Text color
		Color::RGBA<float> pColor;

		//! Background color
		Color::RGBA<float> pBackColor;

		//! Anti-alias the text ?
		bool pAntiAliased;

		//! Position of the edition cursor, in lines and columns
		Point2D<uint> pCursorPos;

		//! Line number of the top-most displayed line (for scrolling)
		uint pTopLineNb;

		//! Height of a line in % of the font pixel size
		Dimension pLineHeight;

		//! Horizontal margin (pixels)
		float pHorizMargin;

		//! Vertical margin (pixels)
		float pVertMargin;

		//! Conversion data for font and text units
		ConversionData pConversion;

	}; // class TextEditor



} // namespace Control
} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_CONTROL_TEXTEDITOR_H__
