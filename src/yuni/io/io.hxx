#pragma once
#include "io.h"


namespace Yuni
{
namespace IO
{

	inline bool Exists(const AnyString& filename)
	{
		return (Yuni::IO::typeUnknown != Yuni::IO::TypeOf(filename));
	}




} // namespace IO
} // namespace Yuni
