# include "button.h"
# include "../theme.h"

namespace Yuni
{
namespace UI
{
namespace Control
{


	void Button::draw(DrawingSurface::Ptr& surface, bool root) const
	{
		if (!pVisible)
			return;

		auto themeptr = Theme::Current();
		auto& theme = *themeptr;
		Point2D<float> pos(pPosition);
		// If the button is the root control, use absolute coordinates
		if (root)
			pos(0, 0);
		surface->beginRectangleClipping(pos.x, pos.y, pSize.x, pSize.y);
		// Manual full-background drawing (FIXME : this is not optimal)
		surface->drawFilledRectangle(theme.borderColor, theme.buttonColor, pos.x, pos.y,
			pSize.x, pSize.y, theme.borderWidth);
		// Try drawing images on top
		if (pBeingClicked && !(!pImageClicking))
			surface->drawImage(pImageClicking, pos.x, pos.y, pSize.x, pSize.y, theme.buttonColor,
				pDisplay, pOffsetX, pOffsetY, pOpacity);
		else if (!(!pImage))
			surface->drawImage(pImage, pos.x, pos.y, pSize.x, pSize.y, theme.buttonColor, pDisplay,
				pOffsetX, pOffsetY, pOpacity);
		// Draw the text if any
		if (!pText.empty())
			surface->drawTextOnColorInRect(pText, theme.font, theme.textColor, theme.buttonColor,
				pos.x, pos.y, pSize.x, pSize.y);
		surface->endClipping();
		pModified = false;
	}



} // namespace Control
} // namespace UI
} // namespace Yuni
