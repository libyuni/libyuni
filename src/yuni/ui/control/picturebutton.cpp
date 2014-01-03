# include "picturebutton.h"

namespace Yuni
{
namespace UI
{
namespace Control
{


	void PictureButton::draw(DrawingSurface::Ptr& surface, bool root)
	{
		if (!pVisible)
			return;

		Point2D<int> pos(pPosition);
		// If the button is the root control, use absolute coordinates
		if (root)
			pos(0, 0);
		if (pBeingClicked && !(!pImageClicking))
			surface->drawImage(pImageClicking, pos.x, pos.y, pSize.x, pSize.y);
		else if (!(!pImage))
			surface->drawImage(pImage, pos.x, pos.y, pSize.x, pSize.y);
		pModified = false;
	}



} // namespace Control
} // namespace UI
} // namespace Yuni
