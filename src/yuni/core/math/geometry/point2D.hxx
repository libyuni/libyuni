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
	inline Point2D<T>& Point2D<T>::operator += (const T& k)
	{
		x += k;
		y += k;
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
	inline bool Point2D<T>::operator < (const Point3D<U>& p) const
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


template<class T>
inline const Yuni::Point2D<T> operator + (const Yuni::Point2D<T>& lhs, const Yuni::Point2D<T>& rhs)
{
	return Yuni::Point2D<T>(lhs) += rhs;
}


#endif // __YUNI_CORE_MATH_GEOMETRY_POINT2D_HXX__
