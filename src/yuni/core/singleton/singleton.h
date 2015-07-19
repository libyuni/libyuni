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
#include "../../yuni.h"
#include "../../thread/policy.h"
#include "../noncopyable.h"
#include "policies/creation.h"
#include "policies/lifetime.h"


namespace Yuni
{

	/*!
	** \brief Holder for a singleton class
	**
	** Manages creation, deletion and access in a MT environment
	**
	** For protection, some operations will fail at compile time.
	** For example, supposing MySingleton inherits Singleton<> :
	** \code
	** int main(void)
	** {
	**     MySingleton& instance = MySingleton::Instance();
	**     // Here we viciously try to delete the instance
	**     delete &instance;
	**     return 0;
	** }
	** \endcode
	*/
	template <class T,
		template <class> class CreationT = Policy::Creation::EmptyConstructor,
		template <class> class LifetimeT = Policy::Lifetime::Normal,
		template <class> class ThreadingT = Policy::ClassLevelLockable>
	class Singleton :
		public ThreadingT<Singleton<T, CreationT, LifetimeT, ThreadingT> >,
		private NonCopyable<Singleton<T, CreationT, LifetimeT, ThreadingT> >
	{
	public:
		//! Stored singleton type
		typedef T StoredType;
		//! Creation policy
		typedef CreationT<T> CreationPolicy;
		//! Lifetime policy
		typedef LifetimeT<T> LifetimePolicy;
		//! Threading policy
		typedef ThreadingT<Singleton<T, CreationT, LifetimeT, ThreadingT> > ThreadingPolicy;
		//! Type as stored in the singleton (volatile if necessary)
		typedef T& Reference;
		//! Volatile pointer
		typedef typename ThreadingPolicy::template Volatile<T*>::Type VolatilePtr;

	public:
		/*!
		** \brief Get the instance of this singleton
		*/
		static Reference Instance();

	public:
		/*!
		** \brief Address-of operator will fail at link time
		*/
		Singleton<T, CreationT, LifetimeT, ThreadingT>* operator & ();

		/*!
		** \brief Address-of operator will fail at link time
		*/
		const Singleton* operator & () const;


	protected:
		/*!
		** \brief Protected constructor !
		*/
		Singleton();

	private:
		//! Destroy the stored instance
		static void DestroyInstance();

	private:
		//! Unique instance of the class
		static VolatilePtr pInstance;
		//! Has the instance already been destroyed once ?
		static bool pDestroyed;

	}; // class Singleton






} // namespace Yuni

#include "singleton.hxx"
