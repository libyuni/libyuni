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
#ifndef __YUNI_CORE_MATH_LOG_HXX__
# define __YUNI_CORE_MATH_LOG_HXX__

# include <cassert>


namespace Yuni
{
namespace Math
{


	template<class T> inline T Log(T x)
	{
		return ::log(x);
	}

	template<> inline float Log<float>(float x)
	{
		return ::logf(x);
	}

	template<> inline long double Log<long double>(long double x)
	{
		return ::logl(x);
	}


	template<class T> inline T Log2(T x)
	{
		# if defined(YUNI_OS_MSVC)
		return static_cast<T>(log(x) / YUNI_LOG_2);
		# else
		return ::log2(x);
		# endif
	}

	template<> inline float Log2<float>(float x)
	{
		# if defined(YUNI_OS_MSVC)
		return static_cast<float>(logf(x) / YUNI_LOG_2);
		# else
		return ::log2f(x);
		# endif

	}

	template<> inline long double Log2<long double>(long double x)
	{
		# if defined(YUNI_OS_MSVC)
		return static_cast<long double>(logl(x) / YUNI_LOG_2);
		# else
		return ::log2l(x);
		# endif
	}


	template<class T> inline T LogOnePlusX(T x)
	{
		/* Assert */
		assert(x > -1. && "x must be greater than -1.0");

		return (Abs(x) >= 1e-4)
			// Direct calculation for larger values
			? Log(1. + x)
			// Taylor approx. log(1 + x) = x - x^2/2! + x^3/3!...
			: (-0.5 * x + 1.) * x;
	}






} // namespace Math
} // namespace Yuni


#endif // __YUNI_CORE_MATH_LOG_HXX__
