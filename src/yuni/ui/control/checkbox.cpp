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
