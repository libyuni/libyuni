#pragma once
#include "taskgroup.h"




namespace Yuni
{
namespace Job
{


	inline void Taskgroup::add(const Bind<bool (IJob&)>& callback)
	{
		add(pDefaultQueueservice, callback);
	}


	inline Taskgroup& Taskgroup::operator += (const Bind<bool (IJob&)>& callback)
	{
		add(pDefaultQueueservice, callback);
		return *this;
	}





} // namespace Job
} // namespace Yuni
