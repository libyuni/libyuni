
#include "rwmutex.h"


namespace Yuni
{


	void RWMutex::writeLock()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		pMutex.lock();
		pSemaphore.acquire(pSemaphore.maxReaders());
		pMutex.unlock();
		# endif
	}


	void RWMutex::writeUnlock()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		pMutex.lock();
		pSemaphore.release(pSemaphore.maxReaders());
		pMutex.unlock();
		# endif
	}




} // namespace Yuni

