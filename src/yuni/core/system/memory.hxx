#pragma once
#include "memory.h"



namespace Yuni
{
namespace System
{
namespace Memory
{


	inline Usage::Usage()
	{
		update();
	}


	inline Usage::Usage(const Usage& copy)
		: available(copy.available), total(copy.total)
	{}



} // namespace Memory
} // namespace System
} // namespace Yuni

