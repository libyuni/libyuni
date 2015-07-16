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
#ifndef __YUNI_CORE_MATH_GEOMETRY_VECTOR3D_HXX__
# define __YUNI_CORE_MATH_GEOMETRY_VECTOR3D_HXX__


namespace Yuni
{


	template<typename T>
	inline Vector3D<T>
	Vector3D<T>::Mean(const Vector3D<T>& p1, const Vector3D<T>& p2)
	{
		return Vector3D<T>().mean(p1, p2);
	}


	template<typename T>
	inline Vector3D<T>::Vector3D():
		x(), y(), z()
	{}


	template<typename T>
	inline Vector3D<T>::Vector3D(const Vector3D<T>& rhs):
		x(rhs.x), y(rhs.y), z(rhs.z)
	{}


	template<typename T>
	template<typename U>
	inline Vector3D<T>::Vector3D(const Vector3D<U>& rhs):
		x(static_cast<T>(rhs.x)),
		y(static_cast<T>(rhs.y)),
		z(static_cast<T>(rhs.z))
	{}


	template<typename T>
	template<typename U, typename V, typename W>
	inline Vector3D<T>::Vector3D(const U& x1, const V& y1, const W& z1):
		x(static_cast<T>(x1)),
		y(static_cast<T>(y1)),
		z(static_cast<T>(z1))
	{}


	template<typename T>
	template<typename U, typename V>
	inline Vector3D<T>::Vector3D(const Point3D<U>& origin, const Point3D<V>& end):
		x(static_cast<T>((end.x - origin.x))),
		y(static_cast<T>((end.y - origin.y))),
		z(static_cast<T>((end.z - origin.z)))
	{}


	template<typename T>
	inline Vector3D<T>& Vector3D<T>::clear()
	{
		x = T();
		y = T();
		z = T();
		return *this;
	}


	template<typename T>
	template<typename U>
	inline void Vector3D<T>::translate(const U v)
	{
		x += static_cast<T>(v);
		y += static_cast<T>(v);
		z += static_cast<T>(v);
	}


	template<typename T>
	template<typename U, typename V, typename W>
	inline void Vector3D<T>::translate(const U x1, const V y1, const W z1)
	{
		x += static_cast<T>(x1);
		y += static_cast<T>(y1);
		z += static_cast<T>(z1);
	}


	template<typename T>
	template<typename U>
	inline void Vector3D<T>::translate(const Vector3D<U>& v)
	{
		x += static_cast<T>(v.x);
		y += static_cast<T>(v.y);
		z += static_cast<T>(v.z);
	}


	template<typename T>
	template<typename U>
	inline Vector3D<T>& Vector3D<T>::mean(const Vector3D<U>& p)
	{
		x = static_cast<T>((x + static_cast<T>(p.x)) / 2.);
		y = static_cast<T>((y + static_cast<T>(p.y)) / 2.);
		z = static_cast<T>((z + static_cast<T>(p.z)) / 2.);
		return *this;
	}


	template<typename T>
	template<typename U, typename V>
	inline Vector3D<T>& Vector3D<T>::mean(const Vector3D<U>& p1, const Vector3D<V>& p2)
	{
		x = static_cast<T>(((p1.x + p2.x) / 2.0f));
		y = static_cast<T>(((p1.y + p2.y) / 2.0f));
		z = static_cast<T>(((p1.z + p2.z) / 2.0f));
		return *this;
	}



	template<typename T>
	template<typename U, typename V, typename W>
	inline void Vector3D<T>::operator () (const U x1, const V y1, const W z1)
	{
		x = static_cast<T>(x1);
		y = static_cast<T>(y1);
		z = static_cast<T>(z1);
	}


	template<typename T>
	template<typename U>
	inline void Vector3D<T>::operator () (const Vector3D<U>& v)
	{
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
		z = static_cast<T>(v.z);
	}


	template<typename T>
	template<typename U, typename V>
	inline void Vector3D<T>::operator () (const Point3D<U>& origin, const Point3D<V>& end)
	{
		x = static_cast<T>((end.x - origin.x));
		y = static_cast<T>((end.y - origin.y));
		z = static_cast<T>((end.z - origin.z));
	}


	template<typename T>
	inline std::ostream& Vector3D<T>::print(std::ostream& out) const
	{
		out << x << "," << y << "," << z;
		return out;
	}


	template<typename T>
	inline bool Vector3D<T>::null() const
	{
		return Math::Zero(x) and Math::Zero(y) and Math::Zero(z);
	}


	template<typename T>
	inline bool Vector3D<T>::unit() const
	{
		return Math::Zero(squareMagnitude() - T(1));
	}


	template<typename T>
	Vector3D<T>& Vector3D<T>::normalize()
	{
		T m = magnitude();
		if (!Math::Zero(m))
		{
			x /= m;
			y /= m;
			z /= m;
		}
		return *this;
	}

	template<typename T>
	Vector3D<T>& Vector3D<T>::normalize(const T coeff)
	{
		T m = magnitude();
		if (!Math::Zero(m))
		{
			m = coeff / m;
			x *= m;
			y *= m;
			z *= m;
		}
		return *this;
	}




	template<typename T>
	inline T Vector3D<T>::magnitude() const
	{
		return Math::SquareRoot(x*x + y*y + z*z);
	}


	template<typename T>
	inline T Vector3D<T>::squareMagnitude() const
	{
		return x*x + y*y + z*z;
	}


	template<typename T>
	inline T Vector3D<T>::Magnitude(const Vector3D& p1, const Vector3D& p2)
	{
		return Math::SquareRoot(p1.x*p2.x + p1.y*p2.y + p1.z*p2.z /* dot product */);
	}


	template<typename T>
	inline T Vector3D<T>::dotProduct(const Vector3D& p1) const
	{
		return p1.x*x + p1.y*y + p1.z*z;
	}


	template<typename T>
	inline T Vector3D<T>::DotProduct(const Vector3D& p1, const Vector3D& p2)
	{
		return p1.x*p2.x + p1.y*p2.y + p1.z*p2.z;
	}


	template<typename T>
	inline Vector3D<T> Vector3D<T>::CrossProduct(const Vector3D<T>& p1, const Vector3D<T>& p2)
	{
		return Vector3D
			(p1.y * p2.z - p1.z * p2.y,
			p1.z * p2.x - p1.x * p2.z,
			p1.x * p2.y - p1.y * p2.x);
	}


	template<typename T>
	template<class U, class V, class W>
	inline Vector3D<T> Vector3D<T>::CrossProduct(const Point3D<U>& p1, const Point3D<V>& p2, const Point3D<W>& p3)
	{
		return CrossProduct(Vector3D(p1, p2), Vector3D(p1, p3));
	}


	template<typename T>
	inline bool Vector3D<T>::AreColinear(const Vector3D& p1, const Vector3D& p2, bool ignoreDirection)
	{
		T value = DotProduct(p1, p2) / (p1.magnitude() * p2.magnitude());
		bool colinear = (value > (1. - YUNI_EPSILON));
		if (ignoreDirection)
			colinear = colinear or (value < YUNI_EPSILON - 1.);
		return colinear;
	}


	template<typename T>
	inline T Vector3D<T>::Angle(const Vector3D<T>& p1, const Vector3D<T>& p2)
	{
		// By definition: cos(alpha) = (p1.p2)/(|p1|.|p2|)
		return static_cast<T>(Math::ACos(DotProduct(p1, p2) / (p1.magnitude() * p2.magnitude())));
	}


	template<class T>
	template<class R>
	inline void Vector3D<T>::Angle(R& out, const Vector3D<T>& p1, const Vector3D<T>& p2)
	{
		// By definition: cos(alpha) = (p1.p2)/(|p1|.|p2|)
		out = static_cast<R>(Math::ACos(DotProduct(p1, p2) / (p1.magnitude() * p2.magnitude())));
	}




	template<typename T>
	inline Vector3D<T>& Vector3D<T>::operator += (const T k)
	{
		x += k;
		y += k;
		z += k;
		return (*this);
	}

	template<typename T>
	template<typename U>
	inline Vector3D<T>& Vector3D<T>::operator += (const Vector3D<U>& p)
	{
		x += static_cast<T>(p.x);
		y += static_cast<T>(p.y);
		z += static_cast<T>(p.z);
		return (*this);
	}

	template<typename T>
	inline Vector3D<T>& Vector3D<T>::operator -= (const T k)
	{
		x -= k;
		y -= k;
		z -= k;
		return (*this);
	}

	template<typename T>
	template<typename U>
	inline Vector3D<T>& Vector3D<T>::operator -= (const Vector3D<U>& p)
	{
		x -= static_cast<T>(p.x);
		y -= static_cast<T>(p.y);
		z -= static_cast<T>(p.z);
		return (*this);
	}


	template<typename T>
	inline Vector3D<T>& Vector3D<T>::operator *= (const T k)
	{
		x *= k;
		y *= k;
		z *= k;
		return (*this);
	}

	template<typename T>
	inline Vector3D<T>& Vector3D<T>::operator /= (const T k)
	{
		if (!Math::Zero(k))
		{
			x /= k;
			y /= k;
			z /= k;
		}
		return (*this);
	}


	template<typename T>
	template<typename U>
	inline Vector3D<T>& Vector3D<T>::operator *= (const Vector3D<U>& p)
	{
		x -= static_cast<T>(p.x);
		y -= static_cast<T>(p.y);
		z -= static_cast<T>(p.z);
		return (*this);
	}

	template<typename T>
	template<typename U>
	inline Vector3D<T>& Vector3D<T>::operator /= (const Vector3D<U>& p)
	{
		x -= static_cast<T>(p.x);
		y -= static_cast<T>(p.y);
		z -= static_cast<T>(p.z);
		return (*this);
	}


	template<class T>
	template<class U>
	inline bool Vector3D<T>::operator == (const Vector3D<U>& rhs) const
	{
		return Math::Equals((T)rhs.x, x)
			&& Math::Equals((T)rhs.y, y)
			&& Math::Equals((T)rhs.z, z);
	}


	template<class T>
	template<class U>
	inline bool Vector3D<T>::operator != (const Vector3D<U>& rhs) const
	{
		return !(*this == rhs);
	}


	template<class T>
	template<class U>
	inline Vector3D<T>& Vector3D<T>::operator = (const Vector3D<U>& p)
	{
		x = (T)p.x;
		y = (T)p.y;
		z = (T)p.z;
		return (*this);
	}





} // namespace Yuni

#endif // __YUNI_CORE_MATH_GEOMETRY_VECTOR3D_HXX__
