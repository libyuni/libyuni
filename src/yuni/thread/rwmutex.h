#ifndef __YUNI_THREADS_RW_MUTEX_H__
# define __YUNI_THREADS_RW_MUTEX_H__

# include "mutex.h"
# include "semaphore.h"



namespace Yuni
{

	/*!
	** \brief Read/Write Mutex (allows several readers but one writer)
	**
	** \ingroup Threads
	*/
	class YUNI_DECL RWMutex final
	{
	public:
		/*!
		** \brief A class-level locking mechanism
		**
		** A class-level locking operation locks all objects in a given class during that operation
		*/
		template<class T>
		class ClassLevelLockable
		{
		public:
			//! A dedicated mutex for the class T
			static RWMutex mutex;

		}; // class ClassLevelLockable


	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default constructor
		*/
		RWMutex(uint maxReaders = 64); // arbitrary limit
		/*!
		** \brief Copy constructor
		**
		** This constructor does actually nothing but it allows the compilation
		** of other classes which would implement a copy constructor
		*/
		RWMutex(const RWMutex&);
		/*!
		** \brief Destructor
		*/
		~RWMutex();
		//@}


		//! \name Read Lock & Unlock
		//@{
		/*!
		** \brief Lock the mutex for read access
		*/
		void readLock();

		/*!
		** \brief Release the lock for read access
		*/
		void readUnlock();
		//@}


		//! \name Write Lock & Unlock
		//@{
		/*!
		** \brief Lock the mutex for write access
		*/
		void writeLock();

		/*!
		** \brief Release the lock for write access
		*/
		void writeUnlock();
		//@}


		//! \name Operators
		//@{
		//! Operator = (do nothing)
		RWMutex& operator = (const RWMutex&);
		//@}


	private:
		# ifndef YUNI_NO_THREAD_SAFE
		//! Semaphore
		Semaphore pSemaphore;
		//! Real mutex
		Mutex pMutex;
		# endif

	}; // class Mutex




	/*!
	** \brief Locks a mutex for read access in the constructor and unlocks it in the destructor (RAII).
	*/
	class ReadMutexLocker final
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Constructor
		**
		** \param m The mutex to lock
		*/
		ReadMutexLocker(RWMutex& m);
		//! Destructor
		~ReadMutexLocker();
		//@}

	private:
		//! Reference to the real mutex
		RWMutex& pMutex;

	}; // ReadMutexLocker


	/*!
	** \brief Locks a mutex for write access in the constructor and unlocks it in the destructor (RAII).
	*/
	class WriteMutexLocker final
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Constructor
		**
		** \param m The mutex to lock
		*/
		WriteMutexLocker(RWMutex& m);
		//! Destructor
		~WriteMutexLocker();
		//@}

	private:
		//! Reference to the real mutex
		RWMutex& pMutex;

	}; // WriteMutexLocker




	//! All mutexes for each class
	template<class T> RWMutex RWMutex::ClassLevelLockable<T>::mutex;




} // namespace Yuni

# include "rwmutex.hxx"

#endif // __YUNI_THREADS_RW_MUTEX_H__
