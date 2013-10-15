
#include "shaderprogram.h"

namespace Yuni
{
namespace Gfx3D
{


	ShaderProgram::ShaderProgram(const VertexShader::Ptr& vShader, const FragmentShader::Ptr& fShader):
		pID(::glCreateProgram()),
		pLinked(false),
		pVertexShader(vShader),
		pFragmentShader(fShader),
		pComputeShader(nullptr)
	{}


	ShaderProgram::ShaderProgram(const AnyString& vShaderPath, const AnyString& fShaderPath):
		pID(::glCreateProgram()),
		pLinked(false)
	{
		VertexShader* vShader = new VertexShader();
		if (not vShader->loadFromFile(vShaderPath))
		{
			delete vShader;
			return;
		}
		pVertexShader = vShader;

		FragmentShader* fShader = new FragmentShader();
		if (not fShader->loadFromFile(fShaderPath))
		{
			delete fShader;
			return;
		}
		pFragmentShader = fShader;
	}


	ShaderProgram::~ShaderProgram()
	{
		if (pVertexShader)
		{
			::glDetachShader(pID, pVertexShader->pID);
			pVertexShader = nullptr;
		}
		if (pFragmentShader)
		{
			::glDetachShader(pID, pFragmentShader->pID);
			pFragmentShader = nullptr;
		}
		if (pComputeShader)
		{
			::glDetachShader(pID, pComputeShader->pID);
			pComputeShader = nullptr;
		}
		::glDeleteProgram(pID);
	}


	bool ShaderProgram::load()
	{
		GLClearError();
		pError.clear();
		// If not yet linked, do it on-the-fly
		if (not pLinked)
		{
			// Make sure we have at least one of the three shaders
			if ((!pVertexShader or !pVertexShader->pID)
				and (!pFragmentShader or !pFragmentShader->pID)
				and (!pComputeShader or !pComputeShader->pID))
			{
				return false;
			}

			if (pVertexShader and pVertexShader->pID)
			{
				::glAttachShader(pID, pVertexShader->pID);
				GLTestError("ShaderProgram::load, glAttachShader, attaching vertex shader");
			}

			if (pFragmentShader and pFragmentShader->pID)
			{
				::glAttachShader(pID, pFragmentShader->pID);
				GLTestError("ShaderProgram::load, glAttachShader, attaching fragment shader");
			}

			if (pComputeShader and pComputeShader->pID)
			{
				::glAttachShader(pID, pComputeShader->pID);
				GLTestError("ShaderProgram::load, glAttachShader, attaching compute shader");
			}

			// Link
			int linked;
			::glLinkProgram(pID);
			GLTestError("ShaderProgram::load, glLinkProgram");
			::glGetProgramiv(pID, GL_LINK_STATUS, &linked);
			GLTestError("ShaderProgram::load, glGetProgramiv");
			pLinked = (linked == GL_TRUE);
			if (!pLinked)
			{
				char log[1024];
				int length = 0;
				::glGetProgramInfoLog(pID, 1024, &length, log);
				log[length] = '\0';
				pError.clear() << "Shader program " << pID << " failed to load !\n" << (const char*)log;
				return false;
			}
		}
		return true;
	}



} // namespace Gfx3D
} // namespace Yuni
