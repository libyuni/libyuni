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
#include "thread.h"
#include "../core/static/inherit.h"
#include <vector>



namespace Yuni
{
namespace Thread
{

	/*!
	** \brief Container for thread
	**
	** This class is a convenient container for dealing with several threads
	** at the same time.
	**
	** Performance Tip: If the threads within this container are destined to die,
	**   you should use the method stop() before destroying this class. This method
	**   will make them stop gracefully and asynchronously and you should earn a
	**   lot of time.
	*/
	template<class T>
	class YUNI_DECL Array : public Policy::ObjectLevelLockableNotRecursive<Array<T> >
	{
	public:
		//! Itself
		typedef Array<T>  ArrayType;
		//! The most suitable smart pointer
		typedef SmartPtr<ArrayType> Ptr;
		//! Thread
		typedef T ThreadType;
		//! Smart Pointer for the thread
		typedef typename T::Ptr ThreadPtr;
		//! Threading Policy
		typedef Policy::ObjectLevelLockableNotRecursive<ArrayType>  ThreadingPolicy;

		// Checking if T is a descendant of IThread
		YUNI_STATIC_ASSERT((Static::InheritsFrom<T, IThread>::Yes), ThreadArray_T_MustInheritFrom_IThread);
		//! Thread list
		typedef std::vector<typename T::Ptr> ThreadList;
		enum
		{
			//! A hard limit value for the number of threads in the pool
			maxThreadsLimit = 1024,
		};

	public:
		//! \name Constructor
		//@{
		/*!
		** \brief Default Constructor
		*/
		Array();

		/*!
		** \brief Copy constructor
		*/
		Array(const Array& rhs);

		/*!
		** \brief Constructor with a given amount of thread
		*/
		explicit Array(uint n);

		/*!
		** \brief Constructor, with a given amount of thread
		*/
		Array(uint n, bool autoStart);

		//! Destructor
		~Array();
		//@}


		//! \name Settings
		//@{
		//! Get if the new thread are automatically started
		bool autoStart() const;
		//! Set if the new thread are automatically started
		void autoStart(const bool v);
		//@}


		/*!
		** \brief Clear the pool (remove all threads)
		**
		** Performance Tip: If the threads within this container are destined to die,
		**   you should use the method stop() before destroying this class. This method
		**   will make them stop gracefully and asynchronously and you should earn a
		**   lot of time.
		**
		** \warning This method may block until all removed threads are destroyed
		**   (and stopped) if they are not used any longer.
		*/
		void clear();

		/*!
		** \brief Add an existing thread into the container
		**
		** The thread will be automatically started if autoStart() is true.
		*/
		void add(typename T::Ptr thread);

		/*!
		** \brief Add an existing thread into the container
		**
		** \param thread The thread to add
		** \param autostart True to start the thread before adding it, false to do
		**   nothing (whatever the value of autoStart())
		*/
		void add(typename T::Ptr thread, bool autostart);

		//! \see add(typename T::Ptr)
		void push_back(typename T::Ptr thread);

		/*!
		** \brief Resize the pool
		**
		** This method will have no effect if the new number of threads
		** is equal to the number of threads currently in the pool.
		** If the new value is greater than the current one, some new threads will be created,
		** otherwise the remaining threads will be removed from the pool.
		** A thread that has been removed is not necessary stopped neither destroyed,
		** since all threads in the pool are managed by a smart pointer.
		**
		** \warning This method may block until all removed threads are destroyed
		** (and stopped) if they are not used any longer.
		**
		** \param n The new number of threads in the pool
		*/
		void resize(uint n);


		/*!
		** \brief Start all threads
		*/
		void start();

		/*!
		** \brief Ask to all threads to stop their execution as soon as possible
		*/
		void gracefulStop();

		/*!
		** \brief Stop all threads
		**
		** \warning This method may block until all removed threads are destroyed
		**   (and stopped) if they are not used any longer.
		** \param timeout A timeout for waiting a thread to stop (in milliseconds)
		*/
		void stop(uint timeout = defaultTimeout);

		/*!
		** \brief Restart all threads
		**
		** \warning This method may block until all removed threads are destroyed
		**   (and stopped) if they are not used any longer.
		** \param timeout A timeout for waiting a thread to stop (in milliseconds)
		*/
		void restart(uint timeout = defaultTimeout);

		/*!
		** \brief Wake up all threads
		**
		** This action will have no effect for all threads not already started.
		*/
		void wakeUp();

		/*!
		** \brief Wait for all threads to finish
		*/
		void wait();

		/*!
		** \brief Wait for all threads to finish
		**
		** \warning The current implementation is naive and wait for each thread N milliseconds
		** \param milliseconds A timeout in milliseconds
		*/
		void wait(uint milliseconds);


		/*!
		** \brief Get the number of threads
		*/
		uint size() const;

		//! Get if the array is empty
		bool empty() const;

		/*!
		** \brief Execute a predicate for each thread in pool
		**
		** The following prototype will be used :
		** \code
		** bool operator () (const typename T::Ptr& thread);
		** \endcode
		*/
		template<class PredicateT> void foreachThread(const PredicateT& predicate) const;

		/*!
		** \brief Execute a predicate for each thread in pool
		**
		** The following prototype will be used :
		** \code
		** bool operator () (typename T::Ptr& thread);
		** \endcode
		*/
		template<class PredicateT> void foreachThread(const PredicateT& predicate);


		//! \name Operators
		//@{
		/*!
		** \brief Get the pointer to a specific thread
		**
		** \param index A zero-based index of the thread (invalid values accepted)
		** \return A pointer to the thread (can be NULL if the index is invalid)
		*/
		typename T::Ptr operator [] (uint index) const;

		//! Copy operator
		Array& operator = (const Array& rhs);
		//! Copy operator
		Array& operator = (const Ptr& rhs);
		//! Aggregate
		Array& operator += (const Array& rhs);
		//! Aggregate
		Array& operator += (T* rhs);
		//! Aggregate
		Array& operator += (const typename T::Ptr& rhs);
		//! Aggregate
		Array& operator += (const Ptr& rhs);
		//! Aggregate
		Array& operator << (const Array& rhs);
		//! Aggregate
		Array& operator << (const Ptr& rhs);
		//! Aggregate
		Array& operator << (T* rhs);
		//! Aggregate
		Array& operator << (const typename T::Ptr& rhs);
		//! Aggregate
		//@}


	private:
		/*!
		** \brief Append N threads to the list
		** \internal We use a temporary variable `autostart` to avoid the
		**   use of the atomic one `pAutoStart` when possible and to
		**   avoid changes while adding new threads.
		*/
		void appendNThreadsWL(uint n, bool autostart);

	private:
		# ifdef YUNI_OS_MSVC
		// export instanciation of STL class
		class YUNI_DECL ThreadList; // see http://support.microsoft.com/kb/168958/en-us/
		# endif

		//! The list of threads in the pool
		ThreadList pList;
		//! Auto start thread (when created)
		typename ThreadingPolicy::template Volatile<bool>::Type pAutoStart;

	}; // class Array<T>






} // namespace Thread
} // namespace Yuni

# include "array.hxx"

