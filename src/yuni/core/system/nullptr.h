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
# ifdef __cplusplus /* Only with a C++ Compiler */

/* nullptr_t does not exist in GCC 4.5 */
# if YUNI_OS_GCC_VERSION >= 40000 && YUNI_OS_GCC_VERSION < 40600
#	undef YUNI_HAS_NULLPTR
# endif

# ifdef YUNI_HAS_NULLPTR
#	include <cstddef>
# endif


# ifdef YUNI_HAS_NULLPTR

typedef decltype(nullptr) YuniNullPtr;

# else

/*!
** \brief C++ Idioms/nullptr
**
** \see proposal (N2431)
*/
const class YuniNullPtr final
{
public:
	/*! Convertible to any type of null non-member */
	template<class T> inline operator T*() const
	{
		return 0;
	}

	/*!
	** \brief Any type of null member pointer
	**
	** \note In gcc 4.1.1 compiler that does not recognize the comparison of
	** nullptr with point to member function.
	*/
	template<class C, class T> inline operator T C::*() const
	{
		return 0;
	}

	# if YUNI_OS_GCC_VERSION >= 40400
	template<class T> bool operator == (const T* rhs) const
	{
		return !rhs;
	}

	template<class T> bool operator != (const T* rhs) const
	{
		return (NULL != rhs);
	}
	# endif

private:
	void operator&() const;  /* Can't take address of nullptr */

} nullptr = {};


# endif /* nullptr support */


namespace Yuni
{

	typedef /*YUNI_DECL*/  YuniNullPtr  NullPtr;

} /* namespace Yuni */



# ifndef YUNI_HAS_NULLPTR
# if YUNI_OS_GCC_VERSION >= 40400
template<class T>
inline bool operator == (const T* rhs, const Yuni::NullPtr&)
{
	return !rhs;
}

template<class T>
inline bool operator != (const T* rhs, const Yuni::NullPtr&)
{
	return static_cast<const T*>(NULL) != rhs;
}
# endif
# endif

# endif /* C++ Compiler */

