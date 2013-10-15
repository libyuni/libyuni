#ifndef __YUNI_IO_DIRECTORY_HXX__
# define __YUNI_IO_DIRECTORY_HXX__

# include "../../core/traits/cstring.h"
# include "../../core/traits/length.h"
# include "../../core/static/remove.h"
# include "../constants.h"
# ifdef YUNI_HAS_STDLIB_H
#	include <stdlib.h>
# endif


namespace Yuni
{
namespace Private
{
namespace IO
{
namespace Directory
{

	/*!
	** \brief Get the current directory which must be freed
	*/
	char* CurrentDirectory();


	inline bool DummyCopyUpdateEvent(Yuni::IO::Directory::CopyState, const String&, const String&, uint64, uint64)
	{
		return true;
	}



} // namespace Directory
} // namespace IO
} // namespace Private
} // namespace Yuni



namespace Yuni
{
namespace IO
{
namespace Directory
{


	inline bool Exists(const AnyString& path)
	{
		return ((IO::typeFolder & IO::TypeOf(path)) != 0);
	}


	inline bool Copy(const AnyString& source, const AnyString& destination, bool recursive, bool overwrite)
	{
		CopyOnUpdateBind e;
		e.bind(&Private::IO::Directory::DummyCopyUpdateEvent);
		return Copy(source, destination, recursive, overwrite, e);
	}




} // namespace Directory
} // namespace IO
} // namespace Yuni





namespace Yuni
{
namespace IO
{
namespace Directory
{
namespace Current
{

	template<class StringT>
	inline void Get(StringT& out, bool clearBefore)
	{
		char* c = Yuni::Private::IO::Directory::CurrentDirectory();
		if (c)
		{
			if (clearBefore)
				out = c;
			else
				out += c;
			::free(c);
		}
		else
		{
			if (clearBefore)
				out.clear();
		}
	}




} // namespace Current
} // namespace Directory
} // namespace IO
} // namespace Yuni

#endif // __YUNI_IO_DIRECTORY_H__
