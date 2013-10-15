#ifndef __YUNI_UI_CONTROL_BUTTON_H__
# define __YUNI_UI_CONTROL_BUTTON_H__

# include "../../yuni.h"
# include "control.h"
# include "../font.h"

namespace Yuni
{
namespace UI
{
namespace Control
{


	//! A button is a clickable control that triggers an event
	class Button: public IControl
	{
	public:
		//! Type of bind for the button callback
		typedef Yuni::Bind<void (void)>  Callback;

	public:
		Button(int x, int y, uint width, uint height):
			IControl(x, y, width, height)
		{}

		Button(const Point2D<>& position, const Point2D<>& size):
			IControl(position, size)
		{}

		//! Virtual destructor
		virtual ~Button() {}

		const String& text() const { return pText; }
		void text(const AnyString& text) { pText = text; invalidate(); }

		//! Launch a click event
		void click() const { pOnClick(); }

		//! Bind the onClick event
		template<class FuncT>
		void onClick(const FuncT& function) { pOnClick.bind(function); }

		//! Bind the onClick event
		template<class ClassT, class MethodT>
		void onClick(const ClassT& object, const MethodT& method) { pOnClick.bind(object, method); }

		//! Draw the button on the surface
		//		virtual void draw(DrawingSurface::Ptr& surface, bool root);

	private:
		Callback pOnClick;

		String pText;

		String pHoverText;

	}; // class Button



} // namespace Control
} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_CONTROL_BUTTON_H__
