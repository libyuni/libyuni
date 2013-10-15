#include "checkbox.h"
#include "../theme.h"

namespace Yuni
{
namespace UI
{
namespace Control
{

	/*
	void CheckBox::draw(DrawingSurface::Ptr& surface, bool root)
	{
		if (!pVisible)
			return;

		auto themeptr = Theme::Current();
		auto& theme = *themeptr;
		Point2D<> pos(pPosition);
		// If the button is the root control, use absolute coordinates
		if (root)
			pos(0, 0);
		// Draw the check box
		surface->drawRectangle(theme.borderColor, theme.buttonColor,
			pos.x, pos.y, theme.checkBoxSize, theme.checkBoxSize, theme.borderWidth);
		// Dimension our control around the box only, not the label, for mouse event handling.
		pSize(theme.checkBoxSize, theme.checkBoxSize);
		// Draw the cross if the box is checked
		if (pChecked)
		{
			surface->drawLine(theme.borderColor, pos.x, pos.y,
				pos.x + theme.checkBoxSize, pos.y + theme.checkBoxSize, theme.borderWidth);
			surface->drawLine(theme.borderColor, pos.x + theme.checkBoxSize, pos.y,
				pos.x, pos.y + theme.checkBoxSize, theme.borderWidth);
		}
		// Draw the label
		surface->drawText(pText, theme.font, theme.textColor,
			pos.x + theme.checkBoxSize + theme.margin, pos.y);
		pModified = false;
	}
	*/


} // namespace Control
} // namespace UI
} // namespace Yuni
