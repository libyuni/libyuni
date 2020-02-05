/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** gitlab: https://gitlab.com/libyuni/libyuni/
** github: https://github.com/libyuni/libyuni/ {mirror}
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
		GLClearError();
		pError.clear();

		if (pID == invalidID)
			pID = ::glCreateShader(GL_VERTEX_SHADER);
		const char* data = source.data();
		::glShaderSource(pID, 1, &data, nullptr);
		::glCompileShader(pID);
		GLTestError("VertexShader::loadFromMemory, glCompileShader");

		int compiled;
		::glGetShaderiv(pID, GL_COMPILE_STATUS, &compiled);
		GLTestError("VertexShader::loadFromMemory, glGetProgramiv");

		if (GL_FALSE == compiled)
		{
			char log[1024];
			int length = 0;
			::glGetShaderInfoLog(pID, 1024, &length, log);

			pError.clear() << "Shader " << pID << " failed to compile !";
			if ((uint) length < 1024)
			{
				log[(uint)length] = '\0'; // for safety, just in case
				pError += '\n';
				pError.append(log, (uint)length);
				std::cerr << pError;
			}
		}
		return GL_TRUE == compiled;
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
		GLClearError();
		pError.clear();

		if (pID == invalidID)
			pID = ::glCreateShader(GL_FRAGMENT_SHADER);

		const char* data = source.data();
		::glShaderSource(pID, 1, &data, nullptr);
		::glCompileShader(pID);
		GLTestError("FragmentShader::loadFromMemory");

		int compiled;
		::glGetShaderiv(pID, GL_COMPILE_STATUS, &compiled);
		GLTestError("FragmentShader::loadFromMemory, glGetProgramiv");

		if (GL_FALSE == compiled)
		{
			char log[1024];
			int length = 0;
			::glGetShaderInfoLog(pID, 1024, &length, log);

			pError.clear() << "Shader " << pID << " failed to compile !";
			if ((uint) length < 1024)
			{
				log[(uint)length] = '\0'; // for safety, just in case
				pError += '\n';
				pError.append(log, (uint)length);
				std::cerr << pError;
			}
		}
		return GL_TRUE == compiled;
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
