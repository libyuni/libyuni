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
		//! Smart pointer
		typedef Ancestor::SmartPtrType<CheckBox>::Ptr  Ptr;

		//! Checkbox checked callback
		Yuni::Bind<void (IControl* sender, bool newValue)>  onCheckChanged;

	public:
		CheckBox(float x, float y, float width, float height):
			IControl(x, y, width, height),
			pChecked(false),
			pBeingClicked(false)
		{}

		CheckBox(const Point2D<float>& position, const Point2D<float>& size):
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
		virtual void draw(DrawingSurface::Ptr& surface, float xOffset, float yOffset) const override;

	private:
		//! Control reaction to mouse down
		virtual EventPropagation mouseDown(Input::IMouse::Button btn, float, float) override
		{
			if (Input::IMouse::ButtonLeft == btn)
				pBeingClicked = true;
			return epStop;
		}
		//! Control reaction to mouse up
		virtual EventPropagation mouseUp(Input::IMouse::Button btn, float, float) override
		{
			if (Input::IMouse::ButtonLeft == btn && pBeingClicked)
			{
				pChecked = !pChecked;
				invalidate();
				onCheckChanged(this, pChecked);
			}
			pBeingClicked = false;
			return epStop;
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
