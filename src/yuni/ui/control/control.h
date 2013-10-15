#ifndef __YUNI_UI_CONTROL_H__
# define __YUNI_UI_CONTROL_H__

# include "../../yuni.h"
# include "../../core/smartptr.h"
# include "../../core/point2D.h"
# include <vector>
//# include "drawingsurface.h"
# include "../input/key.h"
# include "../input/mouse.h"
# include "../eventpropagation.h"

namespace Yuni
{
namespace UI
{

	//! Forward declaration
	class View;


	//! A UI control is a part of 2D overlay that reacts to certain events
	class IControl
	{
	public:
		typedef SmartPtr<IControl>  Ptr;
		typedef std::vector<Ptr>  Vector;

	public:
		Yuni::Bind<EventPropagation (IControl* sender, int x, int y)>  onMouseMove;
		Yuni::Bind<EventPropagation (IControl* sender, Input::IMouse::Button btn, int x, int y)>  onMouseDown;
		Yuni::Bind<EventPropagation (IControl* sender, Input::IMouse::Button btn, int x, int y)>  onMouseUp;
		Yuni::Bind<EventPropagation (IControl* sender, Input::IMouse::Button btn, int x, int y)>  onMouseDblClick;
		Yuni::Bind<EventPropagation (IControl* sender, float delta)>  onMouseScroll;
		Yuni::Bind<EventPropagation (IControl* sender, int x, int y)>  onMouseHover;
		Yuni::Bind<EventPropagation (IControl* sender, Input::Key)>  onKeyDown;
		Yuni::Bind<EventPropagation (IControl* sender, Input::Key)>  onKeyUp;

	public:
		IControl():
			pPosition(0, 0),
			pSize(20u, 20u),
			pVisible(true),
			pModified(true),
			pReadOnly(false),
			pEnabled(true)
		{}

		IControl(int x, int y, uint width, uint height):
			pPosition(x, y),
			pSize(width, height),
			pVisible(true),
			pModified(true),
			pReadOnly(false),
			pEnabled(true)
		{}

		IControl(const Point2D<int>& position, const Point2D<uint>& size):
			pPosition(position),
			pSize(size),
			pVisible(true),
			pModified(true),
			pReadOnly(false),
			pEnabled(true)
		{}

		virtual ~IControl() {}

		int x() const { return pPosition.x; }

		int y() const { return pPosition.y; }

		uint width() const { return pSize.x; }

		uint height() const { return pSize.y; }

		//! Cursor used when hovering the control
		// const Cursor& cursor() const;
		// void cursor(const Cursor& c);

		void moveTo(int x, int y)
		{
			pPosition(x, y);
		}

		void moveBy(int x, int y)
		{
			pPosition.x += x;
			pPosition.y += y;
		}

		void moveTo(const Point2D<int>& position)
		{
			pPosition(position);
		}

		void resize(uint width, uint height)
		{
			pSize(width, height);
		}

		void resize(const Point2D<uint>& size)
		{
			pSize(size);
		}

		//! Get whether the control is currently enabled
		bool enabled() const { return pEnabled; }

		//! Get the visibility of the control
		bool visible() const { return pVisible; }
		//! Set the visibility of the control
		void show() { show(true); }
		void hide() { show(false); }
		void show(bool visible);

		//! Draw the control
		//		virtual void draw(DrawingSurface::Ptr& surface, bool root) = 0;

		//! Recursively search for the deepest child control containing the given point
		IControl* getControlAt(int x, int y);

		//! Add child controls
		void addChild(const IControl::Ptr& child) { pChildren.push_back(child); }
		void addChild(IControl* child) { pChildren.push_back(child); }

		//! Invalidate the control (force redraw)
		void invalidate()
		{
			pModified = true;
		}
		//! Is the control or any of its children modified ?
		bool modified() const;

	protected:
		//! Recursively search for the controls containing the given point, return them stacked
		void getControlStackAt(int x, int y, std::vector<IControl*>& stack);

		//! Draw the child controls
		// void drawChildren(DrawingSurface::Ptr& surface)
		// {
		// 	for (auto& child : pChildren)
		// 		child->draw(surface, false);
		// }

		EventPropagation doMouseMove(int x, int y);
		EventPropagation doMouseDown(Input::IMouse::Button btn, int x, int y);
		EventPropagation doMouseUp(Input::IMouse::Button btn, int x, int y);
		EventPropagation doMouseDblClick(Input::IMouse::Button btn, int x, int y);
		EventPropagation doMouseScroll(float delta, int x, int y);
		EventPropagation doMouseHover(int x, int y);
		EventPropagation doKeyDown(Input::Key);
		EventPropagation doKeyUp(Input::Key);

	protected:
		Point2D<int> pPosition;

		Point2D<uint> pSize;

		bool pVisible;

		bool pModified;

		bool pReadOnly;

		bool pEnabled;

		//! Child controls
		Vector pChildren;

		//! The render window will dispatch events to the private handlers
		friend class Yuni::UI::View;

	}; // class IControl



} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_CONTROL_H__
