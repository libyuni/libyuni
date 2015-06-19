#pragma once
#include "symbol.h"




namespace Yuni
{
namespace DynamicLibrary
{


	inline Symbol::Symbol()
		: pPtr(NULL)
	{}


	inline Symbol::Symbol(Symbol::Handle p)
		: pPtr(p)
	{}


	inline Symbol::Symbol(const Symbol& copy)
		: pPtr(copy.pPtr)
	{}


	inline bool Symbol::null() const
	{
		return (NULL == pPtr);
	}


	inline bool Symbol::valid() const
	{
		return (NULL != pPtr);
	}


	inline Symbol& Symbol::operator = (const Symbol& rhs)
	{
		pPtr = rhs.pPtr;
		return *this;
	}


	inline Symbol& Symbol::operator = (Symbol::Handle hndl)
	{
		pPtr = hndl;
		return *this;
	}


	inline Symbol::Handle Symbol::ptr() const
	{
		return pPtr;
	}





} // namespace DynamicLibrary
} // namespace Yuni
