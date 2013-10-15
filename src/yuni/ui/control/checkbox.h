#ifndef __YUNI_UI_CONTROL_CHECKBOX_H__
# define __YUNI_UI_CONTROL_CHECKBOX_H__

# include "../../yuni.h"
# include "control.h"
# include "../font.h"

namespace Yuni
{
namespace UI
{
namespace Control
{


	//! A check box is a clickable control that has a boolean state: either checked or unchecked
	class CheckBox: public IControl
	{
	public:
		//! Type of bind for the checkbox callback
		typedef Yuni::Bind<void (bool newValue)>  Callback;

	public:
		CheckBox(int x, int y):
			IControl(x, y, 0u, 0u),
			pChecked(false)
		{}

		CheckBox(const Point2D<int>& position):
			IControl(position.x, position.y, 0u, 0u),
			pChecked(false)
		{}

		//! Virtual destructor
		virtual ~CheckBox() {}

		const String& text() const { return pText; }
		void text(const AnyString& text) { pText = text; invalidate(); }

		//! Modify the value
		void value(bool newValue) { pChecked = newValue; invalidate(); pOnChanged(newValue); }
		//! Get the current value
		bool value() const { return pChecked; }

		//! Bind the onClick event
		template<class FuncT>
		void onChanged(const FuncT& function) { pOnChanged.bind(function); }

		//! Bind the onClick event
		template<class ClassT, class MethodT>
		void onChanged(const ClassT& object, const MethodT& method) { pOnChanged.bind(object, method); }

		//! Draw the checkbox on the surface
		//		virtual void draw(DrawingSurface::Ptr& surface, bool root);

	private:
		Callback pOnChanged;

		bool pChecked;

		String pText;

		String pHoverText;

	}; // class CheckBox



} // namespace Control
} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_CONTROL_CHECKBOX_H__
