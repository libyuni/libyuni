#pragma once
#include "signal.h"



namespace Yuni
{
namespace Thread
{

	inline bool Signal::valid() const
	{
		#ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		return (pHandle != NULL);
		# else
		return true;
		# endif
		# else
		return true;
		# endif
	}


	inline bool Signal::operator ! () const
	{
		return not valid();
	}


	inline Signal& Signal::operator = (const Signal&)
	{
		return *this;
	}





} // namespace Thread
} // namespace Yuni

