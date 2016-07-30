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
#include <type_traits>
#include <initializer_list>



namespace Yuni
{


	/*!
	** \brief Space-efficient set of flags from an enum class
	*/
	template<class T>
	class Flags final
	{
	public:
		//! Underlying type
		typedef typename std::underlying_type<T>::type Type;

	public:
		//! \name Constructors
		//@{
		//! Default constructor
		Flags() = default;
		//! Copy constructor
		Flags(const Flags&) = default;

		//! Constructor from initializer_list
		Flags(const std::initializer_list<T>&);
		//@}


		//! Remove all flags
		void clear();

		//! Get if no flag is set
		bool empty() const;


		//! Set all flags
		void setAll();

		//! Set a particular flag
		void set(T flag);

		//! Set all flags set in another container
		void set(const Flags<T>&);

		//! Set flags from initializer_list
		void set(const std::initializer_list<T>&);


		//! Remove a particular flag
		void unset(T flag);

		//! Unset all flags set in another container
		void unset(const Flags<T>&);

		//! Unset flags from initializer_list
		void unset(const std::initializer_list<T>&);


		//! Toggle a particular flag
		void toggle(T flag);


		//! Get if a flag is set
		bool get(T flag) const;
		//! Get if the set of flags is set
		bool contains(const std::initializer_list<T>&) const;
		//! Get if at least one of the given flag is set
		bool containsOneOf(const std::initializer_list<T>&) const;


		//! Set all but a particular flag
		void setAllBut(T flag);

		//! Unset all but a particular flag
		void unsetAllBut(T flag);


		//! set a particular flag
		Flags& operator += (T);
		//! set flags from another set
		Flags& operator += (const Flags<T>& rhs);
		//! set a list of flags
		Flags& operator += (std::initializer_list<T>);

		//! unset a particular flag
		Flags& operator -= (T);
		//! unset flags set in another set
		Flags& operator -= (const Flags<T>& rhs);
		//! Unset a list of flags
		Flags& operator -= (std::initializer_list<T>);


		//! Get if a particular flag is set
		bool operator () (T flag) const;

		//! Compare two set of flags
		bool operator == (const Flags&) const;

		//! Assign
		Flags& operator = (const Flags&) = default;
		//! Assign from an initializer_list
		Flags& operator = (const std::initializer_list<T>&);


	private:
		Type pBits = 0u;

	}; // class Flags





} // namespace Yuni


template<class T> Yuni::Flags<T> operator + (const Yuni::Flags<T>& a, const Yuni::Flags<T>& b);
template<class T> Yuni::Flags<T> operator - (const Yuni::Flags<T>& a, const Yuni::Flags<T>& b);

#include "flags.hxx"
