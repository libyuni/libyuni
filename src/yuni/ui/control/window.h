#ifndef __YUNI_UI_CONTROL_WINDOW_H__
# define __YUNI_UI_CONTROL_WINDOW_H__

# include "../../yuni.h"
# include "control.h"
//# include "drawingsurface.h"

namespace Yuni
{
namespace UI
{
namespace Control
{


	//! A window is a movable control that is meant to contain other controls
	class Window: public IControl
	{
	public:
		Window(int x, int y, uint width, uint height):
			IControl(x, y, width, height)
		{}

		Window(const Point2D<>& position, const Point2D<>& size):
			IControl(position, size)
		{}

		//! Virtual destructor
		virtual ~Window() {}

		//! Draw the window
		virtual void draw(DrawingSurface::Ptr& surface, bool root);

	}; // class Window



} // namespace Control
} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_CONTROL_WINDOW_H__
