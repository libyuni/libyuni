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
#ifndef __YUNI_CORE_MATH_GEOMETRY_RECT_2D_H__
# define __YUNI_CORE_MATH_GEOMETRY_RECT_2D_H__

# include "../../../yuni.h"
# include "point2D.h"


namespace Yuni
{

	/*!
	** \brief 2D-Rectangle
	*/
	template<class T>
	class YUNI_DECL Rect2D final
	{
	public:
		//! The original type T
		typedef T Type;
		//! The original type T
		typedef T value_type;
		//! Complete type for Rect2D
		typedef Rect2D<Type> Rect2DType;
		//! Point2D
		typedef Point2D<Type> PointType;

	public:
		//! \name Constructors
		//@{
		/*!
		** \brief Default constructor
		*/
		Rect2D();

		/*!
		** \brief Construct the rectangle to cover two 2D-points
		**
		** \param a The first point to cover
		** \param b The second point to cover
		*/
		template<class U, class V>
		Rect2D(const Point2D<U>& a, const Point2D<V>& b);

		/*!
		** \brief Constructor with given coordinates
		**
		** \param ax1 The top-left X-coordinate
		** \param ay1 The top-left Y-coordinate
		** \param ax2 The bottom-right X-coordinate
		** \param ay2 The bottom-right Y-coordinate
		*/
		template<class U, class V, class W, class X>
		Rect2D(const U ax1, const V ay1, const W ax2, const X ay2);

		/*!
		** \brief Construct the rectangle to cover two other rectangles
		**
		** \param a The first rectangle to cover
		** \param b The second rectangle to cover
		*/
		template<class U, class V>
		Rect2D(const Rect2D<U>& a, const Rect2D<V>& b);
		//@} // Constructors


		//! \name Reset values
		//@{
		/*!
		** \brief Reset all coordinates to zero
		** \return Always *this
		*/
		Rect2D<T>& reset();

		/*!
		** \brief Reset the rectangle coordinates to cover two 2D-points
		**
		** \param a The first point to cover
		** \param b The second point to cover
		** \return Always *this
		*/
		template<class U, class V>
		Rect2D<T>& reset(const Point2D<U>& a, const Point2D<V>& b);

		/*!
		** \brief Reset the rectangle coordinates to cover two 2D-points
		**
		** \param a The first X-coordinate
		** \param b The first Y-coordinate
		** \param c The second X-coordinate
		** \param d The second Y-coordinate
		** \return Always *this
		*/
		template<class U, class V, class W, class X>
		Rect2D<T>& reset(const U a, const V b, const W c, const X d);

		/*!
		** \brief Reset the rectangle coordinates to cover two other rectangles
		**
		** \param a The first rectangle to cover
		** \param b The second rectangle to cover
		** \return Always *this
		*/
		template<class U, class V>
		Rect2D<T>& reset(const Rect2D<U>& a, const Rect2D<V>& b);
		//@}

		/*!
		** \brief Get the equivalent Point2D for the top left
		*/
		PointType topLeft() const;
		/*!
		** \brief Get the equivalent Point2D for the bottom right
		*/
		PointType bottomRight() const;


		//! \name Measurement
		//@{
		/*!
		** \brief Get the surface occupied by the rectangle
		*/
		T size() const;
		/*!
		** \brief Get the width of the rectangle
		*/
		T width() const;
		/*!
		** \brief Get the height of the rectangle
		*/
		T height() const;
		//@}


		//! \name Move the entire rect
		//@{
		/*!
		** \brief Move the entire rect
		**
		** \param ax1 The new top left X-coordinate
		** \param ay1 The new top left Y-coordinate
		*/
		template<class U> void move(const U ax1, const U ay1);

		/*!
		** \brief Move the entire rect
		**
		** \param p The new top left coordinates
		*/
		template<class U> void move(const Point2D<U>& p);

		/*!
		** \brief Move the entire rect
		**
		** \param ax1 The new top left X-coordinate
		** \param ay1 The new top left Y-coordinate
		*/
		template<class U, class V> void operator () (const U& ax1, const V& ay1);

		/*!
		** \brief Move the entire rect
		** \param p The new top left coordinates
		*/
		template<class U> void operator () (const Point2D<U>& p);

		/*!
		** \brief Reset the rectangle coordinates to cover two 2D-points
		**
		** \param a The first point to cover
		** \param b The second point to cover
		*/
		template<class U, class V>
		void operator () (const Point2D<U>& a, const Point2D<V>& b);

		/*!
		** \brief Reset the rectangle coordinates to cover two other rectangles
		**
		** \param a The first rectangle to cover
		** \param b The second rectangle to cover
		*/
		template<class U, class V>
		void operator () (const Rect2D<U>& a, const Rect2D<V>& b);
		//@}


		//! \name Geometrical localisation
		//@{
		/*!
		** \brief Get if a point is contained inside the rect
		**
		** \param p The point
		** \return True if the point is contained inside the rect
		*/
		template<class U> bool contains(const Point2D<U>& p) const;
		/*!
		** \brief Get if a point is contained inside the rect
		**
		** \param nx The X-coordinate of the point
		** \param ny The Y-coordinate of the point
		** \return True if the point is contained inside the rect
		*/
		template<class U, class V> bool contains(const U nx, const V ny) const;

		/*!
		** \brief Get if another rectangle is contained inside this rectangle
		**
		** \param r The rect to test if it is inside the rect
		** \return True if the rect `r` is inside the rect `this`
		*/
		template<class U> bool contains(const Rect2D<U>& r) const;

		/*!
		** \brief Get if another rectangle collides with this rectangle
		*/
		template<class U> bool collidedWith(const Rect2D<U>& r) const;
		//@} Geometrical localisation


		//! \name Assignment
		//@{
		/*!
		** \brief Move the entire rect
		**
		** \param p The new top left coordinates
		** \return Always *this
		*/
		template<class U> Rect2D<T>& operator = (const Point2D<U>& p);

		/*!
		** \brief Assign new values for all coordinates from another rect
		**
		** \param p The new coordinates
		** \return Always *this
		*/
		template<class U> Rect2D<T>& operator = (const Rect2D<U>& p);
		//@}


		//! \name Stream printing
		//@{
		/*!
		** \brief Print the point
		*/
		std::ostream& print(std::ostream& out) const;
		//@}


		/*!
		** \brief Swap coordinates if needed to have x1/y1 at the top-left and
		** x2/y2 at the bottom-right
		*/
		void normalize();

	public:
		//! The top left X-coordinate
		T x1;
		//! The top left Y-coordinate
		T y1;
		//! The bottom right X-coordinate
		T x2;
		//! The bottom right Y-coordinate
		T y2;

	}; // class Rect2D





} // namespace Yuni

# include "rect2D.hxx"



//! name Operator overload for stream printing
//@{
template<class T>
inline std::ostream& operator << (std::ostream& out, const Yuni::Rect2D<T>& p)
{ return p.print(out); }
//@}


#endif // __YUNI_CORE_MATH_GEOMETRY_RECT_2D_H__
