# include "picture.h"
# include "../theme.h"

namespace Yuni
{
namespace UI
{
namespace Control
{


	void Picture::draw(DrawingSurface::Ptr& surface, bool root) const
	{
		if (!pVisible)
			return;

		auto themeptr = Theme::Current();
		auto& theme = *themeptr;
		Point2D<float> pos(pPosition);
		// If the picture is the root control, use absolute coordinates
		if (root)
			pos(0, 0);
		surface->beginRectangleClipping(pos.x, pos.y, pSize.x, pSize.y);
		// Manual full-background drawing (FIXME : this is not optimal)
		surface->drawFilledRectangle(theme.borderColor, pFillColor, pos.x, pos.y,
			pSize.x, pSize.y, theme.borderWidth);
		// Then try drawing the image
		if (!(!pImage))
			surface->drawImage(pImage, pos.x, pos.y, pSize.x, pSize.y, pFillColor, pDisplay,
				pOffsetX, pOffsetY, pOpacity);
		surface->endClipping();
		pModified = false;
	}



} // namespace Control
} // namespace UI
} // namespace Yuni
