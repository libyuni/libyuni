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
#include "materialsurface.h"
#include "texture.h"

namespace Yuni
{
namespace Gfx3D
{

	namespace // anonymous
	{

		static Texture::Ptr White = nullptr;

		static Texture::Ptr Checker = nullptr;

	} // namespace anonymous


	void MaterialSurfaceColored::activate(const ShaderProgram::Ptr& shaders, int index)
	{
		assert(shaders && shaders->valid());
		if (!White)
		{
			White = Texture::New(1, 1, 4, Texture::UInt8, nullptr, false);
			White->clearToWhite();
		}

		shaders->bindUniform(pUniformName, pColor.red, pColor.green, pColor.blue, pColor.alpha);
		// Bind the white
		if (index >= 0)
		{
			::glActiveTexture(GL_TEXTURE0 + index);
			::glBindTexture(GL_TEXTURE_2D, White->id());
			shaders->bindUniform(String("Texture") << index, Yuni::Gfx3D::Vertex<>::vaTexture0 + index);
			::glActiveTexture(GL_TEXTURE0);
		}
	}



	void MaterialSurfaceTextured::activate(const ShaderProgram::Ptr& shaders, int index)
	{
		assert(shaders && shaders->valid());

		if (index < 0)
			return;

		if (index == 0)
			shaders->bindUniform("MaterialDiffuse", 1.0f, 1.0f, 1.0f, 1.0f);
		::glActiveTexture(GL_TEXTURE0 + index);
		if (!pTexture)
		{
			// Assign a checkerboard texture to clearly show uninitialized texture materials
			if (!Checker)
				Checker = Texture::LoadFromFile("data/textures/checkerboard.jpg");
			::glBindTexture(GL_TEXTURE_2D, Checker->id());
		}
		else
			::glBindTexture(GL_TEXTURE_2D, pTexture->id());
		shaders->bindUniform(String("Texture") << index, Yuni::Gfx3D::Vertex<>::vaTexture0 + index);
		::glActiveTexture(GL_TEXTURE0);
	}



} // namespace Gfx3D
} // namespace Yuni
