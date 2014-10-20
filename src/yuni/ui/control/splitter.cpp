#include "splitter.h"
#include "../theme.h"
#include <cassert>

namespace Yuni
{
namespace UI
{
namespace Control
{


	void Splitter::draw(DrawingSurface::Ptr& surface, float xOffset, float yOffset) const
	{
		if (!pVisible)
			return;

		auto themeptr = Theme::Current();
		auto& theme = *themeptr;
		Point2D<float> pos(pPosition.x + xOffset, pPosition.y + yOffset);

		surface->beginRectangleClipping(pos.x, pos.y, pSize.x, pSize.y);
		// Draw the children
		drawChildren(surface, pos);

		// Draw the splitter line
		switch (pOrient)
		{
			case soVertical:
				surface->drawLine(theme.borderColor, pos.x + pSplitOffset, pos.y,
					pos.x + pSplitOffset, pos.y + pSize.y, theme.borderWidth);
				break;
			case soHorizontal:
				surface->drawLine(theme.borderColor, pos.x, pos.y + pSplitOffset,
					pos.x + pSize.x, pos.y + pSplitOffset, theme.borderWidth);
				break;
			default:
				assert(false && "UI::Control::Splitter : Invalid enum value for SplitOrientation");
				break;
		}

		surface->endClipping();
		pModified = false;
	}


	void Splitter::resizePanels()
	{
		switch (pOrient)
		{
			case soVertical:
				pChildren[0]->moveTo(0.0f, 0.0f);
				pChildren[0]->size(pSplitOffset, pSize.y);
				pChildren[1]->moveTo(pSplitOffset, 0.0f);
				pChildren[1]->size(pSize.x - pSplitOffset, pSize.y);
				break;
			case soHorizontal:
				pChildren[0]->moveTo(0.0f, 0.0f);
				pChildren[0]->size(pSize.x, pSplitOffset);
				pChildren[1]->moveTo(0.0f, pSplitOffset);
				pChildren[1]->size(pSize.x, pSize.y - pSplitOffset);
				break;
			default:
				assert(false && "UI::Control::Splitter : Invalid enum value for SplitOrientation");
				break;
		}
		if (pChildren[0]->modified() || pChildren[1]->modified())
			invalidate();
	}



} // namespace Control
} // namespace UI
} // namespace Yuni
