#ifndef __YUNI_THREADS_SEMAPHORE_HXX__
# define __YUNI_THREADS_SEMAPHORE_HXX__


namespace Yuni
{

	inline uint Semaphore::maxReaders() const
	{
		return pMaxReaders;
	}


	inline Semaphore& Semaphore::operator -- ()
	{
		acquire();
		return *this;
	}


	inline Semaphore& Semaphore::operator ++ ()
	{
		release();
		return *this;
	}




} // namespace Yuni

#endif // __YUNI_THREADS_SEMAPHORE_HXX__

