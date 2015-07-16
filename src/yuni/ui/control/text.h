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
#ifndef __YUNI_UI_CONTROL_TEXT_H__
# define __YUNI_UI_CONTROL_TEXT_H__

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


	//! A text is a control that displays text on the parent surface
	class Text: public IControl
	{
	public:
		//! Smart pointer
		typedef Ancestor::SmartPtrType<Text>::Ptr  Ptr;

	public:
		Text(float x, float y, float maxWidth, float maxHeight):
			IControl(x, y, maxWidth, maxHeight),
			pText(),
			pFont(Theme::Current()->font),
			// White by default
			pColor(Theme::Current()->textColor),
			pAntiAliased(true)
		{}

		Text(const Point2D<float>& position, const Point2D<float>& maxSize):
			IControl(position, maxSize),
			pText(),
			pFont(Theme::Current()->font),
			pColor(Theme::Current()->textColor),
			pAntiAliased(true)
		{}

		//! Virtual destructor
		virtual ~Text() {}

		//! Draw the panel
		virtual void draw(DrawingSurface::Ptr& surface, float xOffset, float yOffset) const override;

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

	}; // class Text



} // namespace Control
} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_CONTROL_TEXT_H__
