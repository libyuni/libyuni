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
#ifndef __YUNI_GFX3D_MATERIAL_HXX__
# define __YUNI_GFX3D_MATERIAL_HXX__

namespace Yuni
{
namespace Gfx3D
{


	inline Material::Material():
		pName("New material"),
		pID(UUID::fGenerate),
		pAmbient(new MaterialSurfaceColored("MaterialAmbient", 1.0f, 1.0f, 1.0f)),
		pDiffuse(new MaterialSurfaceColored("MaterialDiffuse", 1.0f, 1.0f, 1.0f)),
		pSpecular(new MaterialSurfaceColored("MaterialSpecular", 0.0f, 0.0f, 0.0f)),
		pEmission(new MaterialSurfaceColored("MaterialEmission", 0.0f, 0.0f, 0.0f)),
		pShininess(1.0f),
		pTransparency(0.0f),
		pRefractionIndex(1.0f)
	{}


	inline const Material::ID& Material::id() const
	{
		return pID;
	}


	inline const String& Material::name() const
	{
		return pName;
	}

	inline void Material::name(const AnyString& name)
	{
		pName = name;
	}



	inline const MaterialSurface::Ptr& Material::ambient() const
	{
		return pAmbient;
	}

	template <class T>
	inline void Material::ambient(const Color::RGB<T>& ambient)
	{
		pAmbient = new MaterialSurfaceColored("MaterialAmbient", ambient);
	}

	template <class T>
	inline void Material::ambient(const Color::RGBA<T>& ambient)
	{
		pAmbient = new MaterialSurfaceColored("MaterialAmbient", ambient);
	}


	inline void Material::ambient(const Texture::Ptr& texture)
	{
		pAmbient = new MaterialSurfaceTextured(texture);
	}



	inline const MaterialSurface::Ptr& Material::diffuse() const
	{
		return pDiffuse;
	}


	template <class T>
	inline void Material::diffuse(const Color::RGB<T>& diffuse)
	{
		pDiffuse = new MaterialSurfaceColored("MaterialDiffuse", diffuse);
	}


	template <class T>
	inline void Material::diffuse(const Color::RGBA<T>& diffuse)
	{
		pDiffuse = new MaterialSurfaceColored("MaterialDiffuse", diffuse);
	}


	inline void Material::diffuse(const Texture::Ptr& texture)
	{
		pDiffuse = new MaterialSurfaceTextured(texture);
	}



	inline const MaterialSurface::Ptr& Material::specular() const
	{
		return pSpecular;
	}


	template <class T>
	inline void Material::specular(const Color::RGB<T>& specular)
	{
		pSpecular = new MaterialSurfaceColored("MaterialSpecular", specular);
	}


	template <class T>
	inline void Material::specular(const Color::RGBA<T>& specular)
	{
		pSpecular = new MaterialSurfaceColored("MaterialSpecular", specular);
	}


	inline void Material::specular(const Texture::Ptr& texture)
	{
		pSpecular = new MaterialSurfaceTextured(texture);
	}



	inline const MaterialSurface::Ptr& Material::emission() const
	{
		return pEmission;
	}


	template <class T>
	inline void Material::emission(const Color::RGB<T>& emission)
	{
		pEmission = new MaterialSurfaceColored("MaterialEmission", emission);
	}


	template <class T>
	inline void Material::emission(const Color::RGBA<T>& emission)
	{
		pEmission = new MaterialSurfaceColored("MaterialEmission", emission);
	}


	inline void Material::emission(const Texture::Ptr& texture)
	{
		pEmission = new MaterialSurfaceTextured(texture);
	}



	inline float Material::shininess() const
	{
		return pShininess;
	}


	inline void Material::shininess(float shininess)
	{
		pShininess = shininess;
	}


	inline float Material::transparency() const
	{
		return pTransparency;
	}


	inline void Material::transparency(float transparency)
	{
		pTransparency = transparency;
	}


	inline float Material::refractionIndex() const
	{
		return pRefractionIndex;
	}


	inline void Material::refractionIndex(float newIndex)
	{
		pRefractionIndex = newIndex;
	}



} // namespace Gfx3D
} // namespace Yuni

#endif // __YUNI_GFX3D_MATERIAL_HXX__
