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
#include "../../../yuni.h"
#include "point3D.h"
#include "triangle.h"



namespace Yuni
{

	/*!
	** \brief A bounding box in 3D that grows with what is added inside
	** \ingroup Gfx
	*/
	template <class T = float>
	class YUNI_DECL BoundingBox final
	{
	public:
		//! \name Constructors and destructor
		//@{
		//! Default constructor
		BoundingBox();
		//! Constructor
		BoundingBox(const Point3D<T>& min, const Point3D<T>& max);
		//@}

		/*!
		** \brief Get the minimum coordinates of the box
		*/
		const Point3D<T>& min() const {return pMin;}

		/*!
		** \brief Get the maximum coordinates of the box
		*/
		const Point3D<T>& max() const {return pMax;}

		/*!
		** \brief Get the center of the box
		*/
		const Point3D<T>& center() const {return pCenter;}

		/*!
		** \brief Add a point that can possibly grow the bounding box
		*/
		void addPoint(const Point3D<T>& point);

		/*!
		** \brief Add a triangle that can possibly grow the bounding box
		*/
		void addTriangle(const Triangle& tri);

		/*!
		** \brief Is the point inside the bounding box?
		*/
		bool contains(const Point3D<T>& point) const;

		/*!
		** \brief Reset the bounding box to 0 size
		**
		** \param newCenter Reset the bounding box to a given position
		*/
		template<class U> void reset(Point3D<U> newCenter = Point3D<U>());


	private:
		//! Minimum X,Y,Z coordinates of the box
		Point3D<T> pMin;
		//! Maximum X,Y,Z coordinates of the box
		Point3D<T> pMax;
		//! Center of the box
		Point3D<T> pCenter;

	}; // class BoundingBox





} // namespace Yuni

#include "boundingbox.hxx"
