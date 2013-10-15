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
		//! Checkbox checked callback
		Yuni::Bind<void (IControl* sender, bool newValue)>  onCheckChanged;

	public:
		CheckBox(int x, int y, uint width, uint height):
			IControl(x, y, width, height),
			pChecked(false),
			pBeingClicked(false)
		{}

		CheckBox(const Point2D<int>& position, const Point2D<uint>& size):
			IControl(position, size),
			pChecked(false),
			pBeingClicked(false)
		{}

		//! Virtual destructor
		virtual ~CheckBox() {}

		const String& text() const { return pText; }
		void text(const AnyString& text) { pText = text; invalidate(); }

		//! Modify whether the checkbox is checked
		void checked(bool newValue)
		{
			if (pChecked == newValue)
				return;
			pChecked = newValue;
			invalidate();
			// Callback
			onCheckChanged(this, newValue);
		}
		//! Get whether the checkbox is checked
		bool checked() const { return pChecked; }

		//! Draw the checkbox on the surface
		virtual void draw(DrawingSurface::Ptr& surface, bool root) override;

	private:
		//! Control reaction to mouse down
		virtual void mouseDown(Input::IMouse::Button btn, int, int) override
		{
			if (Input::IMouse::ButtonLeft == btn)
				pBeingClicked = true;
		}
		//! Control reaction to mouse up
		virtual void mouseUp(Input::IMouse::Button btn, int, int) override
		{
			if (Input::IMouse::ButtonLeft == btn && pBeingClicked)
			{
				pChecked = !pChecked;
				invalidate();
				onCheckChanged(this, pChecked);
			}
			pBeingClicked = false;
		}

	private:
		bool pChecked;

		bool pBeingClicked;

		String pText;

		String pHoverText;

	}; // class CheckBox





} // namespace Control
} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_CONTROL_CHECKBOX_H__
