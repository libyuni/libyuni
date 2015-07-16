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

#include <cassert>

namespace Yuni
{
namespace Geometry
{


	template<typename T>
	inline Point3D<T> LinePlaneIntersection(const Point3D<T>& linePoint,
		const Vector3D<T>& lineDirection, const Point3D<T>& planePoint,
		const Vector3D<T>& planeNormal)
	{
		T dotProduct = Vector3D<T>::DotProduct(planeNormal, lineDirection);
		assert(Math::Abs(dotProduct) < YUNI_EPSILON);
		// Vector connecting the two origin points from line to plane
		Vector3D<T> lineToPlane(linePoint, planePoint);
		T factor = DotProduct(lineToPlane, planeNormal) / dotProduct;
		// Scale the direction by the found value
		Vector3D<T> direction(lineDirection);
		direction *= factor;
		// Move the line point along the line to the intersection
		Point3D<T> result(linePoint);
		result.translate(direction.x, direction.y, direction.z);
		return result;
	}


	template<typename T>
	inline bool SegmentIntersectsPlane(const Point3D<T>& startPoint, const Point3D<T>& endPoint,
		const Point3D<T>& planePoint, const Vector3D<T>& planeNormal)
	{
		Vector3D<T> planeToStart(planePoint, startPoint);
		Vector3D<T> planeToEnd(planePoint, endPoint);

		T startDot = Vector3D<T>::DotProduct(planeToStart, planeNormal);
		T endDot = Vector3D<T>::DotProduct(planeToEnd, planeNormal);
		// The two ends of the segment are on different sides of the plane if :
		// 1. either of the dot products is null (one of the points is in the plane) OR
		// 2. the signs of the dot products are opposite.
		return Math::Zero(startDot) || Math::Zero(endDot) ||
			(startDot > 0) != (endDot > 0);
	}


	// http://geomalgorithms.com/a06-_intersect-2.html
	template<typename T>
	inline bool SegmentIntersectsTriangle(const Point3D<T>& segmentStart, const Point3D<T>& segmentEnd,
		const Point3D<T>& p1, const Point3D<T>& p2, const Point3D<T>& p3)
	{
		Vector3D<T> u(p1, p2);
		Vector3D<T> v(p1, p3);
		Vector3D<T> normal = Vector3D<T>::CrossProduct(u, v);
		Vector3D<T> ray(segmentStart, segmentEnd);
		Vector3D<T> w0(p1, segmentStart);
		T a = -Vector3D<T>::DotProduct(normal, w0);
		T b = Vector3D<T>::DotProduct(normal, ray);
		if (Math::Zero(b))
			return a == 0;
		T r = a / b;
		if (r < 0 || r > 1)
			return false;
		Point3D<T> intersection(segmentStart);
		ray *= r;
		intersection.x += ray.x;
		intersection.y += ray.y;
		intersection.z += ray.z;

		T udotv = Vector3D<T>::DotProduct(u, v);
		T udotu = Vector3D<T>::DotProduct(u, u);
		T vdotv = Vector3D<T>::DotProduct(v, v);
		Vector3D<T> w(p1, intersection);
		T wdotu = Vector3D<T>::DotProduct(w, u);
		T wdotv = Vector3D<T>::DotProduct(w, v);
		T denominator = (udotv * udotv - udotu * vdotv);
		T s = (udotv * wdotv - vdotv * wdotu) / denominator;
		if (s < 0 || s > 1)
			return false;
		T t = (udotv * wdotu - udotu * wdotv) / denominator;
		if (t < 0 || s + t > 1)
			return false;
		return true;
	}


	template<typename T>
	inline bool SegmentIntersectsQuad(const Point3D<T>& segmentStart, const Point3D<T>& segmentEnd,
		const Point3D<T>& p1, const Point3D<T>& p2,
		const Point3D<T>& p3, const Point3D<T>& p4)
	{
		Vector3D<T> normal = Vector3D<T>::CrossProduct(p1, p2, p3);
		if (!SegmentIntersectsPlane(segmentStart, segmentEnd, p1, normal))
			return false;
		return SegmentIntersectsTriangle(segmentStart, segmentEnd, p1, p2, p3) ||
			SegmentIntersectsTriangle(segmentStart, segmentEnd, p1, p3, p4);
	}


	template<typename T>
	inline bool SegmentIntersectsSphere(const Point3D<T>& startPoint, const Point3D<T>& endPoint,
		const Point3D<T>& center, T radius)
	{
		Vector3D<T> lineDirection(startPoint, endPoint);
		// Project the center of the sphere on the line
		Point3D<T> projection = PointToLineProjection(center, startPoint, lineDirection);
		// Check point ordering along the line : projection must be between start and end
		if ((projection.x < startPoint.x and projection.x < endPoint.x) or
			(projection.x > startPoint.x and projection.x > endPoint.x))
			return false;
		if ((projection.y < startPoint.y and projection.y < endPoint.y) or
			(projection.y > startPoint.y and projection.y > endPoint.y))
			return false;
		if ((projection.z < startPoint.z and projection.z < endPoint.z) or
			(projection.z > startPoint.z and projection.z > endPoint.z))
			return false;
		// Now check that the projection is inside the sphere : OO' <= r
		return Vector3D<T>(center, projection).squareMagnitude() <= radius * radius;
	}


	template<typename T>
	inline Point3D<T> PointToLineProjection(const Point3D<T>& point,
		const Point3D<T>& linePoint, const Vector3D<T>& lineDirection)
	{
		Vector3D<T> translation(lineDirection);
		translation.normalize();
		T dot = Vector3D<T>::DotProduct(translation, Vector3D<T>(linePoint, point));
		translation *= dot;
		Point3D<T> projection(linePoint);
		projection.translate(translation.x, translation.y, translation.z);
		return projection;
	}


	template<typename T>
	inline Point3D<T> PointToPlaneProjection(const Point3D<T>& point,
		const Point3D<T>& planePoint, const Vector3D<T>& planeNormal)
	{
		// First project on a normal line to the plane
		Point3D<T> pn = PointToLineProjection(point, planePoint, planeNormal);
		// Get the vector from the projection to the point
		Vector3D<T> translation(pn, point);
		// Translate the plane origin by this vector
		Point3D<T> projection(planePoint);
		projection.translate(translation.x, translation.y, translation.z);
		return projection;
	}


	template<typename T>
	inline T VectorAngle(const Vector3D<T>& start, const Vector3D<T>& destination)
	{
		// TODO
		return T();
	}



} // namespace Geometry
} // namespace Yuni
