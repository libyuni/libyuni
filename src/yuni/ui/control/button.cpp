# include "button.h"
# include "../theme.h"

namespace Yuni
{
namespace UI
{
namespace Control
{

	/*
	void Button::draw(DrawingSurface::Ptr& surface, bool root)
	{
		if (!pVisible)
			return;

		auto themeptr = Theme::Current();
		auto& theme = *themeptr;
		Point2D<> pos(pPosition);
		// If the button is the root control, use absolute coordinates
		if (root)
			pos(0, 0);
		surface->drawRectangle(theme.borderColor, theme.buttonColor,
			pos.x, pos.y, pSize.x, pSize.y, theme.borderWidth);
		surface->beginRectangleClipping(pos.x, pos.y, pSize.x, pSize.y);
		surface->drawTextInRect(pText, theme.font, theme.textColor, pos.x, pos.y,
			pSize.x, pSize.y);
		surface->endClipping();
		pModified = false;
	}
	*/


} // namespace Control
} // namespace UI
} // namespace Yuni
