#include "panel.h"
#include "../theme.h"

namespace Yuni
{
namespace UI
{
namespace Control
{

	Panel::Panel(float x, float y, float width, float height):
		IControl(x, y, width, height)
	{
		pColor = Theme::Current()->panelColor;
	}


	Panel::Panel(const Point2D<float>& position, const Point2D<float>& size):
		IControl(position, size)
	{
		pColor = Theme::Current()->panelColor;
	}


	void Panel::draw(DrawingSurface::Ptr& surface, float xOffset, float yOffset) const
	{
		if (!pVisible)
			return;

		auto themeptr = Theme::Current();
		auto& theme = *themeptr;
		Point2D<float> pos(pPosition.x + xOffset, pPosition.y + yOffset);
		surface->beginRectangleClipping(pos.x, pos.y, pSize.x, pSize.y);
		if (pColor.alpha > 0)
		{
			surface->drawFilledRectangle(pColor, pColor,
				pos.x, pos.y, pSize.x, pSize.y, theme.borderWidth);
		}
		// Draw the children
		drawChildren(surface, pos);
		surface->endClipping();
		pModified = false;
	}



} // namespace Control
} // namespace UI
} // namespace Yuni
