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
		pModified = false;
	}



} // namespace Control
} // namespace UI
} // namespace Yuni
