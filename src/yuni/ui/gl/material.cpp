#include "material.h"
#include "shaderprogram.h"

namespace Yuni
{
namespace Gfx3D
{

	namespace // anonymous
	{
		Material::Ptr sDefault = new Material();

	}; // namespace anonymous


	const Material::Ptr& Material::Default()
	{
		return sDefault;
	}


	void Material::activate(const ShaderProgram::Ptr& shaders)
	{
		if (!shaders || !shaders->valid())
			return;

		pAmbient->activate(shaders);
		pDiffuse->activate(shaders);
		pSpecular->activate(shaders);
		pEmission->activate(shaders);
	}



} // namespace Gfx3D
} // namespace Yuni
