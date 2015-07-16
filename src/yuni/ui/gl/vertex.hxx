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
#ifndef __YUNI_GFX3D_VERTEX_HXX__
# define __YUNI_GFX3D_VERTEX_HXX__

namespace Yuni
{
namespace Gfx3D
{


	template<
		template<class> class NorP,
		template<class> class ColP,
		template<class> class TexP,
		class UserP
	>
	inline Vertex<NorP, ColP, TexP, UserP>::Vertex():
		Yuni::Point3D<float>(0.0f, 0.0f, 0.0f)
	{}


	template<
		template<class> class NorP,
		template<class> class ColP,
		template<class> class TexP,
		class UserP
	>
	template<class U, class V, class W>
	inline Vertex<NorP, ColP, TexP, UserP>::Vertex(U x, V y, W z):
		Yuni::Point3D<float>(x, y, z)
	{}


	template<
		template<class> class NorP,
		template<class> class ColP,
		template<class> class TexP,
		class UserP
	>
	inline Vertex<NorP, ColP, TexP, UserP>::Vertex(const Vertex& v):
		Yuni::Point3D<float>(v),
		NormalPolicy(v.normal()),
		ColorPolicy(v.color()),
		TexCoordPolicy(v.texCoord()),
		UserDataPolicy(v.user()),
		PaddingT()
	{}


	template<
		template<class> class NorP,
		template<class> class ColP,
		template<class> class TexP,
		class UserP
	>
	inline Vertex<NorP, ColP, TexP, UserP>& Vertex<NorP, ColP, TexP, UserP>::operator = (const Vertex& v)
	{
		// Copy each policy separately. Copying `None` policies will do nothing.
		(*static_cast<Yuni::Point3D<float>*>(this)) = static_cast<const Yuni::Point3D<float>&>(v);
		(*static_cast<NormalPolicy*>(this)) = static_cast<const NormalPolicy&>(v);
		(*static_cast<ColorPolicy*>(this)) = static_cast<const ColorPolicy&>(v);
		(*static_cast<TexCoordPolicy*>(this)) = static_cast<const TexCoordPolicy&>(v);
		(*static_cast<UserDataPolicy*>(this)) = static_cast<const UserDataPolicy&>(v);
		return *this;
	}


	template<
		template<class> class NorP,
		template<class> class ColP,
		template<class> class TexP,
		class UserP
	>
	inline bool Vertex<NorP, ColP, TexP, UserP>::operator < (const Vertex& v) const
	{
		if (x < v.x)
			return true;
		if (v.x < x)
			return false;
		if (y < v.y)
			return true;
		if (v.y < y)
			return false;
		if (z < v.z)
			return true;
		if (v.z < z)
			return false;
		int cmp = normal().compare(v.normal());
		if (cmp)
			return cmp < 0;
		cmp = color().compare(v.color());
		if (cmp)
			return cmp < 0;
		cmp = texCoord().compare(v.texCoord()) < 0;
		if (cmp)
			return cmp < 0;
		return user().compare(v.user()) < 0;
	}


	template<
		template<class> class NorP,
		template<class> class ColP,
		template<class> class TexP,
		class UserP
	>
	inline Yuni::Point3D<float>& Vertex<NorP, ColP, TexP, UserP>::position()
	{
		return *static_cast<Yuni::Point3D<float>*>(this);
	}

	template<
		template<class> class NorP,
		template<class> class ColP,
		template<class> class TexP,
		class UserP
	>
	inline const Yuni::Point3D<float>& Vertex<NorP, ColP, TexP, UserP>::position() const
	{
		return *static_cast<Yuni::Point3D<float>*>(this);
	}


	template<
		template<class> class NorP,
		template<class> class ColP,
		template<class> class TexP,
		class UserP
	>
	inline typename Vertex<NorP, ColP, TexP, UserP>::NormalPolicy& Vertex<NorP, ColP, TexP, UserP>::normal()
	{
		return *static_cast<NormalPolicy*>(this);
	}

	template<
		template<class> class NorP,
		template<class> class ColP,
		template<class> class TexP,
		class UserP
	>
	inline const typename Vertex<NorP, ColP, TexP, UserP>::NormalPolicy& Vertex<NorP, ColP, TexP, UserP>::normal() const
	{
		return *static_cast<const NormalPolicy*>(this);
	}


	template<
		template<class> class NorP,
		template<class> class ColP,
		template<class> class TexP,
		class UserP
	>
	inline typename Vertex<NorP, ColP, TexP, UserP>::ColorPolicy& Vertex<NorP, ColP, TexP, UserP>::color()
	{
		return *static_cast<ColorPolicy*>(this);
	}

	template<
		template<class> class NorP,
		template<class> class ColP,
		template<class> class TexP,
		class UserP
	>
	inline const typename Vertex<NorP, ColP, TexP, UserP>::ColorPolicy& Vertex<NorP, ColP, TexP, UserP>::color() const
	{
		return *static_cast<const ColorPolicy*>(this);
	}


	template<
		template<class> class NorP,
		template<class> class ColP,
		template<class> class TexP,
		class UserP
	>
	inline typename Vertex<NorP, ColP, TexP, UserP>::TexCoordPolicy& Vertex<NorP, ColP, TexP, UserP>::texCoord()
	{
		return *static_cast<TexCoordPolicy*>(this);
	}

	template<
		template<class> class NorP,
		template<class> class ColP,
		template<class> class TexP,
		class UserP
	>
	inline const typename Vertex<NorP, ColP, TexP, UserP>::TexCoordPolicy& Vertex<NorP, ColP, TexP, UserP>::texCoord() const
	{
		return *static_cast<const TexCoordPolicy*>(this);
	}


	template<
		template<class> class NorP,
		template<class> class ColP,
		template<class> class TexP,
		class UserP
	>
	inline typename Vertex<NorP, ColP, TexP, UserP>::UserDataPolicy& Vertex<NorP, ColP, TexP, UserP>::user()
	{
		return *static_cast<UserDataPolicy*>(this);
	}

	template<
		template<class> class NorP,
		template<class> class ColP,
		template<class> class TexP,
		class UserP
	>
	inline const typename Vertex<NorP, ColP, TexP, UserP>::UserDataPolicy& Vertex<NorP, ColP, TexP, UserP>::user() const
	{
		return *static_cast<const UserDataPolicy*>(this);
	}



} // namespace Gfx3D
} // namespace Yuni

#endif // __YUNI_GFX3D_VERTEX_HXX__
