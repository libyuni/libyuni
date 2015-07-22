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
#include "system.h"
#include "../../core/system/environment.h"




namespace Yuni
{
namespace IO
{
namespace Directory
{
namespace System
{

	namespace // anonymous
	{

		template<class StringT>
		static inline bool TemporaryImpl(StringT& out, bool emptyBefore)
		{
			if (emptyBefore)
				out.clear();

			#if defined(YUNI_OS_WINDOWS)
			{
				if (not Yuni::System::Environment::Read("TEMP", out, false))
				{
					if (not Yuni::System::Environment::Read("TMP", out, false))
						return false;
				}
			}
			#else
			{
				// On UNIXes, the environment variable TMPDIR must be checked
				// first. Unfortunately, It may happen that no env variable is available.
				if (not Yuni::System::Environment::Read("TMPDIR", out, false))
				{
					if (not Yuni::System::Environment::Read("TMP", out, false))
					{
						if (not Yuni::System::Environment::Read("TEMP", out, false))
							out += "/tmp"; // default
					}
				}
			}
			#endif

			return true;
		}



		template<class StringT>
		static inline bool UserHomeImpl(StringT& out, bool emptyBefore)
		{
			if (emptyBefore)
				out.clear();

			#ifdef YUNI_OS_WINDOWS
			{
				if (not Yuni::System::Environment::Read("HOMEDRIVE", out, false))
					out += "C:"; // C by default
				if (not Yuni::System::Environment::Read("HOMEPATH", out, false))
					out += '\\';
				return true;
			}
			#else
			{
				// UNIX
				return Yuni::System::Environment::Read("HOME", out, false);
			}
			#endif

			return false; // fallback
		}



		template<class StringT>
		static inline bool FontsImpl(StringT& out, bool emptyBefore)
		{
			if (emptyBefore)
				out.clear();

			#ifdef YUNI_OS_WINDOWS
			{
				if (not Yuni::System::Environment::Read("WINDIR", out, false))
					out += "C:\\Windows"; // C:\Windows by default
				out += "\\Fonts\\";
			}
			#elif defined(YUNI_OS_MACOS)
			{
				out = "/Library/Fonts/";
			}
			#else // YUNI_OS_LINUX
			{
				out = "/usr/share/fonts/truetype/";
			}
			#endif

			return true;
		}


	} // anonymous namespace








	bool Temporary(String& out, bool emptyBefore)
	{
		return TemporaryImpl(out, emptyBefore);
	}


	bool Temporary(Clob& out, bool emptyBefore)
	{
		return TemporaryImpl(out, emptyBefore);
	}


	bool UserHome(Clob& out, bool emptyBefore)
	{
		return UserHomeImpl(out, emptyBefore);
	}


	bool UserHome(String& out, bool emptyBefore)
	{
		return UserHomeImpl(out, emptyBefore);
	}


	bool Fonts(String& out, bool emptyBefore)
	{
		return FontsImpl(out, emptyBefore);
	}


	bool Fonts(Clob& out, bool emptyBefore)
	{
		return FontsImpl(out, emptyBefore);
	}




} // namespace System
} // namespace Directory
} // namespace IO
} // namespace Yuni
