#ifndef __YUNI_THREADS_SEMAPHORE_H__
# define __YUNI_THREADS_SEMAPHORE_H__

# include "../yuni.h"
# ifndef YUNI_NO_THREAD_SAFE
#	if !defined(YUNI_OS_WINDOWS) && !defined(YUNI_OS_MAC)
#		include <semaphore.h> // unix / sem_t
#	endif
# endif




namespace Yuni
{

	/*!
	** \brief Semaphore, protect access to shared data with concurrent read
	**
	** \ingroup Threads
	*/
	class YUNI_DECL Semaphore final
	{
	public:
		/*!
		** \brief Default constructor
		*/
		explicit Semaphore(uint readers = 64);
		/*!
		** \brief Copy constructor
		*/
		Semaphore(const Semaphore&);
		//! Destructor
		~Semaphore();
		//@}


		//! \name Acquire / release
		//@{
		/*!
		** \brief Acquire 1 resource
		**
		** If there aren't that many resources available, the call will block until this is the case.
		*/
		void acquire();
		/*!
		** \brief Acquire n resources
		**
		** If there aren't that many resources available, the call will block until this is the case.
		*/
		void acquire(uint n);

		/*!
		** \brief Release 1 resource
		*/
		void release();
		/*!
		** \brief Release n resources
		*/
		void release(uint n);
		//@}


		//! \name Informations
		//@{
		//! Get the maximum number of concurrent readers
		uint maxReaders() const;
		//@}


		//! \name Operators
		//@{
		//! Try to acquire a resource
		Semaphore& operator -- ();
		//! Release a resource
		Semaphore& operator ++ ();
		//! Copy operator
		Semaphore& operator = (const Semaphore&);
		//@}


	private:
		//! Initialize the internal semaphore
		inline void initialize(uint readers);
		//! Release the internal semaphore
		inline void finalize();

	private:
		# ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		void* pSemaphore;
		# else
		//! Pthread semaphore
		#	ifdef YUNI_OS_MAC
		void* pSemaphore;
		#	else
		::sem_t pSemaphore; // not allocated by new
		#	endif
		# endif
		# endif
		uint pMaxReaders;

	}; // class Semaphore





} // namespace Yuni

# include "semaphore.hxx"

#endif // __YUNI_THREADS_SEMAPHORE_H__
