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

/*!
** \file
** \brief Threading policies
*/

# include "../yuni.h"
# ifdef YUNI_OS_WINDOWS
#	include "../core/system/windows.hdr.h"
# endif
# include "mutex.h"



namespace Yuni
{
namespace Policy
{

	/*!
	** \name Threading policies
	** \ingroup Policies
	*/
	//@{

	namespace // anonymous
	{

		/*!
		** \brief Helper class for retrieving the reference to the mutex in any situations
		*/
		template<class T>
		class MutexExtractor final
		{
		public:
			static Mutex& Reference(const T& rhs)
			{
				return const_cast<Mutex&>(rhs.pMutex);
			}
		};

		template<>
		class MutexExtractor<Yuni::Mutex> final
		{
		public:
			static Mutex& Reference(const Mutex& rhs)
			{
				return const_cast<Mutex&>(rhs);
			}
		};

	} // anonymous namespace





	/*!
	** \brief Implementation of the Threading Model policy in a single-threaded environnement
	** \ingroup Policies
	**
	** No synchronization is required in this case
	*/
	template<class Host>
	class SingleThreaded
	{
	public:
		//! The threading policy for another class
		template<class T> struct Class { typedef SingleThreaded<T>  Type; };

	public:
		/*!
		** \brief Locks a mutex in the constructor and unlocks it in the destructor.
		*/
		class MutexLocker final
		{
		public:
			MutexLocker() {}
			template<class C> MutexLocker(const C&) {}
			~MutexLocker() {}
		}; // class MutexLocker


		/*!
		** \brief A volative type
		*/
		template<typename U> struct Volatile { typedef U Type; };

		//! Get if this policy is thread-safe
		enum { threadSafe = false };

	public:
		//! \name Constructor & Destructor
		//@{
		//! Default constructor
		SingleThreaded() {}
		//! Copy constructor
		SingleThreaded(const SingleThreaded&) {}
		//! Destructor
		~SingleThreaded() {}
		//@}

	}; // class SingleThreaded



	/*!
	** \brief Implementation of the Threading Model policy in a multi-threaded environnement (one recursive mutex per object)
	**
	** The inner mutex is recursive.
	** \ingroup Policies
	*/
	template<class Host>
	class ObjectLevelLockable
	{
	public:
		//! The threading policy for another class
		template<class T> struct Class { typedef ObjectLevelLockable<T>  Type; };

	public:
		/*!
		** \brief Locks a mutex in the constructor and unlocks it in the destructor.
		*/
		class MutexLocker final
		{
		public:
			template<class C> MutexLocker(const C& h) :
				pHostToLock(MutexExtractor<C>::Reference(h))
			{
				pHostToLock.lock();
			}

			~MutexLocker()
			{
				pHostToLock.unlock();
			}

		private:
			Mutex& pHostToLock;
		}; // class MutexLocker

		/*!
		** \brief A volative type
		*/
		template<typename U> struct Volatile { typedef volatile U Type; };

		//! Get if this policy is thread-safe
		enum { threadSafe = true };

	public:
		//! \name Constructor & Destructor
		//@{
		//! Default constructor
		ObjectLevelLockable() : pMutex(true /*recursive*/) {}
		//! Copy constructor
		ObjectLevelLockable(const ObjectLevelLockable&) :pMutex(true /*recursive*/) {}
		//! Destructor
		~ObjectLevelLockable() {}
		//@}

	protected:
		//! Mutex
		mutable Mutex pMutex;
		template<class> friend class MutexExtractor;

	}; // class ObjectLevelLockable



	/*!
	** \brief Implementation of the Threading Model policy in a multi-threaded environnement (one mutex per object, not recursive)
	** \ingroup Policies
	*/
	template<class Host>
	class ObjectLevelLockableNotRecursive
	{
	public:
		//! The threading policy for another class
		template<class T> struct Class { typedef ObjectLevelLockableNotRecursive<T>  Type; };

	public:
		/*!
		** \brief Locks a mutex in the constructor and unlocks it in the destructor.
		*/
		class MutexLocker final
		{
		public:
			template<class C> MutexLocker(const C& h) :
				pHostToLock(MutexExtractor<C>::Reference(h))
			{
				pHostToLock.lock();
			}

			~MutexLocker()
			{
				pHostToLock.unlock();
			}

		private:
			Mutex& pHostToLock;
		}; // class MutexLocker

		/*!
		** \brief A volative type
		*/
		template<typename U> struct Volatile { typedef volatile U Type; };

		//! Get if this policy is thread-safe
		enum { threadSafe = true };

	public:
		//! \name Constructor & Destructor
		//@{
		//! Default constructor
		ObjectLevelLockableNotRecursive() : pMutex(false) {}
		//! Copy constructor
		ObjectLevelLockableNotRecursive(const ObjectLevelLockableNotRecursive&) : pMutex(false) {}
		//! Destructor
		~ObjectLevelLockableNotRecursive() {}
		//@}

	protected:
		//! Mutex
		mutable Mutex pMutex;
		template<class> friend class MutexExtractor;

	}; // class ObjectLevelLockableNotRecursive




	/*!
	** \brief Implementation of the Threading Model policy in a multi-threaded environnement (one mutex per class)
	** \ingroup Policies
	*/
	template<class Host>
	class ClassLevelLockable
	{
	public:
		//! The threading policy for another class
		template<class T> struct Class { typedef ClassLevelLockable<T>  Type; };

	public:
		/*!
		** \brief Locks a mutex in the constructor and unlocks it in the destructor.
		*/
		class MutexLocker final
		{
		public:
			MutexLocker()
			{
				Mutex::ClassLevelLockable<Host>::mutex.lock();
			}
			MutexLocker(const MutexLocker&) {}
			template<class C> MutexLocker(const C&)
			{
				Mutex::ClassLevelLockable<Host>::mutex.lock();
			}
			~MutexLocker()
			{
				Mutex::ClassLevelLockable<Host>::mutex.unlock();
			}
		}; // class MutexLocker

		/*!
		** \brief A volative type
		*/
		template<typename U> struct Volatile { typedef volatile U Type; };

		//! Get if this policy is thread-safe
		enum { threadSafe = true };

	public:
		//! \name Constructor & Destructor
		//@{
		//! Default constructor
		ClassLevelLockable() :pMutex(Mutex::ClassLevelLockable<Host>::mutex) {}
		//! Copy constructor
		ClassLevelLockable(const ClassLevelLockable&) :pMutex(Mutex::ClassLevelLockable<Host>::mutex) {}
		//! Destructor
		~ClassLevelLockable() {}
		//@}

	private:
		//! Mutex
		Mutex& pMutex;

	}; // class ClassLevelLockable



	//@} // Threading Policies




} // namespace Policy
} // namespace Yuni

