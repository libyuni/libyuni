#ifndef __YUNI_UI_CONTROL_H__
# define __YUNI_UI_CONTROL_H__

# include "../../yuni.h"
# include "../../core/event.h"
# include "../../core/smartptr.h"
# include "../../core/smartptr/intrusive.h"
# include "../../core/point2D.h"
# include "../../core/dictionary.h"
# include "../../core/functional/binaryfunctions.h"
# include <vector>
# include "../gl/drawingsurface.h"
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
	class IControl : public IIntrusiveSmartPtr<IControl>, public IEventObserver<IControl>
	{
	public:
		typedef IIntrusiveSmartPtr<IControl> Ancestor;
		//! Smart pointer
		typedef Ancestor::SmartPtrType<IControl>::Ptr  Ptr;
		//! Vector of Controls
		typedef std::vector<Ptr>  Vector;

		//! Unordered set of controls (by ptr)
		typedef Yuni::Set<Ptr>::Ordered  Set;


	public:
		//! Mouse move callback
		Yuni::Event<EventPropagation (IControl* sender, float x, float y)>  onMouseMove;
		//! Mouse down callback
		Yuni::Event<EventPropagation (IControl* sender, Input::IMouse::Button btn, float x, float y)>  onMouseDown;
		//! Mouse up callback
		Yuni::Event<EventPropagation (IControl* sender, Input::IMouse::Button btn, float x, float y)>  onMouseUp;
		//! Mouse double-click callback
		Yuni::Event<EventPropagation (IControl* sender, Input::IMouse::Button btn, float x, float y)>  onMouseDblClick;
		//! Mouse scroll callback
		Yuni::Event<EventPropagation (IControl* sender, float delta)>  onMouseScroll;
		//! Mouse enter callback
		Yuni::Event<EventPropagation (IControl* sender, float x, float y)>  onMouseEnter;
		//! Mouse leave callback
		Yuni::Event<EventPropagation (IControl* sender, float x, float y)>  onMouseLeave;
		//! Mouse hover callback
		Yuni::Event<EventPropagation (IControl* sender, float x, float y)>  onMouseHover;
		//! Key down callback
		Yuni::Event<EventPropagation (IControl* sender, Input::Key)>  onKeyDown;
		//! Key up callback
		Yuni::Event<EventPropagation (IControl* sender, Input::Key)>  onKeyUp;

	public:
		//! Empty constructor
		IControl():
			pPosition(0, 0),
			pSize(20u, 20u),
			pVisible(true),
			pModified(true),
			pReadOnly(false),
			pEnabled(true),
			pOpacity(1.0f)
		{}

		//! Constructor with integers for position and size
		IControl(float x, float y, float width, float height):
			pPosition(x, y),
			pSize(Math::Max(width, 0.0f), Math::Max(height, 0.0f)),
			pVisible(true),
			pModified(true),
			pReadOnly(false),
			pEnabled(true),
			pOpacity(1.0f)
		{}

		//! Constructor with points for position and size
		IControl(const Point2D<float>& position, const Point2D<float>& size):
			pPosition(position),
			pSize(size),
			pVisible(true),
			pModified(true),
			pReadOnly(false),
			pEnabled(true),
			pOpacity(1.0f)
		{}

		//! Virtual destructor
		virtual ~IControl() { destroyBoundEvents(); }

		//! Get X position
		float x() const { return pPosition.x; }
		//! Get Y position
		float y() const { return pPosition.y; }
		//! Get position
		const Point2D<float> position() const { return pPosition; }
		//! Set position to an absolute value
		void moveTo(float x, float y)
		{
			pPosition(x, y);
			invalidate();
		}
		void moveTo(const Point2D<float>& position)
		{
			pPosition(position);
			invalidate();
		}
		//! Move position by a relative value
		void moveBy(float x, float y)
		{
			pPosition.x += x;
			pPosition.y += y;
			invalidate();
		}

		//! Get width
		float width() const { return pSize.x; }
		//! Get height
		float height() const { return pSize.y; }
		//! Get size
		const Point2D<float>& size() const { return pSize; }
		//! Set size
		void size(float width, float height)
		{
			pSize(width, height);
			invalidate();
		}
		template<class T>
		void size(const Point2D<T>& size)
		{
			pSize(size);
			invalidate();
		}
		void zoom(float factor) { pSize(pSize.x * factor, pSize.y * factor); invalidate(); }

		//! Cursor used when hovering the control
		// const Cursor& cursor() const;
		// void cursor(const Cursor& c);

		//! Get whether the control is currently enabled
		bool enabled() const { return pEnabled; }

		//! Get the visibility of the control
		bool visible() const { return pVisible; }
		//! Set the visibility of the control
		void show() { show(true); }
		void hide() { show(false); }
		void show(bool visible);

		//! Draw the control
		virtual void draw(DrawingSurface::Ptr& surface, float xOffset = 0.0f, float yOffset = 0.0f) const = 0;

		//! Recursively search for the deepest child control containing the given point
		IControl* getControlAt(float x, float y);
		IControl* getControlAt(const Point2D<float>& point) { return getControlAt(point.x, point.y); }

		//! Add child controls
		void addChild(const IControl::Ptr& child) { pChildren.push_back(child); invalidate(); }
		void addChild(IControl* child) { pChildren.push_back(child); invalidate(); }

		//! Remove all child controls
		void clearChildren() { pChildren.clear(); invalidate(); }

		//! Invalidate the control (force redraw)
		void invalidate()
		{
			pModified = true;
		}
		//! Is the control or any of its children modified ?
		bool modified() const;

		//! Get control opacity
		float opacity() const { return pOpacity; }
		//! Set control opacity
		void opacity(float newOpacity) { pOpacity = newOpacity; invalidate(); }

	protected:
		//! Check if a given point is inside the control's rectangle
		template<class T>
		bool contains(const T& point) const
		{
			return contains(point.x, point.y);
		}
		template<class T>
		bool contains(T x, T y) const
		{
			return x >= pPosition.x && x <= pPosition.x + pSize.x &&
				y >= pPosition.y && y <= pPosition.y + pSize.y;
		}

		//! Recursively search for the controls containing the given point, return them stacked
		void getControlStackAt(float x, float y, std::vector<IControl*>& stack);

		//! Draw the child controls
		void drawChildren(DrawingSurface::Ptr& surface) const
		{
			for (auto& child : pChildren)
				child->draw(surface, pPosition.x, pPosition.y);
		}

		//! Launch a mouse move event
		EventPropagation doMouseMove(float x, float y, Set& enteredControls);
		//! Launch a mouse down event
		EventPropagation doMouseDown(Input::IMouse::Button btn, float x, float y);
		//! Launch a mouse up event
		EventPropagation doMouseUp(Input::IMouse::Button btn, float x, float y);
		//! Launch a mouse double-click event
		EventPropagation doMouseDblClick(Input::IMouse::Button btn, float x, float y);
		//! Launch a mouse scroll event
		EventPropagation doMouseScroll(float delta, float x, float y);
		//! Launch a mouse enter event
		EventPropagation doMouseEnter(float x, float y);
		//! Launch a mouse leave event
		EventPropagation doMouseLeave(float x, float y);
		//! Launch a mouse hover event
		EventPropagation doMouseHover(float x, float y);
		//! Launch a key down event
		EventPropagation doKeyDown(Input::Key key);
		//! Launch a key up event
		EventPropagation doKeyUp(Input::Key key);

		//! Control reaction to mouse move
		virtual EventPropagation mouseMove(float, float) { return epContinue; }
		//! Control reaction to mouse down
		virtual EventPropagation mouseDown(Input::IMouse::Button, float, float) { return epContinue; }
		//! Control reaction to mouse up
		virtual EventPropagation mouseUp(Input::IMouse::Button, float, float) { return epContinue; }
		//! Control reaction to mouse double-click
		virtual EventPropagation mouseDblClick(Input::IMouse::Button, float, float) { return epContinue; }
		//! Control reaction to mouse scroll
		virtual EventPropagation mouseScroll(float, float, float) { return epContinue; }
		//! Control reaction to mouse enter
		virtual EventPropagation mouseEnter(float, float) { return epContinue; }
		//! Control reaction to mouse enter
		virtual EventPropagation mouseLeave(float, float) { return epContinue; }
		//! Control reaction to mouse hover
		virtual EventPropagation mouseHover(float, float) { return epContinue; }
		//! Control reaction to key up
		virtual EventPropagation keyDown(Input::Key) { return epContinue; }
		//! Control reaction to key down
		virtual EventPropagation keyUp(Input::Key) { return epContinue; }

	private:
		template<class... Args>
		static EventPropagation eventFold(const Event<EventPropagation (Args...)>& event, Args... args)
		{
			return event.fold(epContinue, Functional::Max<EventPropagation>(), args...);
		}

	protected:
		Point2D<float> pPosition;

		Point2D<float> pSize;

		bool pVisible;

		mutable bool pModified;

		bool pReadOnly;

		bool pEnabled;

		//! Opacity of the control : 0 is transparent, 1 is opaque
		float pOpacity;

		//! Child controls
		Vector pChildren;

		//! The render window will dispatch events to the private handlers
		friend class Yuni::UI::View;

	}; // class IControl



} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_CONTROL_H__
