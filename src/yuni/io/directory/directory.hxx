#pragma once
#include "directory.h"
#include "../../core/traits/cstring.h"
#include "../../core/traits/length.h"
#include "../../core/static/remove.h"
#include "../constants.h"
#ifdef YUNI_HAS_STDLIB_H
#	include <stdlib.h>
#endif




namespace Yuni
{
namespace Private
{
namespace IO
{
namespace Directory
{

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
		return (IO::typeFolder == IO::TypeOf(path));
	}


	inline bool Copy(const AnyString& source, const AnyString& destination, bool recursive, bool overwrite)
	{
		CopyOnUpdateBind e;
		e.bind(&Private::IO::Directory::DummyCopyUpdateEvent);
		return Copy(source, destination, recursive, overwrite, e);
	}


	inline bool Copy(const AnyString& source, const AnyString& destination, const CopyOnUpdateBind& onUpdate)
	{
		return Copy(source, destination, true, true, onUpdate);
	}




} // namespace Directory
} // namespace IO
} // namespace Yuni
