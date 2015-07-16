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
#ifndef __YUNI_INPUT_KEYBOARD_H__
# define __YUNI_INPUT_KEYBOARD_H__

# include "../../yuni.h"
# include "../../core/bind.h"
# include "../../core/smartptr.h"
# include "key.h"
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


	//! Keyboard input management
	class Keyboard
	{
	public:
		//! Smart pointer
		typedef SmartPtr<Keyboard>  Ptr;

	public:
		//! Arguments for event callbacks
		struct EventArgs
		{
			EventArgs(const bool* keyStates):
				keys(keyStates),
				propagate(UI::epStop)
			{}

			//! Key states : true if the key is currently pushed
			const bool* const keys;

			//! Should we propagate the event to underlying controls / views ?
			UI::EventPropagation propagate;

		}; // struct EventArgs

	public:
		//! Prototype for a keyboard event callback
		//typedef Yuni::Bind<void (IControl::Ptr sender, EventArgs& args)>  KeyboardCallback;

		//! \name Bindings for user-code event listeners
		//@{
		Yuni::Bind<void (Key key)>  onKeyDown;
		Yuni::Bind<void (Key key)>  onKeyUp;
		//@}

	public:
		//! Constructor
		Keyboard()
		{
			for (uint i = 0; i < KeyCount; ++i)
				pKeyDown[i] = false;
		}

		//! Virtual destructor
		virtual ~Keyboard()
		{
			onKeyDown.unbind();
			onKeyUp.unbind();
		}

		//! Is the key pressed ?
		bool isPressed(Key key) const { return pKeyDown[key]; }

	private:
		//! \name Event notifications from window
		//@{
		void doDown(Key key) { pKeyDown[key] = true; onKeyDown(key); }
		void doUp(Key key) { pKeyDown[key] = false; onKeyUp(key); }
		//@}

	protected:
		//! Keep button state
		volatile bool pKeyDown[KeyCount];

		//! Friend declaration : RenderWindow manages data in this class
		friend class Yuni::UI::RenderWindow;

	}; // class Keyboard



} // namespace Input
} // namespace Yuni


#endif // __YUNI_INPUT_KEYBOARD_H__
