#ifndef __YUNI_THREADS_RW_MUTEX_HXX__
# define __YUNI_THREADS_RW_MUTEX_HXX__


namespace Yuni
{

	inline RWMutex::RWMutex(uint maxReaders) :
		pSemaphore(maxReaders)
	{
	}


	inline RWMutex::~RWMutex()
	{
	}


	inline RWMutex::RWMutex(const RWMutex& copy) :
		pSemaphore(copy.pSemaphore)
	{
	}


	inline RWMutex& RWMutex::operator = (const RWMutex&)
	{
		// Do nothing on purpose
		return *this;
	}


	inline void RWMutex::readLock()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		pSemaphore.acquire();
		# endif
	}


	inline void RWMutex::readUnlock()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		pSemaphore.release();
		# endif
	}





	inline ReadMutexLocker::ReadMutexLocker(RWMutex& m) :
		pMutex(m)
	{
		m.readLock();
	}


	inline ReadMutexLocker::~ReadMutexLocker()
	{
		pMutex.readUnlock();
	}


	inline WriteMutexLocker::WriteMutexLocker(RWMutex& m) :
		pMutex(m)
	{
		m.writeLock();
	}


	inline WriteMutexLocker::~WriteMutexLocker()
	{
		pMutex.writeUnlock();
	}




} // namespace Yuni

#endif // __YUNI_THREADS_RW_MUTEX_HXX__
