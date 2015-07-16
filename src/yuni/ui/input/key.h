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
#ifndef __YUNI_INPUT_KEY_H__
# define __YUNI_INPUT_KEY_H__

namespace Yuni
{
namespace Input
{

	/*!
	** \brief Possible key values
	*/
	enum Key
	{
		Invalid,

		// Main control keys
		Escape,
		Enter,
		Space,
		Tab,
		Insert,
		Delete,
		Backspace,
		CapsLock,
		ScrollLock,
		PrintScreen,
		Pause,
		Cancel,
		Break,
		Clear,
		Select,
		Execute,
		Help,

		// Modifiers
		ShiftL,
		ShiftR,
		AltL,
		AltR,
		ControlL,
		ControlR,
		MetaL,
		MetaR,
		Menu,

		// Motion
		Up,
		Down,
		Left,
		Right,
		PageUp,
		PageDown,
		Home,
		End,

		// Keypad specials
		NumLock, KPEnter, KPDelete, KPInsert, KPSlash, KPAsterisk, KPMinus, KPPlus,

		// Keypad numbers
		KP0, KP1, KP2, KP3, KP4, KP5, KP6, KP7, KP8, KP9,

		// F*
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,

		// Numbers
		N0, N1, N2, N3, N4, N5, N6, N7, N8,	N9,

		// Alphabet
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

		// Symbols
		Backquote,
		Minus,
		Plus,
		Underscore,
		Asterisk,
		Sharp,
		BracketL,
		BracketR,
		ParenL,
		ParenR,
		BraceL,
		BraceR,
		Ampersand,
		Percent,
		Dollar,
		Slash,
		Backslash,
		Colon,
		Semicolon,
		Less,
		Equal,
		Greater,
		Quote,
		Doublequote,
		At,
		Pipe,
		Comma,
		Tilde,
		Circumflex,
		Exclamation,
		Question,
		Dot,

		// Mouse Buttons
		MouseL,
		MouseR,
		MouseMid,
		MouseScrollUp,
		MouseScrollDown,

		// Keep as last element to count the values
		KeyCount
	};



} // namespace Input
} // namespace Yuni

#endif // __YUNI_INPUT_KEY_H__
