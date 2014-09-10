#ifndef __YUNI_CORE_FUNCTIONAL_BINARYFUNCTIONS_H__
# define __YUNI_CORE_FUNCTIONAL_BINARYFUNCTIONS_H__

# include "../../yuni.h"
# include "../math.h"

namespace Yuni
{
namespace Functional
{


	template<class T>
	class Max
	{
	public:
		bool operator () (T& val1, const T& val2) const
		{
			val1 = Math::Max(val1, val2);
			return true;
		}

	}; // class Max


	template<class T>
	class Min
	{
	public:
		bool operator () (T& val1, const T& val2) const
		{
			val1 = Math::Min(val1, val2);
			return true;
		}

	}; // class Min


	template<class T>
	class Add
	{
	public:
		bool operator () (T& val1, const T& val2) const
		{
			val1 += val2;
			return true;
		}

	}; // class Add


	template<class T>
	class Sub
	{
	public:
		bool operator () (T& val1, const T& val2) const
		{
			val1 -= val2;
			return true;
		}

	}; // class Sub


	template<class T>
	class Mul
	{
	public:
		bool operator () (const T& val1, const T& val2) const
		{
			val1 *= val2;
			return true;
		}

	}; // class Mul


	template<class T>
	class Div
	{
	public:
		bool operator () (T& val1, const T& val2) const
		{
			val1 /= val2;
			return true;
		}

	}; // class Div


	template<class T>
	class And
	{
	public:
		bool operator () (T& val1, const T& val2) const
		{
			val1 = val1 && val2;
			return true;
		}

	}; // class And


	template<class T>
	class Or
	{
	public:
		bool operator () (T& val1, const T& val2) const
		{
			val1 = val1 || val2;
			return true;
		}

	}; // class Or


	template<class T>
	class Xor
	{
	public:
		bool operator () (T& val1, const T& val2) const
		{
			val1 = val1 ^ val2;
			return true;
		}

	}; // class Xor


#define lambda(A, B, EXPR) struct { \
		bool operator () (A, B) const \
		{ \
			EXPR; \
		} \
	}



} // namespace Functional
} // namespace Yuni

#endif // __YUNI_CORE_FUNCTIONAL_BINARYFUNCTIONS_H__
