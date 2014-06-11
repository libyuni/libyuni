#include "text.h"

namespace Yuni
{
namespace UI
{
namespace Control
{


	void Text::draw(DrawingSurface::Ptr& surface, bool root) const
	{
		if (!pVisible)
			return;

		Point2D<float> pos(pPosition);
		// If the text is the root control, use absolute coordinates
		if (root)
			pos(0.0f, 0.0f);
		surface->beginRectangleClipping(pos.x, pos.y, pSize.x, pSize.y);
		if (!pText.empty())
			surface->drawText(pText, pFont, pColor, pos.x, pos.y);
		surface->endClipping();
		pModified = false;
	}



} // namespace Control
} // namespace UI
} // namespace Yuni
