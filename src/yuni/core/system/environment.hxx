#ifndef __YUNI_CORE_SYSTEM_ENVIRONMENT_HXX__
# define __YUNI_CORE_SYSTEM_ENVIRONMENT_HXX__

# include "windows.hdr.h"
# include "../traits/cstring.h"
# ifdef YUNI_HAS_STDLIB_H
#	include <stdlib.h>
# endif


namespace Yuni
{
namespace System
{
namespace Environment
{

	inline String Read(const AnyString& name)
	{
		# ifdef YUNI_HAS_STDLIB_H
		return ::getenv(name.c_str());
		# else
		return nullptr;
		# endif
	}


	template<class StringT>
	inline bool Read(const AnyString& name, StringT& out, bool emptyBefore)
	{
		// Assert, if a C* container can not be found at compile time
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, SystemEnvironment_InvalidTypeForString);

		if (emptyBefore)
			out.clear();

		# ifdef YUNI_HAS_STDLIB_H
		const char* e = ::getenv(name.c_str());
		if (e and '\0' != *e)
		{
			out += e;
			return true;
		}
		return false;

		# else
		return false;
		# endif
	}




} // namespace Environment
} // namespace System
} // namespace Yuni

#endif // __YUNI_CORE_SYSTEM_ENVIRONMENT_HXX__
