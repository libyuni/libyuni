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
#include "../../math/base.h"
#include <cmath>


namespace Yuni
{
namespace Private
{
namespace CStringImpl
{


	enum
	{
		kindUnknown,
		kindIntegral,
		kindDecimal,
	};


	template<class T> struct TypeInfo final { enum { kind = kindIntegral, }; };
	template<> struct TypeInfo<float> final { enum { kind = kindDecimal, }; };
	template<> struct TypeInfo<double> final { enum { kind = kindDecimal, }; };
	template<> struct TypeInfo<long double> final { enum { kind = kindDecimal, }; };





	template<class BaseT, class T, int KindT = TypeInfo<T>::kind> class From {};

	template<class BaseT, class T> class From<BaseT, T, 0> { /* Unknown */ };


	template<class BaseT, class T>
	class From<BaseT, T, kindIntegral> final
	{
	public:
		//! Type
		typedef T Type;

		enum
		{
			/*!
			** \brief Minimum length for a temporary buffer
			**
			** The buffer size is proportional to the maximum number of digits
			** for displaying the value.
			** The real formula is : n.log10(2) (for Base 10).
			** But we add a few extra spaces.
			*/
			bufferCapacity = (BaseT::n >= 10)
				? ((1 + sizeof(Type) * 8 * 309 / 1024) + ((sizeof(Type) * 8 * 309 / 1024) % 8))
				: (1 + sizeof(Type) * 8),

			/*!
			** \brief A non-zero value if an arbitrary value T can be negative
			**
			** \internal The operator '>' is used to avoid a compiler warning
			*/
			strictlyPositiveByContract = (Type(-1) > 0),
		};

	public:
		template<class S>
		static void AppendTo(S& str, Type value, int /* precision */= 0)
		{
			if (!value)
			{
				// nothing to do if equals to 0
				str.appendWithoutChecking(*BaseT::Digit());
			}
			else
			{
				// From this point, the value can not be null

				// A temporary buffer to store the result
				char buffer[(uint)bufferCapacity];
				// A pointer to the current char (at the end of the buffer)
				char* p = buffer + ((uint)bufferCapacity - 1);
				// Char count
				uint nbChar = 0;

				if ((uint) strictlyPositiveByContract or value > 0)
				{
					// The given variable is strictly positive, by contract or by value
					// note: The do..while structure has better performances in
					// our case
					do
					{
						*p = *(BaseT::Digit() + (value % BaseT::n));
						--p;
						++nbChar;
					} while ((value /= BaseT::n));
					str.appendWithoutChecking(++p, nbChar);
				}
				else
				{
					// The given variable is strictly negative
					// note: The do..while structure has better performances in
					// our case
					do
					{
						*p = *(BaseT::Digit() - (value % BaseT::n));
						--p;
						++nbChar;
					} while ((value /= BaseT::n));
					*p = '-';
					str.appendWithoutChecking(p, ++nbChar);
				}
			}
		}

	}; // class From<T>



	// This specialization should not be used
	// The conversion may not be what expected since no rounding
	// is done.
	template<class BaseT, class T>
	class From<BaseT, T, kindDecimal> final
	{
	public:
		//! Type
		typedef double Type;

		enum
		{
			/*!
			** \brief Minimum Buffer length
			*/
			bufferCapacity = (sizeof(Type) * 5 >> 1) + 1 /* sign */,

			/*!
			** \brief A non-zero value if an arbitrary value T can be negative
			**
			** \internal The operator '>' is used to avoid a compiler warning
			*/
			strictlyPositiveByContract = 0,
		};

	public:
		template<class S>
		static void AppendTo(S& str, Type value, int precision = 3)
		{
			// fpclassify: man 3 fpclassify  (macro)
			switch (std::fpclassify(value))
			{
				case FP_NAN: str.appendWithoutChecking("nan", 3);return;
				case FP_INFINITE:
					str.appendWithoutChecking(((std::isinf(value) > 0) ? "+inf" : "-inf"), 4);
					return;
				default:
					{
						if (value < 0)
						{
							str += '-';
							AppendToFromSign<S>(str, fabs(value), precision);
						}
						else
							AppendToFromSign<S>(str, value, precision);
					}
			}
		}

	private:
		template<class S>
		static void AppendToFromSign(S& str, Type value, int precision)
		{
			const Type integral = ::floor(value);
			if (integral < 4294967295u)
				From<BaseT, uint32_t>::AppendTo(str, (uint32_t)integral);
			else
				From<BaseT, uint64_t>::AppendTo(str, (uint64_t)integral);
			if (precision > 0)
			{
				str += '.';
				const Type decimal = ::round((value - integral) * Power10(precision));
				if (decimal < 4294967295u)
					From<BaseT, uint32_t>::AppendTo(str, (uint32_t)decimal);
				else
					From<BaseT, uint64_t>::AppendTo(str, (uint64_t)decimal);
			}
		}

		static int32_t Power10(int precision)
		{
			switch (precision)
			{
				case 3:  return 1000;      // The most widely used value first
				case 6:  return 1000000;
				case 1:  return 10;
				case 2:  return 100;
				case 4:  return 10000;
				case 5:  return 100000;
				case 7:  return 10000000;
				case 8:  return 100000000;
				default: return 1000000000;
			}
		}

	}; // class From<T>






} // namespace CStringImpl
} // namespace Private
} // namespace Yuni
