/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** github: https://github.com/libyuni/libyuni/
** gitlab: https://gitlab.com/libyuni/libyuni/ (mirror)
*/
#pragma once
#include "array.h"



namespace Yuni
{
namespace Thread
{

	template<class T>
	Array<T>::Array(const Array<T>& rhs)
	{
		typename ThreadingPolicy::MutexLocker locker(rhs);
		pAutoStart = rhs.pAutoStart;
		pList = rhs.pList;
	}


	template<class T>
	inline Array<T>::Array(uint n)
	{
		if (n > maxThreadsLimit)
			n = maxThreadsLimit;
		appendNThreadsWL(n, false);
	}


	template<class T>
	inline Array<T>::Array(uint n, bool autoStart)
		: pAutoStart{autoStart}
	{
		if (n > maxThreadsLimit)
			n = maxThreadsLimit;
		appendNThreadsWL(n, autoStart);
	}


	template<class T>
	inline Array<T>::~Array()
	{
		// We won't stop all remaining threads. They have to do it by themselves
		// when destroyed.
		// however, it would be wise to destroy them before the vtable is corrupted
		clear();
	}


	template<class T>
	inline bool Array<T>::autoStart() const
	{
		return (pAutoStart);
	}


	template<class T>
	inline void Array<T>::autoStart(const bool v)
	{
		pAutoStart  = v;
	}


	template<class T>
	void Array<T>::clear()
	{
		// We will make a copy of the list to release the lock
		// as soon as possible since this routine may take some time...
		ThreadList copy;
		{
			typename ThreadingPolicy::MutexLocker locker(*this);
			if (pList.empty())
				return;
			copy.swap(pList);
		}
		// the container `copy` will be destroyed here, thus all threads
	}


	template<class T>
	void Array<T>::add(typename T::Ptr thread)
	{
		if (pAutoStart)
			thread->start();
		typename ThreadingPolicy::MutexLocker locker(*this);
		pList.push_back(thread);
	}


	template<class T>
	void Array<T>::add(typename T::Ptr thread, bool autostart)
	{
		if (autostart)
			thread->start();
		typename ThreadingPolicy::MutexLocker locker(*this);
		pList.push_back(thread);
	}


	template<class T>
	inline void Array<T>::push_back(typename T::Ptr thread)
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		pList.push_back(thread);
	}


	template<class T>
	void Array<T>::resize(uint n)
	{
		if (0 == n)
		{
			// When resizing to 0 elements, it is exactly equivalent to directly
			// call the method clear(), which should be more efficient
			clear();
			return;
		}
		if (n > maxThreadsLimit)
			n = maxThreadsLimit;
		// If we have some thread to remove from the pool, we will use this copy list
		// since it can take some time
		ThreadList copy;
		{
			// Locking
			typename ThreadingPolicy::MutexLocker locker(*this);
			// Keeping the number of existing thread
			const uint count = pList.size();
			if (count == n)
				return;
			if (count < n)
			{
				// We don't have enough threads in pool. Creating a few of them...
				appendNThreadsWL(n - count, pAutoStart);
				return;
			}
			// Asking to the last threads to stop by themselves as soon as possible
			// This should be done early to make them stop asynchronously.
			// We may earn a lot of time like this.
			for (uint i = n; i < count; ++i)
				pList[i]->gracefulStop();
			// Creating a list of all threads that must be removed
			copy.reserve(count - n);
			for (uint i = n; i < count; ++i)
				copy.push_back(pList[i]);
			// We can resize the vector, the removed threads will be stopped later
			pList.resize(count);
		}
		// all unwanted threads will be stopped (probably destroyed) here
	}


	template<class T>
	void Array<T>::start()
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		// We can start all threads at once while locked because this operation
		// should be fast enough (signal only).
		for (auto& ptr: pList)
			ptr->start();
	}


	template<class T>
	void Array<T>::gracefulStop()
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		// We can ask to all threads to gracefully stop while locked because this operation
		// should be fast enough (signal only).
		for (auto& ptr: pList)
			ptr->gracefulStop();
	}


	template<class T>
	void Array<T>::wait()
	{
		// We will make a copy of the list to release the lock as soon as
		// possible since this routine may take some time...
		ThreadList copy;
		{
			typename ThreadingPolicy::MutexLocker locker(*this);
			if (pList.empty())
				return;
			copy = pList;
		}
		for (auto& ptr: copy)
			ptr->wait();
	}


	template<class T>
	void Array<T>::wait(uint milliseconds)
	{
		// We will make a copy of the list to release the lock as soon as
		// possible since this routine may take some time...
		ThreadList copy;
		{
			typename ThreadingPolicy::MutexLocker locker(*this);
			if (pList.empty())
				return;
			copy = pList;
		}
		for (auto& ptr: copy)
			ptr->wait(milliseconds);
	}


	template<class T>
	void Array<T>::stop(uint timeout)
	{
		// We will make a copy of the list to release the lock as soon as
		// possible since this routine may take some time...
		ThreadList copy;
		{
			typename ThreadingPolicy::MutexLocker locker(*this);
			if (pList.empty())
				return;
			copy = pList;
		}
		// Asking to the last threads to stop by themselves as soon as possible
		// This should be done early to make them stop asynchronously.
		// We may earn a lot of time like this.
		for (auto& ptr: copy)
			ptr->gracefulStop();
		// Now we can kill them if they don't cooperate...
		for (auto& ptr: copy)
			ptr->stop(timeout);
	}


	template<class T>
	void Array<T>::restart(uint timeout)
	{
		// We will make a copy of the list to release the lock as soon as
		// possible since this routine may take some time...
		ThreadList copy;
		{
			typename ThreadingPolicy::MutexLocker locker(*this);
			if (pList.empty())
				return;
			copy = pList;
		}
		// Asking to the last threads to stop by themselves as soon as possible
		// This should be done early to make them stop asynchronously.
		// We may earn a lot of time like this.
		for (auto& ptr: copy)
			ptr->gracefulStop();
		// Now we can kill them if they don't cooperate...
		for (auto& ptr: copy)
			ptr->stop(timeout);
		// And start them again
		for (auto& ptr: copy)
			ptr->start();
	}


	template<class T>
	void Array<T>::wakeUp()
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		// We can wake all threads up at once while locked because this operation
		// should be fast enough (signal only).
		for (auto& ptr: pList)
			ptr->wakeUp();
	}


	template<class T>
	inline typename T::Ptr Array<T>::operator [] (uint index) const
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		return (index < pList.size()) ? pList[index] : T::Ptr();
	}


	template<class T>
	Array<T>& Array<T>::operator = (const Array<T>& rhs)
	{
		typename ThreadingPolicy::MutexLocker lockerR(rhs);
		typename ThreadingPolicy::MutexLocker locker(*this);
		pAutoStart = rhs.pAutoStart;
		pList = rhs.pList;
		return *this;
	}


	template<class T>
	Array<T>& Array<T>::operator = (const Ptr& rhs)
	{
		typename Array<T>::Ptr keepReference = rhs;
		typename ThreadingPolicy::MutexLocker lockerR(*keepReference);
		typename ThreadingPolicy::MutexLocker locker(*this);
		pAutoStart = keepReference.pAutoStart;
		pList = keepReference.pList;
		return *this;
	}


	template<class T>
	Array<T>& Array<T>::operator += (const Array<T>& rhs)
	{
		typename ThreadingPolicy::MutexLocker lockerR(rhs);
		typename ThreadingPolicy::MutexLocker locker(*this);
		for (auto& ptr: rhs.pList)
			pList.push_back(ptr);
		return *this;
	}


	template<class T>
	Array<T>& Array<T>::operator += (const Ptr& rhs)
	{
		typename Array<T>::Ptr keepReference = rhs;
		typename ThreadingPolicy::MutexLocker lockerR(*keepReference);
		typename ThreadingPolicy::MutexLocker locker(*this);
		for (auto& ptr: keepReference->pList)
			pList.push_back(ptr);
		return *this;
	}


	template<class T>
	Array<T>& Array<T>::operator << (const Array<T>& rhs)
	{
		typename ThreadingPolicy::MutexLocker lockerR(rhs);
		typename ThreadingPolicy::MutexLocker locker(*this);
		for (auto& ptr: rhs.pList)
			pList.push_back(ptr);
		return *this;
	}


	template<class T>
	Array<T>& Array<T>::operator << (const Ptr& rhs)
	{
		typename Array<T>::Ptr keepReference = rhs;
		typename ThreadingPolicy::MutexLocker lockerR(*keepReference);
		typename ThreadingPolicy::MutexLocker locker(*this);
		for (auto& ptr: keepReference->pList)
			pList.push_back(ptr);
		return *this;
	}


	template<class T>
	inline Array<T>& Array<T>::operator << (T* thread)
	{
		add(thread);
		return *this;
	}


	template<class T>
	inline Array<T>& Array<T>::operator << (const typename T::Ptr& thread)
	{
		add(thread);
		return *this;
	}


	template<class T>
	inline Array<T>& Array<T>::operator += (T* thread)
	{
		add(thread);
		return *this;
	}


	template<class T>
	inline Array<T>& Array<T>::operator += (const typename T::Ptr& thread)
	{
		add(thread);
		return *this;
	}


	template<class T>
	void Array<T>::appendNThreadsWL(uint n, bool autostart)
	{
		auto count = pList.size();
		if (count < n)
		{
			pList.reserve(n);
			for (auto i = count; i < n; ++i)
				pList.emplace_back(new T());
			if (autostart)
			{
				for (auto i = count; i < n; ++i)
					pList[i]->start();
			}
		}
	}


	template<class T>
	inline uint Array<T>::size() const
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		return (uint) pList.size();
	}


	template<class T>
	inline bool Array<T>::empty() const
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		return pList.empty();
	}



	template<class T>
	template<class PredicateT>
	void Array<T>::foreachThread(const PredicateT& predicate) const
	{
		// We will make a copy of the list to release the lock as soon as
		// possible since this routine may take some time...
		// (and to prevent dead-locks)
		ThreadList copy;
		{
			typename ThreadingPolicy::MutexLocker locker(*this);
			if (pList.empty())
				return;
			copy = pList;
		}
		for (auto& ptr: copy)
		{
			if (not predicate(ptr))
				return;
		}
	}


	template<class T>
	template<class PredicateT>
	void Array<T>::foreachThread(const PredicateT& predicate)
	{
		// We will make a copy of the list to release the lock as soon as
		// possible since this routine may take some time...
		// (and to prevent dead-locks)
		ThreadList copy;
		{
			typename ThreadingPolicy::MutexLocker locker(*this);
			if (pList.empty())
				return;
			copy = pList;
		}
		for (auto& ptr: copy)
		{
			if (not predicate(ptr))
				return;
		}
	}




} // namespace Thread
} // namespace Yuni

