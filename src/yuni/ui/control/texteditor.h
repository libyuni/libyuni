#ifndef __YUNI_UI_CONTROL_TEXTEDITOR_H__
# define __YUNI_UI_CONTROL_TEXTEDITOR_H__

# include "../../yuni.h"
# include "../../core/color/rgb.h"
# include "../../core/color/rgba.h"
# include "control.h"
# include "../displaymode.h"
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
			pAntiAliased(true),
			pLineSpacing(40.0f)
		{}

		TextEditor(const Point2D<float>& position, const Point2D<float>& maxSize):
			IControl(position, maxSize),
			pText(),
			pFont(Theme::Current()->monoFont),
			pColor(Theme::Current()->textColor),
			pAntiAliased(true),
			pLineSpacing(40.0f)
		{}

		//! Virtual destructor
		virtual ~TextEditor() {}

		//! Draw the panel
		virtual void draw(DrawingSurface::Ptr& surface, bool root) const override;

		//! Clear the text
		String& clear() { invalidate(); return pText.clear(); }

		//! Get the text
		String& text() { invalidate(); return pText; }
		const String& text() const { return pText; }

		//! Modify the font used
		void font(const UI::FTFont::Ptr& font) { if (pFont != font) { pFont = font; invalidate(); } }
		//! Get the font
		const UI::FTFont::Ptr& font() const { return pFont; }

		//! Modify the color used
		void color(float r, float g, float b) { pColor.assign(r, g, b); invalidate(); }
		void color(float r, float g, float b, float a) { pColor.assign(r, g, b, a); invalidate(); }
		void color(const Color::RGB<float>& color) { pColor = color; invalidate(); }
		void color(const Color::RGBA<float>& color) { pColor = color; invalidate(); }

		//! Is the text anti-aliased ?
		bool antiAliased() const { return pAntiAliased; }
		//! Set text anti-aliasing
		void antiAliased(bool newValue) { pAntiAliased = newValue; invalidate(); }

	private:
		//! Text to display
		String pText;

		//! Font to use
		FTFont::Ptr pFont;

		//! Text color
		Color::RGBA<float> pColor;

		//! Anti-alias the text ?
		bool pAntiAliased;

		//! Pixel spacing between two lines
		float pLineSpacing;

	}; // class TextEditor



} // namespace Control
} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_CONTROL_TEXTEDITOR_H__
