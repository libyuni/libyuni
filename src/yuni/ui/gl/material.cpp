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

		pAmbient->activate(shaders, -1);
		pSpecular->activate(shaders, 1);
		pEmission->activate(shaders, 2);
		pDiffuse->activate(shaders, 0);
		// TODO : Have a normal map also ? Is this really material-related ?
	}



} // namespace Gfx3D
} // namespace Yuni
