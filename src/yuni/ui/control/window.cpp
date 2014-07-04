# include "window.h"
# include "../theme.h"

namespace Yuni
{
namespace UI
{
namespace Control
{


	void Window::draw(DrawingSurface::Ptr& surface, float xOffset, float yOffset) const
	{
		if (!pVisible)
			return;

		auto themeptr = Theme::Current();
		auto& theme = *themeptr;
		Point2D<float> pos(pPosition.x + xOffset, pPosition.y + yOffset);

		surface->beginRectangleClipping(pos.x, pos.y, pSize.x, pSize.y);
		surface->drawFilledRectangle(theme.borderColor, theme.windowColor,
			pos.x, pos.y, pSize.x, pSize.y, theme.borderWidth);
		// Draw the children
		drawChildren(surface);
		surface->endClipping();
		pModified = false;
	}



} // namespace Control
} // namespace UI
} // namespace Yuni
