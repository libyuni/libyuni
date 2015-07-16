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
#ifndef __YUNI_UI_THEME_H__
# define __YUNI_UI_THEME_H__

# include "../yuni.h"
# include "../core/color/rgba.h"
# include "../core/smartptr.h"
# include "ftfont.h"


namespace Yuni
{
namespace UI
{


	//! The UI theme describes all colors, margins, fonts, ... for displaying UI
	class Theme
	{
	public:
		//! Smart pointer
		typedef SmartPtr<Theme>  Ptr;

	public:
		//! Get the current theme
		static Theme::Ptr Current();
		//! Get the default theme
		static Theme::Ptr Default();
		//! Switch to another theme
		static void SwitchTo(const Theme::Ptr& newTheme);
		//! Switch back to default theme
		static void SwitchToDefault();

	public:
		//! Empty constructor
		Theme();

		//! Copy constructor
		Theme(const Theme& other);

		//! Font
		FTFont::Ptr font;
		FTFont::Ptr monoFont;

		//! Color for text
		Color::RGBA<float> textColor;
		//! Color for window background
		Color::RGBA<float> windowColor;
		//! Color for button background
		Color::RGBA<float> buttonColor;
		Color::RGBA<float> buttonColorDisabled;
		//! Color for window menus
		Color::RGBA<float> menuColor;
		Color::RGBA<float> menuColorDisabled;
		//! Color for control borders
		Color::RGBA<float> borderColor;
		Color::RGBA<float> borderColorDisabled;
		//! Color for panel background
		Color::RGBA<float> panelColor;
		//! Margin for automatic placement of child controls (in pixels)
		float margin;
		//! Width of control borders (in pixels)
		float borderWidth;
		//! Height of window menus (in pixels)
		float menuHeight;
		//! Checkbox size (both width and height, checkboxes are squares)
		float checkBoxSize;

	private:
		static Theme::Ptr sCurrent;
		static Theme::Ptr sDefault;
		static FTFont::Ptr sDefaultFont;
		static FTFont::Ptr sDefaultMonoFont;

	}; // class Theme



} // namespace UI
} // namespace Yuni

# include "theme.hxx"

#endif // __YUNI_UI_THEME_H__
