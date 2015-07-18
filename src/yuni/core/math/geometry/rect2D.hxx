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
#include "rect2D.h"


namespace Yuni
{

	template<class T>
	inline Rect2D<T>::Rect2D()
		:x1(), y1(), x2(), y2()
	{}


	template<class T>
	template<class U, class V>
	inline Rect2D<T>::Rect2D(const Point2D<U>& a, const Point2D<V>& b)
	{
		reset(a, b);
	}


	template<class T>
	template<class U, class V, class W, class X>
	inline Rect2D<T>::Rect2D(const U ax1, const V ay1, const W ax2, const X ay2)
		:x1((T)ax1), y1((T)ay1), x2((T)ax2), y2((T)ay2)
	{}


	template<class T>
	template<class U, class V>
	inline Rect2D<T>::Rect2D(const Rect2D<U>& a, const Rect2D<V>& b)
	{
		reset(a, b);
	}



	template<class T>
	inline Rect2D<T>& Rect2D<T>::reset()
	{
		// Reset all values to zero
		x1 = y1 = x2 = y2 = T();
		return *this;
	}


	template<class T>
	template<class U, class V>
	Rect2D<T>& Rect2D<T>::reset(const Point2D<U>& a, const Point2D<V>& b)
	{
		x1 = Math::Min( (T) a.x, (T) b.x);
		y1 = Math::Min( (T) a.y, (T) b.y);
		x2 = Math::Max( (T) a.x, (T) b.x);
		y2 = Math::Max( (T) a.y, (T) b.y);
		return *this;
	}


	template<class T>
	template<class U, class V, class W, class X>
	Rect2D<T>& Rect2D<T>::reset(const U a, const V b, const W c, const X d)
	{
		x1 = Math::Min( (T) a, (T) c);
		y1 = Math::Min( (T) b, (T) d);
		x2 = Math::Max( (T) a, (T) c);
		y2 = Math::Max( (T) b, (T) d);
		return *this;
	}


	template<class T>
	template<class U, class V>
	Rect2D<T>& Rect2D<T>::reset(const Rect2D<U>& a, const Rect2D<V>& b)
	{
		x1 = Math::Min( (T) Math::Min(a.x1, a.x2), (T) Math::Min(b.x1, b.y1) );
		y1 = Math::Min( (T) Math::Min(a.y1, a.y2), (T) Math::Min(b.y1, b.y1) );
		x2 = Math::Max( (T) Math::Max(a.x1, a.x2), (T) Math::Max(b.x1, b.y1) );
		y2 = Math::Max( (T) Math::Max(a.y1, a.y2), (T) Math::Max(b.y1, b.y1) );
		return *this;
	}


	template<class T>
	inline T Rect2D<T>::size() const
	{
		return Math::Abs(x2 - x1) * Math::Abs(y2 - y1);
	}


	template<class T>
	inline T Rect2D<T>::width() const
	{
		return Math::Abs(x2 - x1);
	}


	template<class T>
	inline T Rect2D<T>::height() const
	{
		return Math::Abs(y2 - y1);
	}


	template<class T>
	inline typename Rect2D<T>::PointType Rect2D<T>::topLeft() const
	{
		return Point2DType(x1, y1);
	}


	template<class T>
	inline typename Rect2D<T>::PointType Rect2D<T>::bottomRight() const
	{
		return Point2DType(x2, y2);
	}


	template<class T>
	void Rect2D<T>::normalize()
	{
		if (x2 < x1)
			Math::Swap(x1, x2);
		if (y2 < y1)
			Math::Swap(y1, y2);
	}


	template<class T>
	inline std::ostream& Rect2D<T>::print(std::ostream& out) const
	{
		return (out << x1 << "," << y1 << ":" << x2 << "," << y2);
	}


	template<class T>
	template<class U>
	inline void Rect2D<T>::move(const U ax1, const U ay1)
	{
		x2 = (T)ax1 + (x2 - x1);
		y2 = (T)ay1 + (y2 - y1);
		x1 = (T)ax1;
		y1 = (T)ay1;
	}


	template<class T>
	template<class U>
	inline void Rect2D<T>::move(const Point2D<U>& p)
	{
		x2 = (T)p.x + (x2 - x1);
		y2 = (T)p.y + (y2 - y1);
		x1 = (T)p.x;
		y1 = (T)p.y;
	}


	template<class T>
	template<class U>
	inline void Rect2D<T>::operator () (const Point2D<U>& p)
	{
		move(p);
	}


	template<class T>
	template<class U, class V>
	inline void Rect2D<T>::operator () (const U& ax1, const V& ay1)
	{
		move(ax1, ay1);
	}


	template<class T>
	template<class U, class V>
	inline void Rect2D<T>::operator () (const Point2D<U>& a, const Point2D<V>& b)
	{
		reset(a, b);
	}


	template<class T>
	template<class U, class V>
	inline void Rect2D<T>::operator () (const Rect2D<U>& a, const Rect2D<V>& b)
	{
		reset(a, b);
	}


	template<class T>
	template<class U>
	inline bool Rect2D<T>::contains(const Point2D<U>& p) const
	{
		return p.x >= x1 && p.y >= y1 && p.x <= x2 && p.y <= y2;
	}


	template<class T>
	template<class U, class V>
	inline bool Rect2D<T>::contains(const U nx, const V ny) const
	{
		return nx >= x1 && ny >= y1 && nx <= x2 && ny <= y2;
	}


	template<class T>
	template<class U>
	inline bool Rect2D<T>::contains(const Rect2D<U>& r) const
	{
		return (contains(r.x1, r.y1) && contains(r.x2, r.y2));
	}


	template<class T>
	template<class U>
	inline bool Rect2D<T>::collidedWith(const Rect2D<U>& r) const
	{
		return (contains(r.x1, r.y1) || contains(r.x2, r.y2));
	}


	template<typename T>
	template<class U>
	inline Rect2D<T>& Rect2D<T>::operator = (const Point2D<U>& p)
	{
		move(p);
		return *this;
	}


	template<typename T>
	template<class U>
	inline Rect2D<T>& Rect2D<T>::operator = (const Rect2D<U>& p)
	{
		x1 = (T)p.x1;
		y1 = (T)p.y1;
		x2 = (T)p.x2;
		y2 = (T)p.y2;
		return *this;
	}




} // namespace Yuni
