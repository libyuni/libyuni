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
#ifndef __YUNI_CORE_MATH_GEOMETRY_QUATERNION_HXX__
# define __YUNI_CORE_MATH_GEOMETRY_QUATERNION_HXX__

# include "../math.h"


namespace Yuni
{


	template<class T>
	inline T Quaternion<T>::Magnitude(const Quaternion& q)
	{
		return Math::SquareRoot(q.w * q.w + q.v.x * q.v.x + q.v.y * q.v.y + q.v.z * q.v.z);
	}


	template<class T>
	inline T Quaternion<T>::SquareMagnitude(const Quaternion& q)
	{
		return q.w * q.w + q.v.x * q.v.x + q.v.y * q.v.y + q.v.z * q.v.z;
	}


	template<class T>
	template<class U>
	inline Point3D<T> Quaternion<T>::Rotate(const Point3D<T>& p, const Point3D<T>& origin,
		const Vector3D<U>& axis, T angle)
	{
		// The view quaternion is [0, v]
		Quaternion<T> view(0, p.x - origin.x, p.y - origin.y, p.z - origin.z);
		if (!view.unit())
			view.normalize();
		Vector3D<U> axis2(axis);
		if (!axis2.unit())
			axis2.normalize();
		T sinA = Math::Sin(angle / (T)2);
		Quaternion<T> rot(Math::Cos(angle / (T)2), axis2.x * sinA, axis2.y * sinA, axis2.z * sinA);
		if (!rot.unit())
			rot.normalize();
		Vector3D<T> result = ((rot * view) * rot.conjugate()).v;
		return Point3D<T>(result.x + origin.x, result.y + origin.y, result.z + origin.z);
	}


	template<class T>
	template<class U>
	inline Vector3D<T> Quaternion<T>::Rotate(const Vector3D<T>& v, const Vector3D<U>& axis, T angle)
	{
		// The view quaternion is [0, v]
		Quaternion<T> view(0, v);
		if (!view.unit())
			view.normalize();
		Vector3D<U> axis2(axis);
		if (!axis2.unit())
			axis2.normalize();
		T sinA = Math::Sin(angle / (T)2);
		Quaternion<T> rot(Math::Cos(angle / (T)2), axis2.x * sinA, axis2.y * sinA, axis2.z * sinA);
		if (!rot.unit())
			rot.normalize();
		return ((rot * view) * rot.conjugate()).v;
	}



	template<class T>
	inline Quaternion<T>::Quaternion():
		w(1),
		v()
	{}


	template<class T>
	inline Quaternion<T>::Quaternion(T newW, T x, T y, T z):
		w(newW),
		v(x, y, z)
	{}


	template<class T>
	inline Quaternion<T>::Quaternion(T newW, const Vector3D<T>& newV):
		w(newW),
		v(newV)
	{}


	template<class T>
	template<class U>
	inline Quaternion<T>::Quaternion(const Quaternion<U>& other):
		w((T)other.w),
		v(other.v)
	{}


	template<class T>
	inline Quaternion<T>& Quaternion<T>::clear()
	{
		w = T(1);
		v.clear();
		return *this;
	}


	template<class T>
	inline T Quaternion<T>::magnitude() const
	{
		return Magnitude(*this);
	}


	template<class T>
	inline bool Quaternion<T>::null() const
	{
		return Math::Zero(w) && v.null();
	}


	template<class T>
	inline bool Quaternion<T>::unit() const
	{
		return Math::Zero(SquareMagnitude(*this) - T(1));
	}


	template<class T>
	inline Quaternion<T> Quaternion<T>::conjugate() const
	{
		return Quaternion<T>(w, -v.x, -v.y, -v.z);
	}


	template<class T>
	inline void Quaternion<T>::normalize()
	{
		T mag = magnitude();
		w /= mag;
		v.x /= mag;
		v.y /= mag;
		v.z /= mag;
	}


	template<class T>
	template<class U>
	inline Quaternion<T>& Quaternion<T>::operator = (const Quaternion<U>& other)
	{
		w = static_cast<T>(other.w);
		v = other.v;
	}


	template<class T>
	template<class U>
	inline Quaternion<T>& Quaternion<T>::operator += (const Quaternion<U>& other)
	{
		w += static_cast<T>(other.w);
		v.x += static_cast<T>(other.v.x);
		v.y += static_cast<T>(other.v.y);
		v.z += static_cast<T>(other.v.z);
		return *this;
	}


	template<class T>
	template<class U>
	inline Quaternion<T>& Quaternion<T>::operator *= (const Quaternion<U>& other)
	{
		T newW = other.w * w - other.v.x * v.x - other.v.y * v.y - other.v.z * v.z;
		T newX = other.v.x * w + other.w * v.x - other.v.z * v.y + other.v.y * v.z;
		T newY = other.v.y * w + other.v.z * v.x + other.w * v.y - other.v.x * v.z;
		T newZ = other.v.z * w - other.v.y * v.x + other.v.x * v.y + other.w * v.z;
		w = newW;
		v.x = newX;
		v.y = newY;
		v.z = newZ;
		return *this;
	}


	template<class T>
	inline std::ostream& Quaternion<T>::print(std::ostream& out) const
	{
		out << '[' << w << ", (" << v.x << ',' << v.y << ',' << v.z << ")]";
		return out;
	}



} // namespace Yuni

#endif // __YUNI_CORE_MATH_GEOMETRY_QUATERNION_HXX__

