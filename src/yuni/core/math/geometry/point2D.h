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
#include "../math.h"



namespace Yuni
{


	/*!
	** \brief Represents a 2D-point
	*/
	template<class T = float>
	class YUNI_DECL Point2D /* final */
	{
	public:
		/*!
		** \brief Calculate the mean between two points
		**
		** \param p1 First point
		** \param p2 Second point
		** \return A new instance of Point3D
		*/
		static Point2D<T> Mean(const Point2D<T>& p1, const Point2D<T>& p2);


	public:
		//! \name Constructors & Destructor
		//{
		//! Default constructor
		Point2D();
		/*!
		** \brief Constructor
		** \param x1 The default x coordinate
		** \param y1 The default y coordinate
		*/
		template<class U, class V>
		Point2D(const U x1, const V y1);
		//! Constructor by copy
		template<class U>
		Point2D(const Point2D<U>& p);
		//}

		//! Reset the point to origin
		Point2D<T>& reset();


		/*!
		** \brief Move the point to new coordinates
		**
		** \param x1 The new X coordinate
		** \param y1 The new Y coordinate
		*/
		void move(const T& x1, const T& y1);
		/*!
		** \brief Move the point to new coordinates
		** \param p The new coordinates
		*/
		template<class U> void move(const Point2D<U>& p);


		/*!
		** \brief Translate the point with the same value for all coordinates
		**
		** \param k The value to add to all coordinates
		*/
		void translate(const T& k);

		/*!
		** \brief Translate the point with relative coordinates
		**
		** \param x1 The value to add to the x coordinate
		** \param y1 The value to add to the y coordinate
		*/
		void translate(const T& x1, const T& y1);

		/*!
		** \brief Translate the point with relative coordinates from another Point
		** \param p The values to add to the coordinates
		*/
		template<class U> void translate(const Point2D<U>& p);

		/*!
		** \brief Calculate the mean between two points
		**
		** The calling object is modified to store the value
		**
		** \param p Point to compute the mean with
		*/
		template<class U> void mean(const Point2D<U>& p);

		/*!
		** \brief Calculate the mean between two points
		**
		** The calling object is modified to store the value
		**
		** \param p1 Point to compute the mean with
		** \param p2 Second point to compute the mean with
		** \return Always *this
		*/
		template<class U, class V>
		Point2D<T>& mean(const Point2D<U>& p1, const Point2D<V>& p2);


		/*!
		** \brief Get if the point is close to another point
		**
		** \param rhs The other point
		** \param delta Delta value
		*/
		template<class U> bool isCloseTo(const Point2D<U>& rhs, const T& delta) const;

		/*!
		** \brief Get if the point is close to another point
		**
		** \param x1 The X coordinate of the other point
		** \param y1 The Y coordinate of the other point
		** \param delta Delta value
		*/
		bool isCloseTo(const T& x1, const T& y1, const T& delta) const;



		//! \name Operators
		//{
		/*!
		** \brief Reset all coordinates
		**
		** \param x1 The new value for the x coordinate
		** \param y1 The new value for the y coordinate
		** \see move()
		*/
		void operator () (const T& x1, const T& y1);

		/*!
		** \brief Copy all coordinates from another point
		** \param p The coordinates to copy
		** \see move()
		*/
		template<class U> void operator () (const Point2D<U>& p);


		//! Translate the point with the same value for all coordinates
		template<class U> Point2D<T>& operator += (const U& k);

		//! Translate the point with relative coordinates
		template<class U> Point2D<T>& operator += (const Point2D<U>& p);

		//! Apply ratio to the point with the same value for all coordinates
		template<class U> Point2D<T>& operator *= (const U& k);

		/*!
		** \brief Comparison operator (equal with)
		**
		** \param rhs The other point to compare with
		** \return True if the two points are equal
		*/
		template<class U> bool operator == (const Point2D<U>& rhs) const;

		/*!
		** \brief Comparison operator (non equal with)
		**
		** \param rhs The other point to compare with
		** \return True if the two points are not equal
		*/
		template<class U> bool operator != (const Point2D<U>& rhs) const;

		/*!
		** \brief Assign new values for all coordinates from another point
		**
		** \param p The new coordinates
		** \return Always *this
		**
		** \see move()
		*/
		template<class U>
		Point2D<T>& operator = (const Point2D<U>& p);

		/*!
		** \brief Check if a point's coordinates are < to another's
		**
		** \param p The point to compare with
		** \return true if this < p, false otherwise
		**
		** \remarks X is compared first, then if necessary Y, then Z.
		*/
		template<class U> bool operator < (const Point2D<U>& p) const;

		//! Operator [] overload.
		T& operator [] (uint i);
		//! Const Operator [] overload.
		const T& operator [] (uint i) const;
		//} Operators


		/*!
		** \brief Print the point
		**
		** \param[in,out] out An output stream
		** \return The output stream `out`
		*/
		template<class StreamT> StreamT& print(StreamT& out) const;


	public:
		//! X coordinate
		T x;
		//! Y coordinate
		T y;

	}; // class Point2D




} // namespace Yuni


#include "point2D.hxx"

//! name Operator overload for stream printing
//@{
template<class T>
inline std::ostream& operator << (std::ostream& out, const Yuni::Point2D<T>& p);

template<class T, class U>
inline Yuni::Point2D<T> operator + (const Yuni::Point2D<T>& lhs, const U& rhs);

template<class T, class U>
inline Yuni::Point2D<T> operator + (const U& lhs, const Yuni::Point2D<T>& rhs);

template<class T, class U>
inline Yuni::Point2D<T> operator * (const Yuni::Point2D<T>& lhs, const U& rhs);

template<class T, class U>
inline Yuni::Point2D<T> operator * (const U& lhs, const Yuni::Point2D<T>& rhs);
//@}
