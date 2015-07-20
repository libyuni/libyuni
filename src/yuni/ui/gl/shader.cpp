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
#include <yuni/yuni.h>
#include <yuni/io/file.h>
#include <yuni/private/graphics/opengl/glew/glew.h>
#include <cassert>
#include "shader.h"
#include "glerror.h"


namespace Yuni
{
namespace Gfx3D
{

	VertexShader::VertexShader(const AnyString& filePath)
	{
		loadFromFile(filePath);
	}


	VertexShader::~VertexShader()
	{
		if (pID != invalidID)
			::glDeleteShader(pID);
	}


	bool VertexShader::loadFromFile(const AnyString& filePath)
	{
		String data;
		if (IO::errNone != IO::File::LoadFromFile(data, filePath))
		{
			# if defined(DEBUG) && !defined(NDEBUG)
			std::cerr << "Failed to load shader \"" << filePath << "\" !";
			# endif
			return false;
		}
		return loadFromMemory(data);
	}


	bool VertexShader::loadFromMemory(const AnyString& source)
	{
		if (pID == invalidID)
			pID = ::glCreateShader(GL_VERTEX_SHADER);
		const char* data = source.data();
		::glShaderSource(pID, 1, &data, nullptr);
		::glCompileShader(pID);
		return GLTestError("VertexShader::loadFromMemory");
	}








	FragmentShader::FragmentShader(const AnyString& filePath)
	{
		loadFromFile(filePath);
	}


	FragmentShader::~FragmentShader()
	{
		if (pID != invalidID)
			::glDeleteShader(pID);
	}


	bool FragmentShader::loadFromFile(const AnyString& filePath)
	{
		String data;
		if (IO::errNone != IO::File::LoadFromFile(data, filePath))
		{
			# if defined(DEBUG) && !defined(NDEBUG)
			std::cerr << "Failed to load shader \"" << filePath << "\" !" << std::endl;
			# endif
			return false;
		}
		return loadFromMemory(data);
	}


	bool FragmentShader::loadFromMemory(const AnyString& source)
	{
		if (pID == invalidID)
			pID = ::glCreateShader(GL_FRAGMENT_SHADER);

		const char* data = source.data();
		::glShaderSource(pID, 1, &data, nullptr);
		::glCompileShader(pID);
		return GLTestError("FragmentShader::loadFromMemory");
	}




	ComputeShader::ComputeShader(const AnyString& filePath)
	{
		loadFromFile(filePath);
	}


	ComputeShader::~ComputeShader()
	{
		if (pID != invalidID)
			::glDeleteShader(pID);
	}


	bool ComputeShader::loadFromFile(const AnyString& filePath)
	{
		String data;
		if (IO::errNone != IO::File::LoadFromFile(data, filePath))
		{
			# if defined(DEBUG) && !defined(NDEBUG)
			std::cerr << "Failed to load shader \"" << filePath << "\" !";
			# endif
			return false;
		}
		return loadFromMemory(data);
	}


	bool ComputeShader::loadFromMemory(const AnyString& source)
	{
		if (pID == invalidID)
			pID = ::glCreateShader(GL_COMPUTE_SHADER);

		const char* data = source.data();
		::glShaderSource(pID, 1, &data, nullptr);
		::glCompileShader(pID);
		return GLTestError("ComputeShader::loadFromMemory, glCompileShader");
	}





} // namespace Gfx3D
} // namespace Yuni
