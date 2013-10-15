#ifndef __YUNI_IO_DIRECTORY_SYSTEM_HXX__
# define __YUNI_IO_DIRECTORY_SYSTEM_HXX__

# include "../../core/system/environment.h"


namespace Yuni
{
namespace IO
{
namespace Directory
{
namespace System
{


	template<class StringT>
	bool Temporary(StringT& out, bool emptyBefore)
	{
		if (emptyBefore)
			out.clear();
		# ifdef YUNI_OS_WINDOWS
		if (!Yuni::System::Environment::Read("TEMP", out, false))
		{
			if (!Yuni::System::Environment::Read("TMP", out, false))
				return false;
		}
		# else
		// On UNIXes, the environment variable TMPDIR must be checked
		// first. Unfortunately, It may happen that no env variable is available.
		if (!Yuni::System::Environment::Read("TMPDIR", out, false))
		{
			if (!Yuni::System::Environment::Read("TMP", out, false))
			{
				if (!Yuni::System::Environment::Read("TEMP", out, false))
					out += "/tmp";
			}
		}
		# endif

		return true;
	}



	template<class StringT>
	bool UserHome(StringT& out, bool emptyBefore)
	{
		if (emptyBefore)
			out.clear();

		# ifdef YUNI_OS_WINDOWS
		if (!Yuni::System::Environment::Read("HOMEDRIVE", out, false))
			out += "C:"; // C by default
		if (!Yuni::System::Environment::Read("HOMEPATH", out, false))
			out += '\\';
		return true;
		# else
		return Yuni::System::Environment::Read("HOME", out, false);
		# endif
	}






} // namespace System
} // namespace Directory
} // namespace IO
} // namespace Yuni

# include "system.hxx"

#endif // __YUNI_IO_DIRECTORY_HXX__
