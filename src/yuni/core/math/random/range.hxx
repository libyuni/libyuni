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
#include "range.h"



namespace Yuni
{
namespace Math
{
namespace Random
{


	template<class D, int LoValue, int HiValue, typename T>
	inline Range<D,LoValue,HiValue,T>::Range()
	{}


	template<class D, int LoValue, int HiValue, typename T>
	inline Range<D,LoValue,HiValue,T>::~Range()
	{}


	template<class D, int LoValue, int HiValue, typename T>
	inline const T
	Range<D,LoValue,HiValue,T>::min()
	{
		return T(LoValue);
	}


	template<class D, int LoValue, int HiValue, typename T>
	inline const T
	Range<D,LoValue,HiValue,T>::max()
	{
		return T(HiValue);
	}



	template<class D, int LoValue, int HiValue, typename T>
	inline void
	Range<D,LoValue,HiValue,T>::reset()
	{
		pDistribution.reset();
	}



	namespace
	{
		template<class D, int LoValue, int HiValue, typename T>
		struct AlgorithmFromType
		{
			static inline T Next(D& d)
			{
				// Default algorithm, suitable for Integer types
				return T((double)d.next() / (double(d.max()) + 1.) * T(HiValue - LoValue)) + LoValue;
			}
		};

		template<class D, int LoValue, int HiValue>
		struct AlgorithmFromType<D, LoValue, HiValue, float>
		{
			static inline float Next(D& d)
			{
				return float(float(d.next()) / ((float(d.max()) + 1.) / float(HiValue - LoValue)) + float(LoValue));
			}
		};

		template<class D, int LoValue, int HiValue>
		struct AlgorithmFromType<D, LoValue, HiValue, double>
		{
			static inline double Next(D& d)
			{
				return double(d.next()) / ((double(d.max()) + 1.) / double(HiValue)) + double(LoValue);
			}
		};

		template<class D, int LoValue, int HiValue>
		struct AlgorithmFromType<D, LoValue, HiValue, long double>
		{
			static inline long double Next(D& d)
			{
				return (long double)(d.next()) / (((long double)(d.max()) + 1.) / (long double)(HiValue)) + (long double)(LoValue);
			}
		};

	} // anonymouse namespace



	template<class D, int LoValue, int HiValue, typename T>
	inline const T
	Range<D,LoValue,HiValue,T>::next()
	{
		return AlgorithmFromType<D, LoValue, HiValue, T>::Next(pDistribution);
	}




} // namespace Random
} // namespace Math
} // namespace Yuni
