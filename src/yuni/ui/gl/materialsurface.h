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
#ifndef __YUNI_GFX3D_MATERIALSURFACE_H__
# define __YUNI_GFX3D_MATERIALSURFACE_H__

# include <yuni/yuni.h>
# include <yuni/core/smartptr.h>
# include <yuni/core/string.h>
# include <yuni/core/color/rgba.h>
# include <vector>
# include "texture.h"
# include "shaderprogram.h"

namespace Yuni
{
namespace Gfx3D
{


	/*!
	** \name A material surface is an abstract base for various ways of coloring in a material
	*/
	class MaterialSurface
	{
	public:
		//! Smart pointer
		typedef SmartPtr<MaterialSurface>  Ptr;

	public:
		//! Virtual destructor
		virtual ~MaterialSurface() {}

		//! Activate this surface
		virtual void activate(const ShaderProgram::Ptr& shaders, int index) = 0;

	}; // class MaterialSurface


	/*!
	** \name Textured surface
	*/
	class MaterialSurfaceTextured: public MaterialSurface
	{
	public:
		//! Constructor
		MaterialSurfaceTextured(const Texture::Ptr& texture):
			pTexture(texture)
		{}

		//! Virtual destructor
		virtual ~MaterialSurfaceTextured() {}

		//! Activate this surface
		virtual void activate(const ShaderProgram::Ptr& shaders, int index) override;

		//! Texture used on this surface
		const Texture::Ptr& texture() const { return pTexture; }

	private:
		//! Texture used on this surface
		Texture::Ptr pTexture;

	}; // class MaterialSurfaceTextured


	/*!
	** \name Singly-colored material surface
	*/
	class MaterialSurfaceColored: public MaterialSurface
	{
	public:
		template<class T>
		MaterialSurfaceColored(const AnyString& uniformName, T r, T g, T b):
			pUniformName(uniformName),
			pColor(r, g, b)
		{}

		template<class T>
		MaterialSurfaceColored(const AnyString& uniformName, T r, T g, T b, T a):
			pUniformName(uniformName),
			pColor(r, g, b, a)
		{}

		template<class T>
		MaterialSurfaceColored(const AnyString& uniformName, const Color::RGB<T>& color):
			pUniformName(uniformName),
			pColor(color)
		{}

		template<class T>
		MaterialSurfaceColored(const AnyString& uniformName, const Color::RGBA<T>& color):
			pUniformName(uniformName),
			pColor(color)
		{}

		//! Virtual destructor
		virtual ~MaterialSurfaceColored() {}

		//! Activate this surface
		virtual void activate(const ShaderProgram::Ptr& shaders, int index) override;

		//! Surface color
		const Color::RGBA<float>& color() const { return pColor; }

	private:
		//! Name of the corresponding uniform in the shaders
		String pUniformName;

		//! Color used for this surface
		Color::RGBA<float> pColor;

	}; // class MaterialSurfaceColored



} // namespace Gfx3D
} // namespace Yuni

#endif // __YUNI_GFX3D_MATERIALSURFACE_H__
