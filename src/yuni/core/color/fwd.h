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
#include "../math.h"
#include "../string.h"
#include "../static/if.h"
#include "../static/types.h"



namespace Yuni
{
namespace Color
{

	// forward declarations
	template<class T> class RGB;
	template<class T> class RGBA;


} // namespace Color
} // namespace Yuni


namespace Yuni
{
namespace Private
{
namespace Color
{

	/*!
	** \brief A default transparency upperBound, according to a given type
	*/
	template<class T> struct DefaultValues       { enum { lowerBound = 0, upperBound = 255 }; };

	template<> struct DefaultValues<float>       { enum { lowerBound = 0, upperBound = 1 }; };
	template<> struct DefaultValues<double>      { enum { lowerBound = 0, upperBound = 1 }; };
	# ifdef YUNI_HAS_LONG_DOUBLE
	template<> struct DefaultValues<long double> { enum { lowerBound = 0, upperBound = 1 }; };
	# endif


	/*!
	** \brief The most appropriate type for printing
	*/
	template<class T> struct Print  { typedef T Type; };
	template<> struct Print<char>  { typedef int Type; };
	template<> struct Print<unsigned char>  { typedef uint Type; };


	template<class T>
	struct SignedType
	{
		typedef typename Static::If<(T(-1) < T(0)), int, uint>::Type Type;
	};

	template<> struct SignedType<float>       { typedef float Type; };
	template<> struct SignedType<double>      { typedef double Type; };
	# ifdef YUNI_HAS_LONG_DOUBLE
	template<> struct SignedType<long double> { typedef long double Type; };
	# endif

	/*!
	** \brief The most appropriate type for calculation
	*/
	template<class T, class V> struct Calculation
	{
		typedef
			typename Static::If<Static::Type::StrictlyEqual<T, long double>::Yes || Static::Type::StrictlyEqual<V, long double>::Yes,
				long double,
				typename Static::If<Static::Type::StrictlyEqual<T, double>::Yes || Static::Type::StrictlyEqual<V, double>::Yes,
					double,
					typename Static::If<Static::Type::StrictlyEqual<T, float>::Yes || Static::Type::StrictlyEqual<V, float>::Yes,
						float,
						typename SignedType<T>::Type >::Type
				>::Type
			>::Type Type;

	}; // struct Calculation



} // namespace Color
} // namespace Private
} // namespace Yuni
