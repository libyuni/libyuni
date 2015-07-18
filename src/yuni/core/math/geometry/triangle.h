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
#include "../../smartptr.h"
#include "edge.h"
#include "vector3D.h"



namespace Yuni
{

	/*!
	** \brief A triangle is a 3-sided polygon
	** \ingroup Gfx
	**
	** The triangle is the base element for all meshes
	** It knows its edges, and its vertices.
	*/
	class YUNI_DECL Triangle final
	{
	public:
		typedef SmartPtr<Triangle> Ptr;

	public:
		//! \name Constructors
		//@{
		Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3):
			pVertex1(v1), pVertex2(v2), pVertex3(v3), pNormal(NULL)
		{
			calculateEdges();
		}

		//! Copy constructor
		Triangle(const Triangle& t):
			pVertex1(t.vertex1()), pVertex2(t.vertex2()), pVertex3(t.vertex3()), pNormal(NULL)
		{
			calculateEdges();
		}

		//! Destructor
		~Triangle()
		{
			if (pNormal)
				delete pNormal;
		}

		//@}


		//! \name Accessors
		//@{
		const Vertex& vertex1() const { return pVertex1; }
		const Vertex& vertex2() const { return pVertex2; }
		const Vertex& vertex3() const { return pVertex3; }

		/*!
		** \brief Get the normal (unit) vector to this triangle
		*/
		const Vector3D<float>& normal()
		{
			if (!pNormal)
			{
				Vector3D<float> edge1(vertex1().position(), vertex2().position());
				Vector3D<float> edge2(vertex1().position(), vertex3().position());
				pNormal = new Vector3D<float>(Vector3D<float>::CrossProduct(edge1, edge2));
				pNormal->normalize();
			}
			return *pNormal;
		}

		//@}

		/*!
		** \brief Print the triangle
		**
		** \param[in,out] out An output stream
		** \return The output stream `out`
		*/
		std::ostream& print(std::ostream& out) const
		{
			out << "Triangle with vertices at:" << std::endl;
			out << pVertex1.position() << std::endl;
			out << pVertex2.position() << std::endl;
			out << pVertex3.position() << std::endl;
			return out;
		}

		/*!
		** \brief Generate the OpenGL code for this triangle
		**
		** \param[in,out] out An output stream
		** \return The output stream `out`
		*/
		std::ostream& glPrint(std::ostream& out) const
		{
			out << "glBegin(GL_TRIANGLES);" << std::endl;
			pVertex1.glPrint(out);
			pVertex2.glPrint(out);
			pVertex3.glPrint(out);
			out << "glEnd();" << std::endl;
			return out;
		}

	private:
		/*!
		** \brief Create the edges of the triangle given its vertices
		*/
		void calculateEdges()
		{
			pEdges.push_back(SmartPtr<Edge>(new Edge(pVertex1, pVertex2)));
			pEdges.push_back(SmartPtr<Edge>(new Edge(pVertex2, pVertex3)));
			pEdges.push_back(SmartPtr<Edge>(new Edge(pVertex3, pVertex1)));
		}

	private:
		typedef std::vector<SmartPtr<Edge> > EdgeList;

	private:
		//! Vertices
		Vertex pVertex1;
		Vertex pVertex2;
		Vertex pVertex3;

		//! Edges
		EdgeList pEdges;

		//! Surface normal
		Vector3D<float>* pNormal;

	}; // class Triangle




} // namespace Yuni
