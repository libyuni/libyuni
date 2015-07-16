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
#ifndef __YUNI_GFX3D_GL_ERROR_H__
# define __YUNI_GFX3D_GL_ERROR_H__

#	include <iostream>
#	include <yuni/core/logs.h>
#	include <yuni/core/string.h>
#	include <yuni/private/graphics/opengl/glew/glew.h>

namespace Yuni
{
namespace Gfx3D
{

	inline void GLClearError()
	{
		# ifndef NDEBUG
		::glGetError();
		# endif // DEBUG
	}


	inline bool GLTestError(const AnyString& location)
	{
		# ifndef NDEBUG

		GLenum err = ::glGetError();
		if (err == GL_NO_ERROR)
			return true;

		const char* errorText;

		switch (err)
		{
			case GL_NO_ERROR:
				errorText = "GL_NO_ERROR";
				break;
			case GL_INVALID_VALUE:
				errorText = "GL_INVALID_VALUE";
				break;
			case GL_INVALID_ENUM:
				errorText = "GL_INVALID_ENUM";
				break;
			case GL_INVALID_OPERATION:
				errorText = "GL_INVALID_OPERATION";
				break;
			case GL_STACK_OVERFLOW:
				errorText = "GL_STACK_OVERFLOW";
				break;
			case GL_STACK_UNDERFLOW:
				errorText = "GL_STACK_UNDERFLOW";
				break;
			case GL_OUT_OF_MEMORY:
				errorText = "GL_OUT_OF_MEMORY";
				break;
			default:
				errorText = "Unknown GL error";
		}

		std::cerr << errorText << " in " << location << std::endl;
		return false;

		# endif // !NDEBUG
		return true;
	}



} // namespace Gfx3D
} // namespace Yuni

#endif // __YUNI_GFX3D_GL_ERROR_H__
