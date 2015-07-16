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
#ifndef __YUNI_CORE_MATH_GEOMETRY_POINT2D_HXX__
# define __YUNI_CORE_MATH_GEOMETRY_POINT2D_HXX__


namespace Yuni
{

	template<class T>
	inline Point2D<T>::Point2D() :
		x(), y()
	{}


	template<class T>
	template<class U, class V>
	inline Point2D<T>::Point2D(const U x1, const V y1) :
		x((T)x1), y((T)y1)
	{}


	template<class T>
	template<class U>
	inline Point2D<T>::Point2D(const Point2D<U>& p) :
		x((T)p.x), y((T)p.y)
	{}


	template<class T>
	inline Point2D<T>& Point2D<T>::reset()
	{
		x = y = T();
		return *this;
	}


	template<class T>
	inline void Point2D<T>::move(const T& x1, const T& y1)
	{
		x = x1;
		y = y1;
	}


	template<class T>
	template<class U>
	inline void Point2D<T>::move(const Point2D<U>& p)
	{
		x = (T) p.x;
		y = (T) p.y;
	}


	template<class T>
	inline void Point2D<T>::translate(const T& k)
	{
		x += k;
		y += k;
	}


	template<class T>
	inline void Point2D<T>::translate(const T& x1, const T& y1)
	{
		x += x1;
		y += y1;
	}


	template<class T>
	template<class U>
	inline void Point2D<T>::translate(const Point2D<U>& p)
	{
		x += (T) p.x;
		y += (T) p.y;
	}


	template<class T>
	template<class U>
	inline void Point2D<T>::mean(const Point2D<U>& p)
	{
		x = (x + p.x) / T(2);
		y = (y + p.y) / T(2);
	}


	template<class T>
	template<class U, class V>
	inline Point2D<T>& Point2D<T>::mean(const Point2D<U>& p1, const Point2D<V>& p2)
	{
		x = (p1.x + p2.x) / T(2);
		y = (p1.y + p2.y) / T(2);
		return *this;
	}


	template<class T>
	inline Point2D<T> Point2D<T>::Mean(const Point2D<T>& p1, const Point2D<T>& p2)
	{
		return Point2D<T>().mean(p1, p2);
	}


	template<class T>
	template<class U>
	bool Point2D<T>::isCloseTo(const Point2D<U>& rhs, const T& delta) const
	{
		return Math::Abs((U) x - rhs.x) < delta and Math::Abs((U) y - rhs.y) < delta;
	}


	template<class T>
	inline bool Point2D<T>::isCloseTo(const T& x1, const T& y1, const T& delta) const
	{
		return Math::Abs(x - x1) < delta and Math::Abs(y - y1) < delta;
	}


	template<class T>
	inline void Point2D<T>::operator () (const T& x1, const T& y1)
	{
		x = x1;
		y = y1;
	}


	template<class T>
	template<class U>
	inline void Point2D<T>::operator () (const Point2D<U>& p)
	{
		x = (T) p.x;
		y = (T) p.y;
	}


	template<class T>
	template<class U>
	inline Point2D<T>& Point2D<T>::operator += (const U& k)
	{
		x += (T)k;
		y += (T)k;
		return (*this);
	}


	template<class T>
	template<class U>
	inline Point2D<T>& Point2D<T>::operator += (const Point2D<U>& p)
	{
		x += (T) p.x;
		y += (T) p.y;
		return (*this);
	}


	template<class T>
	template<class U>
	inline Point2D<T>& Point2D<T>::operator *= (const U& k)
	{
		x = (T)(x * k);
		y = (T)(y * k);
		return (*this);
	}


	template<class T>
	template<class U>
	inline bool Point2D<T>::operator == (const Point2D<U>& rhs) const
	{
		return ((T) rhs.x == x) and ((T) rhs.y == y);
	}


	template<class T>
	template<class U>
	inline bool Point2D<T>::operator != (const Point2D<U>& rhs) const
	{
		return !(*this == rhs);
	}


	template<class T>
	template<class U>
	inline Point2D<T>& Point2D<T>::operator = (const Point2D<U>& p)
	{
		x = (T) p.x;
		y = (T) p.y;
		return (*this);
	}


	template<class T>
	template<class U>
	inline bool Point2D<T>::operator < (const Point2D<U>& p) const
	{
		if (not Math::Equals(x, (T)p.x))
			return x < (T)p.x;
		return y < (T)p.y;
	}


	template<class T>
	inline T& Point2D<T>::operator [] (uint i)
	{
		switch (i)
		{
			case 0:
				return x;
			case 1:
				return y;
			default:
				assert(false and "Index out of bounds !");
		}
		return x;
	}


	template<class T>
	inline const T& Point2D<T>::operator [] (uint i) const
	{
		switch (i)
		{
			case 0:
				return x;
			case 1:
				return y;
			default:
				assert(false and "Index out of bounds !");
		}
		return x;
	}


	template<class T>
	template<class StreamT>
	inline StreamT& Point2D<T>::print(StreamT& out) const
	{
		out << '(' << x << ',' << y << ')';
		return out;
	}





} // namespace Yuni



template<class T>
inline std::ostream& operator << (std::ostream& out, const Yuni::Point2D<T>& p)
{
	return p.print(out);
}


template<class T, class U>
inline Yuni::Point2D<T> operator + (const Yuni::Point2D<T>& lhs, const U& rhs)
{
	return Yuni::Point2D<T>(lhs) += rhs;
}


template<class T, class U>
inline Yuni::Point2D<T> operator + (const U& lhs, const Yuni::Point2D<T>& rhs)
{
	return Yuni::Point2D<T>(rhs) += lhs;
}


template<class T, class U>
inline Yuni::Point2D<T> operator * (const Yuni::Point2D<T>& lhs, const U& rhs)
{
	return Yuni::Point2D<T>(lhs) *= rhs;
}


template<class T, class U>
inline Yuni::Point2D<T> operator * (const U& lhs, const Yuni::Point2D<T>& rhs)
{
	return Yuni::Point2D<T>(rhs) *= lhs;
}


#endif // __YUNI_CORE_MATH_GEOMETRY_POINT2D_HXX__
