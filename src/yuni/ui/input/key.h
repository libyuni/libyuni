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
