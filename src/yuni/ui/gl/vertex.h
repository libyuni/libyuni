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
#ifndef __YUNI_GFX3D_VERTEX_H__
# define __YUNI_GFX3D_VERTEX_H__

# include <yuni/yuni.h>
# include <yuni/core/smartptr.h>
# include <yuni/core/point3D.h>
# include <yuni/core/static/assert.h>
# include "vertexpolicies.h"
# include "vertexpadding.h"

namespace Yuni
{
namespace Gfx3D
{


	/*!
	** \brief Generic vertex for vertex buffer usage
	**
	** A vertex has to contain its position, and then may
	** optionally contain normal, color and texture information.
	*/
	template<
		template<class> class NorP = Policy::Normal::With,
		template<class> class ColP = Policy::Color::None,
		template<class> class TexP = Policy::TexCoord::UV,
		class UserP = Policy::User::None
	>
	class Vertex:
		public Yuni::Point3D<float>,
		public NorP<Vertex<NorP, ColP, TexP, UserP> >,
		public ColP<Vertex<NorP, ColP, TexP, UserP> >,
		public TexP<Vertex<NorP, ColP, TexP, UserP> >,
		public UserP,
		public VertexPadding<
			sizeof(Yuni::Point3D<float>) +
			NorP<Vertex<NorP, ColP, TexP, UserP> >::Size +
			ColP<Vertex<NorP, ColP, TexP, UserP> >::Size +
			TexP<Vertex<NorP, ColP, TexP, UserP> >::Size +
			UserP::Size>
	{
	public:
		//! Full type
		typedef Vertex<NorP, ColP, TexP, UserP>  VertexType;
		//! Normal policy
		typedef NorP<VertexType>  NormalPolicy;
		//! Color policy
		typedef ColP<VertexType>  ColorPolicy;
		//! Texture policy
		typedef TexP<VertexType>  TexCoordPolicy;
		//! User data policy
		typedef UserP  UserDataPolicy;
		//! Padding type
		typedef VertexPadding<
			sizeof(Yuni::Point3D<float>) +
			NorP<Vertex<NorP, ColP, TexP, UserP> >::Size +
			ColP<Vertex<NorP, ColP, TexP, UserP> >::Size +
			TexP<Vertex<NorP, ColP, TexP, UserP> >::Size +
			UserP::Size>  PaddingT;
		//! Smart pointer
		typedef SmartPtr<VertexType>  Ptr;

	public:
		enum // anonymous
		{
			//! Size in bytes of data without padding (not particularly useful except for debugging)
			DataSize = (sizeof(Yuni::Point3D<float>) +
				NorP<Vertex<NorP, ColP, TexP, UserP> >::Size +
				ColP<Vertex<NorP, ColP, TexP, UserP> >::Size +
				TexP<Vertex<NorP, ColP, TexP, UserP> >::Size +
				UserP::Size),

			// Features
			//! Does the vertex contain normals data ?
			HasNormal = NormalPolicy::HasNormal,
			//! Does the vertex contain color data ?
			HasColor = ColorPolicy::HasColor,
			//! Does the vertex contain texture data ?
			HasTexture = TexCoordPolicy::HasTexture,

			// Offsets in bytes
			//! Offset of the coordinates data in bytes
			PointOffset = 0,
			//! Offset of the normals data in bytes
			NormalOffset = PointOffset + sizeof(Yuni::Point3D<float>),
			//! Offset of the color data in bytes
			ColorOffset = NormalOffset + NormalPolicy::Size,
			//! Offset of the texture coordinates data in bytes
			TexCoordOffset = ColorOffset + ColorPolicy::Size,
			//! Offset of the user data in bytes
			UserOffset = TexCoordOffset + TexCoordPolicy::Size
		};

		//! For use in ShaderProgram::bindArgument()
		enum Attribute
		{
			//! Coordinates of the vertex in space
			vaPosition = 0,

			//! Vertex normal
			vaNormal = 1,

			//! Vertex color
			vaColor = 2,

			//! Texture coordinates
			vaTextureCoord = 3,

			//! Multi-texturing texture 0
			vaTexture0 = 0,

			//! Multi-texturing texture 1
			vaTexture1 = 1,

			//! Multi-texturing texture 2
			vaTexture2 = 2,

			//! Multi-texturing texture 3
			vaTexture3 = 3,

			//! Multi-texturing texture 4
			vaTexture4 = 4,

			//! Multi-texturing texture 5
			vaTexture5 = 5,

			//! Multi-texturing texture 6
			vaTexture6 = 6,

			//! Multi-texturing texture 7
			vaTexture7 = 7,

			//! User index, all following indices are free, vaUser + 1, vaUser + 2, ...
			vaUser = 12

		}; // enum Attribute


	public:
		//! Empty constructor, default values
		Vertex();

		//! Constructor with coordinates
		template<class U, class V, class W>
		Vertex(U x, V y, W z);

		//! Copy constructor
		Vertex(const Vertex&);

		//! Assignment operator
		Vertex& operator = (const Vertex& v);

		//! Lesser than comparison operator (for std::map::find)
		bool operator < (const Vertex& p) const;

		//! Position of the vertex in space
		Yuni::Point3D<float>& position();
		//! Position of the vertex in space
		const Yuni::Point3D<float>& position() const;

		//! The normal vector at the vertex
		NormalPolicy& normal();
		//! The normal vector at the vertex
		const NormalPolicy& normal() const;

		//! The vertex color
		ColorPolicy& color();
		//! The vertex color
		const ColorPolicy& color() const;

		//! The UV texture coordinates on given vertex
		TexCoordPolicy& texCoord();
		//! The UV texture coordinates on given vertex
		const TexCoordPolicy& texCoord() const;

		//! Optional user data
		UserDataPolicy& user();
		//! Optional user data
		const UserDataPolicy& user() const;

	}; // class Vertex



} // namespace Gfx3D
} // namespace Yuni


# include "vertex.hxx"


#endif // __YUNI_GFX3D_VERTEX_H__
