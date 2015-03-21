#pragma once



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
