#ifndef __YUNI_GFX3D_POSTEFFECT_H__
# define __YUNI_GFX3D_POSTEFFECT_H__

# include <yuni/yuni.h>
# include <yuni/core/smartptr.h>
# include <vector>
# include "shadermanager.h"

namespace Yuni
{
namespace Gfx3D
{

	/*!
	** \brief A post effect is a process that modifies a 2D image using shaders
	*/
	class PostEffect
	{
	public:
		typedef SmartPtr<PostEffect>  Ptr;
		typedef std::vector<Ptr>  Vector;

	public:
		PostEffect(const AnyString& name, const AnyString& vertexShader, const AnyString& fragmentShader, uint width, uint height):
			pName(name),
			pSize(width, height)
		{
			pShaders = ShaderManager::Instance().get(vertexShader, fragmentShader);
			//pShaders.bindAttribute("attrVertex", Yuni::Gfx3D::Vertex<>::vaPosition);
			pShaders->load();
		}

		void enableShader() const
		{
			if (!valid())
				return;
			pShaders->activate();
			pShaders->bindUniform("Texture0", Yuni::Gfx3D::Vertex<>::vaTexture0);
			pShaders->bindUniform("Width", pSize.x);
			pShaders->bindUniform("Height", pSize.y);
		}

		void disableShader() const { pShaders->deactivate(); }

		bool valid() const { return !(!pShaders) && pShaders->valid(); }

		const String& name() const { return pName; }

		void name(const AnyString& newName) { pName = newName; }

	private:
		String pName;

		Point2D<float> pSize;

		ShaderProgram::Ptr pShaders;

	}; // class PostEffect


} // namespace Gfx3D
} // namespace Yuni

#endif // __YUNI_GFX3D_POSTEFFECT_H__
