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
#pragma once
#include "../windows.hdr.h"
#include "console.h"



namespace Yuni
{
namespace System
{
namespace Console
{


	# ifdef YUNI_OS_WINDOWS

	template<class U> inline void ResetTextColor(U&)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}

	template<> struct TextColor<black> final
	{
		template<class U> static void Set(U&)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
		}
	};

	template<> struct TextColor<none> final
	{
		template<class U> static void Set(U&) {}
	};

	template<> struct TextColor<white> final
	{
		template<class U> static void Set(U&)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
	};

	template<> struct TextColor<red> final
	{
		template<class U> static void Set(U&)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		}
	};

	template<> struct TextColor<green> final
	{
		template<class U> static void Set(U&)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		}
	};

	template<> struct TextColor<yellow> final
	{
		template<class U> static void Set(U&)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
		}
	};

	template<> struct TextColor<blue> final
	{
		template<class U> static void Set(U&)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		}
	};

	template<> struct TextColor<purple> final
	{
		template<class U> static void Set(U&)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED);
		}
	};

	template<> struct TextColor<lightblue> final
	{
		template<class U> static void Set(U&)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
		}
	};

	template<> struct TextColor<gray> final
	{
		template<class U> static void Set(U&)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
	};

	template<> struct TextColor<bold> final
	{
		template<class U> static void Set(U&)
		{
		}
	};


	# else


	template<> struct TextColor<none> { template<class U> static void Set(U&) {} };


	template<> struct TextColor<black> final
	{ template<class U> static void Set(U& out) { out << "[1;30m"; } };

	template<> struct TextColor<red> final
	{ template<class U> static void Set(U& out) { out << "[0;31m"; } };

	template<> struct TextColor<green> final
	{ template<class U> static void Set(U& out) { out << "[0;32m"; } };

	template<> struct TextColor<blue> final
	{ template<class U> static void Set(U& out) { out << "[0;34m"; } };

	template<> struct TextColor<yellow> final
	{ template<class U> static void Set(U& out) { out << "[0;33m"; } };

	template<> struct TextColor<purple> final
	{ template<class U> static void Set(U& out) { out << "[0;35m"; } };

	template<> struct TextColor<lightblue> final
	{ template<class U> static void Set(U& out) { out << "[0;36m"; } };

	template<> struct TextColor<gray> final
	{ template<class U> static void Set(U& out) { out << "[0;37m"; } };

	template<> struct TextColor<white> final
	{ template<class U> static void Set(U& out) { out << "[1;37m[1m"; } };

	template<> struct TextColor<bold> final
	{ template<class U> static void Set(U& out) { out << "[1m"; } };


	template<class U> inline void ResetTextColor(U& out)
	{
		out << "[0m";
	}

	# endif




	template<class U> inline void SetTextColor(U& out, const Color color)
	{
		switch (color)
		{
			case black:     TextColor<black>::Set(out); break;
			case red:       TextColor<red>::Set(out); break;
			case green:     TextColor<green>::Set(out); break;
			case yellow:    TextColor<yellow>::Set(out); break;
			case blue:      TextColor<blue>::Set(out); break;
			case purple:    TextColor<purple>::Set(out); break;
			case lightblue: TextColor<lightblue>::Set(out); break;
			case gray:      TextColor<gray>::Set(out); break;
			case white:     TextColor<white>::Set(out); break;
			case bold:      TextColor<bold>::Set(out); break;
			case none:      break;
		}
	}




} // namespace Console
} // namespace System
} // namespace Yuni
