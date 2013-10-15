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
