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
#ifndef __YUNI_UI_GL_SHADERPROGRAM_HXX__
# define __YUNI_UI_GL_SHADERPROGRAM_HXX__

# include "../../core/static/assert.h"
# include "glerror.h"


namespace Yuni
{
namespace Gfx3D
{

	inline bool ShaderProgram::valid() const
	{
		return pLinked;
	}


	inline void ShaderProgram::bindUniform(const AnyString& name, Vertex<>::Attribute value) const
	{
		// int, not uint, otherwise using Texture[0-7] will not work...
		// From the man page : "glUniform1i and glUniform1iv are the only two functions
		// that may be used to load uniform variables defined as sampler types"
		bindUniform(name, static_cast<int>(value));
	}


	inline const String& ShaderProgram::errorMessage() const
	{
		return pError;
	}


	inline void ShaderProgram::vertexShader(const VertexShader::Ptr& shader)
	{
		pVertexShader = shader;
	}


	inline const VertexShader::Ptr& ShaderProgram::vertexShader() const
	{
		return pVertexShader;
	}


	inline void ShaderProgram::fragmentShader(const FragmentShader::Ptr& shader)
	{
		pFragmentShader = shader;
	}


	inline const FragmentShader::Ptr& ShaderProgram::fragmentShader() const
	{
		return pFragmentShader;
	}


	inline void ShaderProgram::computeShader(const ComputeShader::Ptr& shader)
	{
		pComputeShader = shader;
	}


	inline const ComputeShader::Ptr& ShaderProgram::computeShader() const
	{
		return pComputeShader;
	}





} // namespace Gfx3D
} // namespace Yuni

#endif // __YUNI_UI_GL_SHADERPROGRAM_HXX__
