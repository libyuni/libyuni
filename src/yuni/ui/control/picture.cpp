# include "picture.h"

namespace Yuni
{
namespace UI
{
namespace Control
{


	void Picture::draw(DrawingSurface::Ptr& surface, float xOffset, float yOffset) const
	{
		if (!pVisible)
			return;

		Point2D<float> pos(pPosition.x + xOffset, pPosition.y + yOffset);

		surface->beginRectangleClipping(pos.x, pos.y, pSize.x, pSize.y);
		// Manual full-background drawing (FIXME : this is not optimal)
		surface->drawFilledRectangle(pFillColor, pFillColor, pos.x, pos.y,
			pSize.x, pSize.y, 0);
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
