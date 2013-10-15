#ifndef __YUNI_CORE_SINGLETON_SINGLETON_H__
# define __YUNI_CORE_SINGLETON_SINGLETON_H__

# include "../../yuni.h"
# include "../../thread/policy.h"
# include "../noncopyable.h"
# include "policies/creation.h"
# include "policies/lifetime.h"


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

# include "singleton.hxx"

#endif // __YUNI_CORE_SINGLETON_SINGLETON_H__
