#ifndef __YUNI_UI_CONTROL_PANEL_H__
# define __YUNI_UI_CONTROL_PANEL_H__

# include "../../yuni.h"
# include "control.h"

namespace Yuni
{
namespace UI
{
namespace Control
{


	//! A panel is an unmovable, by default transparent control meant to contain other controls.
	class Panel: public IControl
	{
	public:
		Panel(int x, int y, uint width, uint height):
			IControl(x, y, width, height)
		{}

		Panel(const Point2D<>& position, const Point2D<>& size):
			IControl(position, size)
		{}

		//! Virtual destructor
		virtual ~Panel() {}

		//! Draw the panel
		virtual void draw(DrawingSurface::Ptr& surface, bool root) const override;

	}; // class Panel



} // namespace Control
} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_CONTROL_PANEL_H__
