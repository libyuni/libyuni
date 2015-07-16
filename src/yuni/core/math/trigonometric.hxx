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
#ifndef __YUNI_CORE_MATH_TRIGONOMETRIC_HXX__
# define __YUNI_CORE_MATH_TRIGONOMETRIC_HXX__



namespace Yuni
{
namespace Math
{

	template<typename T> inline T Cos(T x)
	{
		return (T)::cos((T)x);
	}

	template<> inline float Cos<float>(float x)
	{
		return ::cosf(x);
	}

	template<> inline long double Cos<long double>(long double x)
	{
		return ::cosl(x);
	}


	template<typename T> inline T Sin(T x)
	{
		return (T)::sin((T)x);
	}

	template<> inline float Sin<float>(float x)
	{
		return ::sinf(x);
	}

	template<> inline long double Sin<long double>(long double x)
	{
		return ::sinl(x);
	}


	template<typename T> inline T Tan(T x)
	{
		return (T)::tan((T)x);
	}

	template<> inline float Tan<float>(float x)
	{
		return ::tanf(x);
	}

	template<> inline long double Tan<long double>(long double x)
	{
		return ::tanl(x);
	}



	template<typename T> inline T ACos(T x)
	{
		return (T)::cos((T)x);
	}

	template<> inline float ACos<float>(float x)
	{
		return ::acosf(x);
	}

	template<> inline long double ACos<long double>(long double x)
	{
		return ::acosl(x);
	}


	template<typename T> inline T ASin(T x)
	{
		return (T)::asin((T)x);
	}

	template<> inline float ASin<float>(float x)
	{
		return ::asinf(x);
	}

	template<> inline long double ASin<long double>(long double x)
	{
		return ::asinl(x);
	}


	template<typename T> inline T ATan(T x)
	{
		return (T)::atan((T)x);
	}

	template<> inline float ATan<float>(float x)
	{
		return ::atanf(x);
	}

	template<> inline long double ATan<long double>(long double x)
	{
		return ::atanl(x);
	}

	template<typename T> inline T ATan2(T y, T x)
	{
		return (T)::atan2(y, x);
	}

	template<> inline float ATan2<float>(float y, float x)
	{
		return ::atan2f(y, x);
	}

	template<> inline long double ATan2<long double>(long double y, long double x)
	{
		return ::atan2l(y, x);
	}




	template<typename T> inline T ASinH(T x)
	{
		# if defined(YUNI_OS_MSVC)
		return (T)::log((T)(x + ::sqrt(x * x + 1)));
		# else
		return (T)::asinh((T)x);
		# endif
	}

	template<> inline float ASinH<float>(float x)
	{
		# if defined(YUNI_OS_MSVC)
		return ::logf((x + ::sqrtf(x * x + 1)));
		# else
		return ::asinhf(x);
		# endif
	}

	template<> inline long double ASinH<long double>(long double x)
	{
		# if defined(YUNI_OS_MSVC)
		return ::logl((x + ::sqrtl(x * x + 1)));
		# else
		return ::asinhl(x);
		# endif
	}




	template<typename T> inline T ACosH(T x)
	{
		# if defined(YUNI_OS_MSVC)
		return (T)::log( (T)(x + ::sqrt(x * x - 1)));
		# else
		return (T)::acosh((T)x);
		# endif
	}

	template<> inline float ACosH<float>(float x)
	{
		# if defined(YUNI_OS_MSVC)
		return ::logf( (x + ::sqrtf(x * x - 1)));
		# else
		return ::acoshf(x);
		# endif
	}

	template<> inline long double ACosH<long double>(long double x)
	{
		# if defined(YUNI_OS_MSVC)
		return ::logl( (x + ::sqrtl(x * x - 1)));
		# else
		return ::acoshl(x);
		# endif
	}



	template<typename T> inline T ATanH(T x)
	{
		# if defined(YUNI_OS_MSVC)
		return (T)::log((T)((T)(1. / x + 1.) / (T)(1. / x - 1.))) / 2.;
		# else
		return (T)::atanh((T)x);
		# endif
	}


	template<> inline float ATanH<float>(float x)
	{
		# if defined(YUNI_OS_MSVC)
		return ::logf(((1.f / x + 1.f) / (1.f / x - 1.f))) / 2.f;
		# else
		return ::atanhf(x);
		# endif
	}

	template<> inline long double ATanH<long double>(long double x)
	{
		# if defined(YUNI_OS_MSVC)
		return ::logl(((1. / x + 1.) / (1. / x - 1.))) / 2.;
		# else
		return ::atanhl(x);
		# endif
	}





} // namespace Math
} // namespace Yuni

#endif // __YUNI_CORE_MATH_TRIGONOMETRIC_HXX__
