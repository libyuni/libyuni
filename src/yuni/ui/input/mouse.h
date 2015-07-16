/*
** YUNI's default license is the GNU Lesser Public License (LGPL), with some
** exclusions (see below). This basically means that you can get the full source
** code for nothing, so long as you adhere to a few rules.
**
** Under the LGPL you may use YUNI for any purpose you wish, and modify it if you
** require, as long as you:
**
** Pass on the (modified) YUNI source code with your software, with original
** copyrights intact :
**  * If you distribute electronically, the source can be a separate download
**    (either from your own site if you modified YUNI, or to the official YUNI
**    website if you used an unmodified version) – just include a link in your
**    documentation
**  * If you distribute physical media, the YUNI source that you used to build
**    your application should be included on that media
** Make it clear where you have customised it.
**
** In addition to the LGPL license text, the following exceptions / clarifications
** to the LGPL conditions apply to YUNI:
**
**  * Making modifications to YUNI configuration files, build scripts and
**    configuration headers such as yuni/platform.h in order to create a
**    customised build setup of YUNI with the otherwise unmodified source code,
**    does not constitute a derived work
**  * Building against YUNI headers which have inlined code does not constitute a
**    derived work
**  * Code which subclasses YUNI classes outside of the YUNI libraries does not
**    form a derived work
**  * Statically linking the YUNI libraries into a user application does not make
**    the user application a derived work.
**  * Using source code obsfucation on the YUNI source code when distributing it
**    is not permitted.
** As per the terms of the LGPL, a "derived work" is one for which you have to
** distribute source code for, so when the clauses above define something as not
** a derived work, it means you don't have to distribute source code for it.
** However, the original YUNI source code with all modifications must always be
** made available.
*/
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
		//! \name Bindings for user-code event listeners
		//@{
		Yuni::Bind<void (int x, int y)>  onMove;
		Yuni::Bind<void (Button btn, int x, int y)>  onButtonDown;
		Yuni::Bind<void (Button btn, int x, int y)>  onButtonUp;
		Yuni::Bind<void (Button btn, int x, int y)>  onDblClick;
		Yuni::Bind<void (float delta)>  onScroll;
		Yuni::Bind<void (int x, int y)>  onHover;
		Yuni::Bind<void ()>  onLeave;
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
			onLeave.unbind();
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
		void doLeave();
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
