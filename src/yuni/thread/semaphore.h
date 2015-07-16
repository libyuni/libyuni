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
#include "../yuni.h"
#ifndef YUNI_NO_THREAD_SAFE
# if !defined(YUNI_OS_WINDOWS) && !defined(YUNI_OS_MAC)
#	include <semaphore.h> // unix / sem_t
# endif
#endif




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

