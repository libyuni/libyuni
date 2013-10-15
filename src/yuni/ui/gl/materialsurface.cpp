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


	void MaterialSurfaceColored::activate(const ShaderProgram::Ptr& shaders)
	{
		assert(shaders && shaders->valid());
		if (!White)
		{
			White = Texture::New(2, 2, 4, nullptr, false);
			White->clearToWhite();
		}

		shaders->bindUniform(pUniformName, pColor.red, pColor.green, pColor.blue, pColor.alpha);
		// Bind the white
		// ::glActiveTexture(GL_TEXTURE0);
		// ::glBindTexture(GL_TEXTURE_2D, White->id());
		// shaders.bindUniform("Texture0", Yuni::Gfx3D::Vertex<>::vaTexture0);
	}



	void MaterialSurfaceTextured::activate(const ShaderProgram::Ptr& shaders)
	{
		assert(shaders && shaders->valid());
		if (!Checker)
			Checker = Texture::LoadFromFile("data/textures/checkerboard.jpg");

		::glActiveTexture(GL_TEXTURE0);
		if (!pTexture)
			// Assign a checkerboard texture to clearly show uninitialized texture materials
			::glBindTexture(GL_TEXTURE_2D, Checker->id());
		else
			::glBindTexture(GL_TEXTURE_2D, pTexture->id());
		shaders->bindUniform("Texture0", Yuni::Gfx3D::Vertex<>::vaTexture0);
	}



} // namespace Gfx3D
} // namespace Yuni
