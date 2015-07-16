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

#ifndef __YUNI_CORE_MATH_GEOMETRY_H__
# define __YUNI_CORE_MATH_GEOMETRY_H__

# include "geometry/point3D.h"
# include "geometry/vector3D.h"

namespace Yuni
{
namespace Geometry
{

	/*!
	** \brief Intersection point of a line and a plane
	**
	** \param linePoint Any point on the line
	** \param lineDirection Direction vector of the line
	** \param planePoint Any point in the plane
	** \param planeNormal Normal vector of the plane
	*/
	template<typename T>
	static Point3D<T> LinePlaneIntersection(const Point3D<T>& linePoint,
		const Vector3D<T>& lineDirection, const Point3D<T>& planePoint,
		const Vector3D<T>& planeNormal);


	/*!
	** \brief Does a line segment intersect a plane ?
	**
	** \param startPoint Start of the segment
	** \param endPoint End of the segment
	** \param planePoint Any point in the plane
	** \param planeNormal Normal vector of the plane
	*/
	template<typename T>
	static bool SegmentIntersectsPlane(const Point3D<T>& startPoint, const Point3D<T>& endPoint,
		const Point3D<T>& planePoint, const Vector3D<T>& planeNormal);


	/*!
	** \brief Does a line segment intersect a quadrangle ?
	**
	** \param segmentStart Start of the segment
	** \param segmentEnd End of the segment
	** \param p1 First point of the quad
	** \param p2 Second point of the quad
	** \param p3 Third point of the quad
	*/
	template<typename T>
	static bool SegmentIntersectsTriangle(const Point3D<T>& segmentStart, const Point3D<T>& segmentEnd,
		const Point3D<T>& p1, const Point3D<T>& p2, const Point3D<T>& p3);

	/*!
	** \brief Does a line segment intersect a quadrangle ?
	**
	** \param segmentStart Start of the segment
	** \param segmentEnd End of the segment
	** \param p1 First point of the quad
	** \param p2 Second point of the quad
	** \param p3 Third point of the quad
	** \param p4 Fourth point of the quad
	*/
	template<typename T>
	static bool SegmentIntersectsQuad(const Point3D<T>& segmentStart, const Point3D<T>& segmentEnd,
		const Point3D<T>& p1, const Point3D<T>& p2,
		const Point3D<T>& p3, const Point3D<T>& p4);

	/*!
	** \brief Does a line segment intersect a sphere ?
	**
	** \param startPoint Start of the segment
	** \param endPoint End of the segment
	** \param center Center of the sphere
	** \param radius Radius of the sphere
	*/
	template<typename T>
	static bool SegmentIntersectsSphere(const Point3D<T>& startPoint, const Point3D<T>& endPoint,
		const Point3D<T>& center, T radius);

	/*!
	** \brief Projection point of a point on a line
	**
	** \param point The point to project
	** \param linePoint Any point in the line
	** \param lineDirection Direction vector of the line
	*/
	template<typename T>
	static Point3D<T> PointToLineProjection(const Point3D<T>& point,
		const Point3D<T>& linePoint, const Vector3D<T>& lineDirection);


	/*!
	** \brief Projection point of a point on a plane
	**
	** \param point The point to project
	** \param planePoint Any point in the plane
	** \param planeNormal Normal vector of the plane
	*/
	template<typename T>
	static Point3D<T> PointToPlaneProjection(const Point3D<T>& point,
		const Point3D<T>& planePoint, const Vector3D<T>& planeNormal);


	/*!
	** \brief Angle between two vectors
	*/
	template<typename T>
	static T VectorAngle(const Vector3D<T>& start, const Vector3D<T>& destination);


} // namespace Geometry
} // namespace Yuni

# include "geometry.hxx"

#endif // __YUNI_CORE_MATH_GEOMETRY_H__
