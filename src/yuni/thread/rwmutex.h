/*
** YUNI's default license is the GNU Lesser Public License (LGPL), with some
** exclusions (see below). This basically means that you can get the full source
** code for nothing, so long as you adhere to a few rules.
**
** Under the LGPL you may use YUNI for any purpose you wish, and modify it if you
** require, as long as you:
**
** Pass on the (modified) YUNI source code with your software, with original
** copyrights intact :
**  * If you distribute electronically, the source can be a separate download
**    (either from your own site if you modified YUNI, or to the official YUNI
**    website if you used an unmodified version) – just include a link in your
**    documentation
**  * If you distribute physical media, the YUNI source that you used to build
**    your application should be included on that media
** Make it clear where you have customised it.
**
** In addition to the LGPL license text, the following exceptions / clarifications
** to the LGPL conditions apply to YUNI:
**
**  * Making modifications to YUNI configuration files, build scripts and
**    configuration headers such as yuni/platform.h in order to create a
**    customised build setup of YUNI with the otherwise unmodified source code,
**    does not constitute a derived work
**  * Building against YUNI headers which have inlined code does not constitute a
**    derived work
**  * Code which subclasses YUNI classes outside of the YUNI libraries does not
**    form a derived work
**  * Statically linking the YUNI libraries into a user application does not make
**    the user application a derived work.
**  * Using source code obsfucation on the YUNI source code when distributing it
**    is not permitted.
** As per the terms of the LGPL, a "derived work" is one for which you have to
** distribute source code for, so when the clauses above define something as not
** a derived work, it means you don't have to distribute source code for it.
** However, the original YUNI source code with all modifications must always be
** made available.
*/
#pragma once
#include "mutex.h"
#include "semaphore.h"




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

	}; // class RWMutex




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

