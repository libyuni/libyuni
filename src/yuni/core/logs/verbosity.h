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
#include "../system/console.h"



namespace Yuni
{
namespace Logs
{
namespace Verbosity
{


	struct YUNI_DECL Unknown final
	{
		static const char* Name() {return "";}
		template<class U> static void AppendName(U&) {}
		enum
		{
			level = 0,
			shouldUsesStdCerr = 0,
			hasName = 0,
			enabled = 1,
		};
		//! Text Color for displaying the verbosity
		static const System::Console::Color color = System::Console::none;
		//! Message Text Color
		static const System::Console::Color messageColor = System::Console::none;

	}; // class Unknown


	struct YUNI_DECL Quiet final
	{
		static const char* Name() {return "quiet";}
		template<class U> static void AppendName(U& u) {u << "[quiet]";}
		enum
		{
			level = 100000, // equivalent to infinite
			shouldUsesStdCerr = 1,
			hasName = 1,
			enabled = 1,
		};
		//! Text Color for displaying the verbosity
		static const System::Console::Color color = System::Console::none;
		//! Message Text Color
		static const System::Console::Color messageColor = System::Console::none;

	}; // class Quiet



	struct YUNI_DECL Fatal final
	{
		static const char* Name() {return "fatal";}
		template<class U> static void AppendName(U& u) {u << "[fatal]";}
		enum
		{
			level = 1000,
			shouldUsesStdCerr = 1,
			hasName = 1,
			enabled = 1,
		};
		//! Text Color for displaying the verbosity
		static const System::Console::Color color = System::Console::red;
		//! Message Text Color
		static const System::Console::Color messageColor = System::Console::none;

	}; // class Fatal



	struct YUNI_DECL Error final
	{
		static const char* Name() {return "error";}
		template<class U> static void AppendName(U& u) {u << "[error]";}
		enum
		{
			level = 2000,
			shouldUsesStdCerr = 1,
			hasName = 1,
			enabled = 1,
		};
		//! Text Color for displaying the verbosity
		static const System::Console::Color color = System::Console::red;
		//! Message Text Color
		static const System::Console::Color messageColor = System::Console::none;

	}; // class Error



	struct YUNI_DECL Warning final
	{
		static const char* Name() {return "warning";}
		template<class U> static void AppendName(U& u) {u << "[warns]";}
		enum
		{
			level = 3000,
			shouldUsesStdCerr = 1,
			hasName = 1,
			enabled = 1,
		};
		//! Text Color for displaying the verbosity
		static const System::Console::Color color = System::Console::yellow;
		//! Message Text Color
		static const System::Console::Color messageColor = System::Console::none;

	}; // class Warning



	struct YUNI_DECL Checkpoint final
	{
		static const char* Name() {return "checkpoint";}
		template<class U> static void AppendName(U& u) {u << "[check]";}

		enum
		{
			level = 4000,
			shouldUsesStdCerr = 0,
			hasName = 1,
			enabled = 1,
		};
		//! Text Color for displaying the verbosity
		static const System::Console::Color color = System::Console::white;
		//! Message Text Color
		static const System::Console::Color messageColor = System::Console::white;

	}; // class Checkpoint



	struct YUNI_DECL Notice final
	{
		static const char* Name() {return "notice";}
		template<class U> static void AppendName(U& u) {u << "[notic]";}

		enum
		{
			level = 5000,
			shouldUsesStdCerr = 0,
			hasName = 1,
			enabled = 1,
		};
		//! Text Color for displaying the verbosity
		static const System::Console::Color color = System::Console::green;
		//! Message Text Color
		static const System::Console::Color messageColor = System::Console::none;

	}; // class Notice


	struct YUNI_DECL Progress final
	{
		static const char* Name() {return "progress";}
		template<class U> static void AppendName(U& u) {u << "[progress]";}

		enum
		{
			level = 6000,
			shouldUsesStdCerr = 0,
			hasName = 1,
			enabled = 1,
		};
		//! Text Color for displaying the verbosity
		static const System::Console::Color color = System::Console::none;
		//! Message Text Color
		static const System::Console::Color messageColor = System::Console::none;

	}; // class Progress



	struct YUNI_DECL Info final
	{
		static const char* Name() {return "info";}
		template<class U> static void AppendName(U& u) {u << "[infos]";}

		enum
		{
			level = 7000,
			shouldUsesStdCerr = 0,
			hasName = 1,
			enabled = 1,
		};
		//! Text Color for displaying the verbosity
		static const System::Console::Color color = System::Console::none;
		//! Message Text Color
		static const System::Console::Color messageColor = System::Console::none;

	}; // class Info



	struct YUNI_DECL Compatibility final
	{
		static const char* Name() {return "Compatibility";}
		template<class U> static void AppendName(U& u) {u << "[compatibility notice]";}

		enum
		{
			level = 8000,
			shouldUsesStdCerr = 0,
			hasName = 1,
			enabled = 1,
		};
		//! Text Color for displaying the verbosity
		static const System::Console::Color color = System::Console::yellow;
		//! Message Text Color
		static const System::Console::Color messageColor = System::Console::none;

	}; // class Info



	struct YUNI_DECL Debug final
	{
		static const char* Name() {return "debug";}
		template<class U> static void AppendName(U& u) {u << "[debug]";}

		enum
		{
			level = 10000,
			shouldUsesStdCerr = 0,
			hasName = 1,
			# ifdef NDEBUG // The debug messages must be disabled
			enabled = 0,
			# else
			enabled = 1,
			# endif
		};
		//! Text Color for displaying the verbosity
		static const System::Console::Color color = System::Console::none;
		//! Message Text Color
		static const System::Console::Color messageColor = System::Console::none;

	}; // class Debug





} // namespace Verbosity
} // namespace Logs
} // namespace Yuni

