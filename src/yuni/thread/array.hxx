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

	namespace {


	template<class T>
	void expandThreadList(T& list, uint32_t newsize, bool autostart) {
		auto count = list.size();
		assert(newsize > count);
		list.reserve(newsize);
		for (auto i = count; i < newsize; ++i)
			list.push_back(new typename T::value_type());
		if (autostart)
		{
			for (auto i = count; i < newsize; ++i)
				list[i]->start();
		}
	}


	template<class T>
	void stopAllThreads(T& list, uint32_t timeout) {
		for (auto& ptr: list)
			ptr->gracefulStop();
		for (auto& ptr: list)
			ptr->stop(timeout);
	}


	} // namespace


	template<class T>
	Array<T>::Array(const Array<T>& rhs)
	{
		typename ThreadingPolicy::MutexLocker locker(rhs);
		pAutoStart = rhs.pAutoStart;
		pList = rhs.pList;
	}


	template<class T>
	inline Array<T>::Array(uint newsize)
	{
		expandThreadList(pList, newsize, false);
	}


	template<class T>
	inline Array<T>::Array(uint n, bool autoStart)
		: pAutoStart{autoStart}
	{
		expandThreadList(pList, n, autoStart);
	}


	template<class T>
	inline Array<T>::~Array()
	{
		clear();
	}


	template<class T>
	inline bool Array<T>::autoStart() const
	{
		return pAutoStart;
	}


	template<class T>
	inline void Array<T>::autoStart(const bool v)
	{
		pAutoStart  = v;
	}


	template<class T>
	void Array<T>::clear()
	{
		ThreadList tmplist;
		{
			typename ThreadingPolicy::MutexLocker locker{*this};
			if (pList.empty())
				return;
			tmplist.swap(pList);
		}
		stopAllThreads(tmplist, defaultTimeout);
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
		pList.emplace_back(thread);
	}


	template<class T>
	void Array<T>::resize(uint32_t newsize)
	{
		ThreadList tmplist; // for thread operations outside of the lock
		{
			typename ThreadingPolicy::MutexLocker locker{*this};
			uint32_t count = static_cast<uint32_t>(pList.size());
			if (count == newsize)
				return;
			if (newsize > count)
			{
				expandThreadList(pList, newsize, pAutoStart);
				return;
			}
			if (newsize == 0)
			{
				tmplist.swap(pList);
			}
			else
			{
				tmplist.reserve(count - newsize);
				for (uint32_t i = newsize; i != count; ++i)
					tmplist.emplace_back(pList[i]);
				pList.resize(newsize);
			}
		}
		stopAllThreads(tmplist, defaultTimeout);
	}


	template<class T>
	void Array<T>::start()
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		for (auto& ptr: pList)
			ptr->start();
	}


	template<class T>
	void Array<T>::gracefulStop()
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		for (auto& ptr: pList)
			ptr->gracefulStop();
	}


	template<class T>
	void Array<T>::wait()
	{
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
		ThreadList copy;
		{
			typename ThreadingPolicy::MutexLocker locker(*this);
			if (pList.empty())
				return;
			copy = pList;
		}
		stopAllThreads(copy, timeout);
	}


	template<class T>
	void Array<T>::restart(uint timeout)
	{
		ThreadList copy;
		{
			typename ThreadingPolicy::MutexLocker locker(*this);
			if (pList.empty())
				return;
			copy = pList;
		}
		stopAllThreads(copy, timeout);
		for (auto& ptr: copy)
			ptr->start();
	}


	template<class T>
	void Array<T>::wakeUp()
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
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
		ThreadList tmplist;
		{
			typename ThreadingPolicy::MutexLocker locker(*this);
			typename ThreadingPolicy::MutexLocker lockerR(rhs);
			pAutoStart = rhs.pAutoStart;
			tmplist.swap(pList);
			pList = rhs.pList;
		}
		return *this;
	}


	template<class T>
	Array<T>& Array<T>::operator = (const Ptr& rhs)
	{
		ThreadList tmplist;
		{
			typename ThreadingPolicy::MutexLocker locker(*this);
			tmplist.swap(pList);
			if (!!rhs) {
				typename ThreadingPolicy::MutexLocker lockerR{*rhs};
				pAutoStart = rhs->pAutoStart;
				pList = rhs->pList;
			}
		}
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
		if (!!rhs)
		{
			typename ThreadingPolicy::MutexLocker lockerR(*rhs);
			typename ThreadingPolicy::MutexLocker locker(*this);
			for (auto& ptr: rhs->pList)
				pList.push_back(ptr);
		}
		return *this;
	}


	template<class T>
	Array<T>& Array<T>::operator << (const Array<T>& rhs)
	{
		return operator + (rhs);
	}


	template<class T>
	Array<T>& Array<T>::operator << (const Ptr& rhs)
	{
		return operator + (rhs);
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
				break;
		}
	}


	template<class T>
	template<class PredicateT>
	void Array<T>::foreachThread(const PredicateT& predicate)
	{
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
				break;
		}
	}




} // namespace Thread
} // namespace Yuni

