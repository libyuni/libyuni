#ifndef __YUNI_UI_CONTROL_WINDOW_H__
# define __YUNI_UI_CONTROL_WINDOW_H__

# include "../../yuni.h"
# include "control.h"


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
		Window(float x, float y, float width, float height):
			IControl(x, y, width, height)
		{}

		Window(const Point2D<float>& position, const Point2D<float>& size):
			IControl(position, size)
		{}

		//! Virtual destructor
		virtual ~Window() {}

		//! Draw the window
		virtual void draw(DrawingSurface::Ptr& surface, float xOffset, float yOffset) const override;

	}; // class Window



} // namespace Control
} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_CONTROL_WINDOW_H__
