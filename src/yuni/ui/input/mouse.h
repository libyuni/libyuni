#ifndef __YUNI_INPUT_MOUSE_H__
# define __YUNI_INPUT_MOUSE_H__

# include "../../yuni.h"
# include "../../core/bind.h"
# include "../../core/smartptr.h"
# include "../../core/point2D.h"
# include "../eventpropagation.h"

namespace Yuni
{

namespace UI
{

	//! Forward declaration
	class RenderWindow;

} // namespace UI


namespace Input
{


	//! Mouse input management
	class IMouse
	{
	public:
		//! Smart pointer
		typedef SmartPtr<IMouse>  Ptr;

	public:
		//! Button types
		enum Button
		{
			ButtonLeft,

			ButtonRight,

			ButtonMiddle,

			ButtonCount // Used to count the enum values, please keep at end of enum

		}; // enum Button

	public:
		//! Arguments for event callbacks
		struct EventArgs
		{
			EventArgs(int xPos, int yPos, const bool* buttonStates, float delta):
				pos(xPos, yPos),
				buttons(buttonStates),
				propagate(UI::epStop),
				scrollDelta(delta)
			{}

			EventArgs(int xPos, int yPos, const bool* buttonStates):
				pos(xPos, yPos),
				buttons(buttonStates),
				propagate(UI::epStop),
				scrollDelta(0)
			{}

			//! X position of the cursor
			Point2D<int> pos;

			//! Mouse button states : true if the button is currently clicked
			const bool* const buttons;

			//! Should we propagate the event to underlying controls / views ?
			UI::EventPropagation propagate;

			//! Delta between current and previous position of the scroll wheel
			float scrollDelta;

		}; // struct EventArgs

	public:
		//! Prototype for a mouse event callback
		//typedef Yuni::Bind<void (IControl::Ptr sender, EventArgs& args)>  MouseCallback;

		//! \name Bindings for user-code event listeners
		//@{
		Yuni::Bind<void (int x, int y)>  onMove;
		Yuni::Bind<void (Button btn, int x, int y)>  onButtonDown;
		Yuni::Bind<void (Button btn, int x, int y)>  onButtonUp;
		Yuni::Bind<void (Button btn, int x, int y)>  onDblClick;
		Yuni::Bind<void (float delta)>  onScroll;
		Yuni::Bind<void (int x, int y)>  onHover;
		//@}

	public:
		//! Constructor
		IMouse()
		{
			for (uint i = 0; i < ButtonCount; ++i)
				pButtonClicked[i] = false;
		}

		//! Virtual destructor
		virtual ~IMouse()
		{
			onMove.unbind();
			onButtonDown.unbind();
			onButtonUp.unbind();
			onDblClick.unbind();
			onScroll.unbind();
			onHover.unbind();
		}

		//! Is the button clicked ?
		bool isClicked(Button btn) const { return pButtonClicked[btn]; }

		//! Cursor position (in pixels)
		const Point2D<int>& pos() const { return pPos; }

		const Point2D<int>& previousPos() const { return pLastPos; }

		const Point2D<int>& dragStartPos() const { return pDragPos; }

		//! \name Cursor visibility
		//@{
		virtual void showCursor() = 0;
		virtual void hideCursor() = 0;
		virtual void showCursor(bool visible) = 0;
		//@}

	private:
		//! \name Event notifications from window
		//@{
		void doMove(int x, int y);
		void doDown(Button btn);
		void doUp(Button btn);
		void doDblClick(Button btn);
		//! Up is +, Down is -
		void doScroll(float scale);
		void doHover(int x, int y);
		//@}

	protected:
		//! Current position
		Point2D<int> pPos;
		//! Previous position
		Point2D<int> pLastPos;
		//! Position at the start of dragging
		Point2D<int> pDragPos;

		//! Keep button state
		bool pButtonClicked[ButtonCount];

		//! Friend declaration : RenderWindow manages data in this class
		friend class Yuni::UI::RenderWindow;

	}; // class IMouse



} // namespace Input
} // namespace Yuni


#endif // __YUNI_INPUT_MOUSE_H__
