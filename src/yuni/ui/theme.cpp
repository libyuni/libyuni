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
# include "theme.h"

namespace Yuni
{
namespace UI
{

	Theme::Ptr Theme::sDefault = new Theme();

	Theme::Ptr Theme::sCurrent = nullptr;

	Theme::Theme():
		font(new FTFont("data/fonts/OpenSans-Light.ttf", 12)),
		monoFont(new FTFont("data/fonts/DejaVuSansMono.ttf", 10)),
		textColor(0.0f, 0.0f, 0.0f), // Opaque Black
 		windowColor(0.9882f, 0.9882f, 0.9882f), // (0xfcfcfc)
		buttonColor(0.2863f, 0.5451f, 0.651f), // (73, 139, 166)
		buttonColorDisabled(0.2863f, 0.5451f, 0.651f), // (103, 120, 139)
		menuColor(0.2863f, 0.5451f, 0.651f), // (73, 139, 166)
		menuColorDisabled(0.2863f, 0.5451f, 0.651f), // (103, 120, 139)
		borderColor(0.1882f, 0.3490f, 0.451f), // (48, 89, 115)
		borderColorDisabled(0.1882f, 0.3490f, 0.451f), // (102, 100, 126)
		panelColor(0.0f, 0.0f, 0.0f, 0.0f), // Full transparent
		margin(10),
		borderWidth(2.5f),
		menuHeight(20),
		checkBoxSize(15)
	{}


	Theme::Theme(const Theme& other):
		font(other.font),
		monoFont(other.monoFont),
		textColor(other.textColor),
 		windowColor(other.windowColor),
		buttonColor(other.buttonColor),
		buttonColorDisabled(other.buttonColorDisabled),
		menuColor(other.menuColor),
		menuColorDisabled(other.menuColorDisabled),
		borderColor(other.borderColor),
		borderColorDisabled(other.borderColorDisabled),
		panelColor(other.panelColor),
		margin(other.margin),
		borderWidth(other.borderWidth),
		menuHeight(other.menuHeight),
		checkBoxSize(other.checkBoxSize)
	{}


	void Theme::SwitchTo(const Theme::Ptr& newTheme)
	{
		if (!(!newTheme))
			sCurrent = newTheme;
	}


	void Theme::SwitchToDefault()
	{
		sCurrent = sDefault;
	}



} // namespace UI
} // namespace Yuni
