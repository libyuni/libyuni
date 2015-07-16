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
#ifndef __YUNI_UI_GL_SHADERMANAGER_H__
# define __YUNI_UI_GL_SHADERMANAGER_H__

# include <yuni/yuni.h>
# include <yuni/core/dictionary.h>
# include <yuni/core/singleton.h>
# include "shaderprogram.h"

namespace Yuni
{
namespace Gfx3D
{


	/*!
	** \brief Create shader programs and shaders
	*/
	class ShaderManager : public Singleton<ShaderManager>
	{
	public:
		//! Pair of vertex + fragment shader IDs
		typedef std::pair<IShader::ID, IShader::ID>  ShaderPair;

		//! Cache for shader programs, by vertex + fragment shader IDs
		typedef Dictionary<ShaderPair, ShaderProgram::Ptr>::Ordered ProgramCache;


	public:
		/*!
		** \brief Get a shader program from the paths to the vertex and fragment shaders
		**
		** \returns The shader program if both shaders were successfully loaded. nullptr otherwise.
		** \warning The attributes need to be binded after this, and load() must be called.
		** \code
		** bool loadShaders(ShaderProgram::Ptr& program)
		** {
		**   program = ShaderManager::getFromFiles("vertexshader.glsl", "fragmentshader.glsl");
		**   if (!program)
		**   {
		**     std::cerr << "Could not find shaders !" << std::endl;
		**     return false;
		**   }
		**
		**   // Bind attributes
		**   program->bindAttribute("MyTexture", Vertex<>::vaTexture0);
		**   // Link the program
		**   if (!program->load())
		**   {
		**     std::cerr << "Could not link shader program !" << std::endl;
		**     return false;
		**   }
		**
		**   // Activate before binding uniforms
		**   program->activate();
		**   // Bind uniforms
		**   program->bindUniform("MyFloatValue", 0.0f);
		**   // You may deactivate it here if you do not want to draw with it immediately
		**   program->deactivate();
		**   return true;
		** }
		** \endcode
		*/
		const ShaderProgram::Ptr getFromFiles(const AnyString& vsPath, const AnyString& fsPath);

		/*
		** \brief Get a shader program from strings containing the vertex and fragment shaders
		**
		** \returns The shader program if both shaders were successfully loaded. nullptr otherwise.
		** \warning The attributes need to be binded after this, and load() must be called.
		*/
		const ShaderProgram::Ptr getFromMemory(const AnyString& vsCode, const AnyString& fsCode);


	private:
		//! Private constructor
		ShaderManager() {}

		//! Private copy constructor
		ShaderManager(const ShaderManager&);

	private:
		//! Current programs as referenced by the IDs of the shaders they contain
		ProgramCache pPrograms;
		//! Currently loaded vertex shaders
		VertexShader::Map pVertexShaders;
		//! Currently loaded fragment shaders
		FragmentShader::Map pFragmentShaders;

		//! Friend declaration for constructor access
		template<class T> friend class Yuni::Policy::Creation::EmptyConstructor;

	}; // class ShaderManager





} // namespace Gfx3D
} // namespace Yuni

#endif // __YUNI_UI_GL_SHADERMANAGER_H__
