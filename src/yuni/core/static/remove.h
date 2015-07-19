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


namespace Yuni
{
namespace Static
{

/*!
** \brief Remove some part about a single type
*/
namespace Remove
{

	//! \name Reference
	//@{
	//! Remove the reference of a type (* or &)
	template <typename T> struct Reference { typedef T Type; };
	template <typename T> struct Reference<T&> { typedef T Type; };
	template <typename T> struct Reference<volatile T&> { typedef volatile T Type; };
	template <typename T> struct Reference<T*> { typedef T Type; };
	template <typename T> struct Reference<T* const> { typedef T Type; };
	template <typename T> struct Reference<volatile T*> { typedef volatile T Type; };
	template <typename T, int N> struct Reference<T[N]> { typedef T Type; };
	template <typename T> struct Reference<const T*> { typedef const T Type; };
	template <typename T> struct Reference<const T* const> { typedef const T Type; };
	template <typename T> struct Reference<const volatile T*> { typedef const volatile T Type; };
	template <typename T, int N> struct Reference<const T[N]> { typedef const T Type; };
	template <typename T> struct Reference<const T&> { typedef const T Type; };
	template <typename T> struct Reference<const volatile T&> { typedef const volatile T Type; };

	template <typename T> struct RefOnly { typedef T Type; };
	template <typename T> struct RefOnly<T&> { typedef T Type; };
	template <typename T> struct RefOnly<volatile T&> { typedef volatile T Type; };
	template <typename T> struct RefOnly<const T&> { typedef const T Type; };
	template <typename T> struct RefOnly<const volatile T&> { typedef const volatile T Type; };

	template <typename T> struct PntOnly { typedef T Type; };
	template <typename T> struct PntOnly<T*> { typedef T Type; };
	template <typename T> struct PntOnly<volatile T*> { typedef volatile T Type; };
	template <typename T> struct PntOnly<const T*> { typedef const T Type; };
	template <typename T> struct PntOnly<const T* const> { typedef const T Type; };
	template <typename T> struct PntOnly<T* const> { typedef const T Type; };
	template <typename T> struct PntOnly<const volatile T*> { typedef const volatile T Type; };
	//@}

	//! \name Volatile
	//@{
	template <typename T> struct Volatile { typedef T Type; };
	template <typename T> struct Volatile<volatile T*> { typedef T* Type; };
	template <typename T> struct Volatile<const volatile T*> { typedef const T* Type; };
	template <typename T> struct Volatile<const volatile T* const> { typedef const T* const Type; };
	template <typename T> struct Volatile<volatile T&> { typedef T& Type; };
	template <typename T> struct Volatile<const volatile T&> { typedef const T& Type; };
	//@}


	//! \name Const
	//@{
	//! Remove the "Const" part of a type
	template <typename T> struct Const { typedef T Type; };
	template <typename T> struct Const<T*> { typedef T* Type; };
	template <typename T> struct Const<T* const> { typedef T* Type; };
	template <typename T> struct Const<T&> { typedef T& Type; };
	template <typename T, int N> struct Const<T[N]> { typedef T Type[N]; };
	template <typename T> struct Const<const T>  { typedef T Type; };
	template <typename T> struct Const<const T*> { typedef T* Type; };
	template <typename T> struct Const<const T* const> { typedef T* Type; };
	template <typename T> struct Const<const T&> { typedef T& Type; };
	template <typename T, int N> struct Const<const T[N]> { typedef T Type[N]; };
	//@}

	template <typename T> struct All
	{
		typedef typename Remove::Volatile<
			typename Remove::Const<
				typename Remove::Reference<T>::Type >::Type>::Type   Type;
	};





} // namespace Remove
} // namespace Static
} // namespaec Yuni
