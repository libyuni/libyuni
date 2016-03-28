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
#include "flags.h"



namespace Yuni
{

	template<class T>
	inline Flags<T>::Flags(const std::initializer_list<T>& list)
	{
		set(list);
	}

	template<class T>
	inline void Flags<T>::clear()
	{
		pBits = 0u;
	}

	template<class T>
	inline void Flags<T>::setAll()
	{
		pBits = ~(pBits & 0);
	}


	template<class T>
	inline bool Flags<T>::empty() const
	{
		return 0 == pBits;
	}


	template<class T>
	inline void Flags<T>::set(T flag)
	{
		pBits |= (1 << static_cast<Type>(flag));
	}

	template<class T>
	inline void Flags<T>::set(const std::initializer_list<T>& list)
	{
		for (auto flag: list)
			set(flag);
	}


	template<class T>
	inline void Flags<T>::set(const Flags<T>& flags)
	{
		pBits |= flags.pBits;
	}


	template<class T>
	inline void Flags<T>::unset(T flag)
	{
		pBits &= ~(1 << static_cast<Type>(flag));
	}


	template<class T>
	inline void Flags<T>::unset(const Flags<T>& flags)
	{
		pBits &= ~flags.pBits;
	}

	template<class T>
	inline void Flags<T>::unset(const std::initializer_list<T>& list)
	{
		for (auto flag: list)
			unset(flag);
	}


	template<class T>
	inline bool Flags<T>::get(T flag) const
	{
		return 0 != (pBits & (1 << static_cast<Type>(flag)));
	}


	template<class T>
	inline bool Flags<T>::contains(const std::initializer_list<T>& list) const
	{
		for (auto flag: list)
		{
			if (not get(flag))
				return false;
		}
		return true;
	}



	template<class T>
	inline bool Flags<T>::operator () (T flag) const
	{
		return get(flag);
	}


	template<class T>
	inline void Flags<T>::toggle(T flag)
	{
		pBits ^= 1 << static_cast<Type>(flag);
	}


	template<class T>
	inline void Flags<T>::setAllBut(T flag)
	{
		pBits = ((~(pBits & 0)) & (1 << static_cast<Type>(flag)));
	}


	template<class T>
	inline void Flags<T>::unsetAllBut(T flag)
	{
		pBits = 1 << static_cast<Type>(flag);
	}


	template<class T> inline Flags<T>& Flags<T>::operator += (T flag)
	{
		set(flag);
		return *this;
	}

	template<class T> inline Flags<T>& Flags<T>::operator += (std::initializer_list<T> flags)
	{
		set(flags);
		return *this;
	}

	template<class T> inline Flags<T>& Flags<T>::operator += (const Flags<T>& flags)
	{
		set(flags);
		return *this;
	}


	template<class T> inline Flags<T>& Flags<T>::operator -= (T flag)
	{
		unset(flag);
		return *this;
	}

	template<class T> inline Flags<T>& Flags<T>::operator -= (std::initializer_list<T> flags)
	{
		unset(flags);
		return *this;
	}

	template<class T> inline Flags<T>& Flags<T>::operator -= (const Flags<T>& flags)
	{
		unset(flags);
		return *this;
	}



	template<class T>
	inline bool Flags<T>::operator == (const Flags<T>& rhs) const
	{
		return pBits == rhs.pBits;
	}


	template<class T>
	inline Flags<T>& Flags<T>::operator = (const std::initializer_list<T>& list)
	{
		pBits = 0;
		set(list);
		return *this;
	}






} // namespace Yuni



template<class T> Yuni::Flags<T> operator + (const Yuni::Flags<T>& a, const Yuni::Flags<T>& b)
{
	return (Yuni::Flags<T>(a) += b);
}


template<class T> Yuni::Flags<T> operator - (const Yuni::Flags<T>& a, const Yuni::Flags<T>& b)
{
	return (Yuni::Flags<T>(a) -= b);
}
