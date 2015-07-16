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
#ifndef __YUNI_CORE_MATH_DISTANCE_HXX__
# define __YUNI_CORE_MATH_DISTANCE_HXX__

# include "../static/types.h"


namespace Yuni
{
namespace Private
{
namespace MathImpl
{

	template<typename T, bool IsDecimal = Static::Type::IsDecimal<T>::Yes>
	struct Distance2D
	{
		static inline T Compute(T x1, T y1, T x2, T y2)
		{
			return Math::SquareRoot<T>((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
		}
	};

	template<typename T>
	struct Distance2D<T,0>
	{
		static inline T Compute(T x1, T y1, T x2, T y2)
		{
			return Math::RoundToInt<double,T>::Value(
				Math::SquareRoot<double>(
					(double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))));
		}
	};


	template<typename T, bool IsDecimal = Static::Type::IsDecimal<T>::Yes>
	struct Distance3D
	{
		static inline T Compute(T x1, T y1, T z1, T x2, T y2, T z2)
		{
			return Math::SquareRoot<T>((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
		}
	};

	template<typename T>
	struct Distance3D<T,0>
	{
		static inline T Compute(T x1, T y1, T z1, T x2, T y2, T z2)
		{
			return Math::RoundToInt<double,T>::Value(
				Math::SquareRoot<double>(
					(double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1))));
		}
	};


} // namespace MathImpl
} // namespace Private
} // namespace Yuni

namespace Yuni
{
namespace Math
{


	template<typename T> inline T Distance2D(T x1, T y1, T x2, T y2)
	{
		return Private::MathImpl::Distance2D<T>::Compute(x1, y1, x2, y2);
	}

	template<typename T> inline T Distance3D(T x1, T y1, T z1, T x2, T y2, T z2)
	{
		return Private::MathImpl::Distance3D<T>::Compute(x1, y1, z1, x2, y2, z2);
	}



} // namespace Math
} // namespace Yuni

#endif // __YUNI_CORE_MATH_DISTANCE_H__
