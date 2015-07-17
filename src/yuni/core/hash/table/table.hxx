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
#include "table.h"



namespace Yuni
{
namespace Hash
{

	template<typename K, typename V, template<class> class TP>
	inline bool Table<K,V,TP>::exists(const K& key)
	{
		typename Table<K,V,TP>::ThreadingPolicy::MutexLocker locker(*this);
		return pTable.find(key) != pTable.end();
	}


	template<typename K, typename V, template<class> class TP>
	inline V& Table<K,V,TP>::operator[] (const K& key)
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		return pTable[key];
	}


	template<typename K, typename V, template<class> class TP>
	inline typename Table<K,V,TP>::iterator Table<K,V,TP>::find(const K& key)
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		return pTable.find(key);
	}


	template<typename K, typename V, template<class> class TP>
	inline V Table<K,V,TP>::value(const K& key, const V& defvalue)
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		const_iterator it = pTable.find(key);
		return (it == pTable.end()) ? defvalue : it->second;
	}

	template<typename K, typename V, template<class> class TP>
	inline void Table<K,V,TP>::erase(const K& key)
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		iterator it = pTable.find(key);
		if (pTable.end() != it)
			pTable.erase(it);
	}


	template<typename K, typename V, template<class> class TP>
	typename Table<K,V,TP>::iterator Table<K,V,TP>::begin()
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		return pTable.begin();
	}


	template<typename K, typename V, template<class> class TP>
	typename Table<K,V,TP>::iterator Table<K,V,TP>::end()
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		return pTable.end();
	}


	template<typename K, typename V, template<class> class TP>
	void Table<K,V,TP>::clear()
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		pTable.clear();
	}


	template<typename K, typename V, template<class> class TP>
	inline int Table<K,V,TP>::size()
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		return pTable.size();
	}


	template<typename K, typename V, template<class> class TP>
	inline std::pair<typename Table<K,V,TP>::iterator, bool> Table<K,V,TP>::insert(const K& key)
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		return pTable.insert(key);
	}




} // namespace Hash
} // namespace Yuni
