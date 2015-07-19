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
#include "checkbox.h"
#include "../theme.h"

namespace Yuni
{
namespace UI
{
namespace Control
{


	void CheckBox::draw(DrawingSurface::Ptr& surface, float xOffset, float yOffset) const
	{
		if (!pVisible)
			return;

		auto themeptr = Theme::Current();
		auto& theme = *themeptr;
		Point2D<float> pos(pPosition.x + xOffset, pPosition.y + yOffset);

		surface->beginRectangleClipping(pos.x, pos.y, theme.checkBoxSize, theme.checkBoxSize);

		// Draw the check box
		surface->drawFilledRectangle(theme.borderColor, theme.buttonColor,
			pos.x, pos.y, theme.checkBoxSize, theme.checkBoxSize, theme.borderWidth);

		// Draw the cross if the box is checked
		if (pChecked)
		{
			surface->drawLine(theme.borderColor, theme.buttonColor, pos.x, pos.y,
				pos.x + theme.checkBoxSize, pos.y + theme.checkBoxSize, theme.borderWidth);
			surface->drawLine(theme.borderColor, theme.buttonColor, pos.x + theme.checkBoxSize,
				pos.y, pos.x, pos.y + theme.checkBoxSize, theme.borderWidth);
		}
		surface->endClipping();

		// Draw the label
		surface->drawText(pText, theme.font, theme.textColor,
			pos.x + theme.checkBoxSize + theme.margin, pos.y);
		pModified = false;
	}




} // namespace Control
} // namespace UI
} // namespace Yuni
