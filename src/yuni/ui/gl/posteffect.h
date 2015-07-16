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
#ifndef __YUNI_UI_POSTEFFECT_H__
# define __YUNI_UI_POSTEFFECT_H__

# include "../../yuni.h"
# include "../../core/smartptr.h"
# include "shadermanager.h"
# include <vector>

namespace Yuni
{
namespace UI
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
		PostEffect(const AnyString& name, const AnyString& vertexShader, const AnyString& fragmentShader, float width, float height):
			pName(name),
			pSize(width > 0.0f ? width : 0.0f, height > 0.0f ? height : 0.0f)
		{
			pShaders = Gfx3D::ShaderManager::Instance().getFromFiles(vertexShader, fragmentShader);
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

		Gfx3D::ShaderProgram::Ptr pShaders;

	}; // class PostEffect



} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_POSTEFFECT_H__
