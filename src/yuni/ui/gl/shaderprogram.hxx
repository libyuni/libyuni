#ifndef __YUNI_GFX3D_SHADERPROGRAM_HXX__
# define __YUNI_GFX3D_SHADERPROGRAM_HXX__

# include "../..//private/graphics/opengl/glew/glew.h"
# include "../../core/static/assert.h"
# include "glerror.h"


namespace Yuni
{
namespace Gfx3D
{


	inline ShaderProgram::ShaderProgram():
		pID(::glCreateProgram()),
		pLinked(false)
	{}


	inline void ShaderProgram::activate() const
	{
		GLClearError();
		::glUseProgram(pID);
		GLTestError("ShaderProgram::activate, glUseProgram");
	}


	inline void ShaderProgram::deactivate() const
	{
		::glUseProgram(0);
	}


	inline bool ShaderProgram::valid() const
	{
		return pLinked;
	}


	inline void ShaderProgram::bindAttribute(const AnyString& name, Vertex<>::Attribute attribIndex) const
	{
		GLClearError();
		::glBindAttribLocation(pID, attribIndex, name.c_str());
		GLTestError("ShaderProgram::bindAttribute, glBindAttribLocation");
	}


	inline void ShaderProgram::bindUniform(const AnyString& name, int value) const
	{
		GLClearError();
		GLint location = ::glGetUniformLocation(pID, name.c_str());
		if (not GLTestError("ShaderProgram::bindUniform, glGetUniformLocation"))
			return;
		::glUniform1i(location, value);
		GLTestError("ShaderProgram::bindUniform, glUniform1i");
	}

	inline void ShaderProgram::bindUniform(const AnyString& name, float value) const
	{
		GLClearError();
		GLint location = ::glGetUniformLocation(pID, name.c_str());
		if (not GLTestError("ShaderProgram::bindUniform, glGetUniformLocation"))
			return;
		::glUniform1f(location, value);
	}

	inline void ShaderProgram::bindUniform(const AnyString& name, uint value) const
	{
		GLClearError();
		GLint location = ::glGetUniformLocation(pID, name.c_str());
		if (not GLTestError("ShaderProgram::bindUniform, glGetUniformLocation"))
			return;
		::glUniform1ui(location, value);
	}

	inline void ShaderProgram::bindUniform(const AnyString& name, Vertex<>::Attribute value) const
	{
		// int, not uint, otherwise using Texture[0-7] will not work...
		// From the man page : "glUniform1i and glUniform1iv are the only two functions
		// that may be used to load uniform variables defined as sampler types"
		bindUniform(name, static_cast<int>(value));
	}


	inline void ShaderProgram::bindUniform(const AnyString& name, const Vector3D<int>& value) const
	{
		GLClearError();
		GLint location = ::glGetUniformLocation(pID, name.c_str());
		if (not GLTestError("ShaderProgram::bindUniform, glGetUniformLocation"))
			return;
		::glUniform3i(location, value.x, value.y, value.z);
	}


	inline void ShaderProgram::bindUniform(const AnyString& name, const Vector3D<float>& value) const
	{
		GLClearError();
		GLint location = ::glGetUniformLocation(pID, name.c_str());
		if (not GLTestError("ShaderProgram::bindUniform, glGetUniformLocation"))
			return;
		::glUniform3f(location, value.x, value.y, value.z);
	}


	inline void ShaderProgram::bindUniform(const AnyString& name, const Vector3D<uint>& value) const
	{
		GLClearError();
		GLint location = ::glGetUniformLocation(pID, name.c_str());
		if (not GLTestError("ShaderProgram::bindUniform, glGetUniformLocation"))
			return;
		::glUniform3ui(location, value.x, value.y, value.z);
	}


	inline void ShaderProgram::bindUniform(const AnyString& name, const Color::RGB<float>& value) const
	{
		GLClearError();
		GLint location = ::glGetUniformLocation(pID, name.c_str());
		if (not GLTestError("ShaderProgram::bindUniform, glGetUniformLocation"))
			return;
		::glUniform3f(location, value.red, value.green, value.blue);
	}


	inline void ShaderProgram::bindUniform(const AnyString& name, const Color::RGBA<float>& value) const
	{
		GLClearError();
		GLint location = ::glGetUniformLocation(pID, name.c_str());
		if (not GLTestError("ShaderProgram::bindUniform, glGetUniformLocation"))
			return;
		::glUniform4f(location, value.red, value.green, value.blue, value.alpha);
	}


	inline void ShaderProgram::bindUniform(const AnyString& name, float v1, float v2, float v3, float v4) const
	{
		GLClearError();
		GLint location = ::glGetUniformLocation(pID, name.c_str());
		if (not GLTestError("ShaderProgram::bindUniform, glGetUniformLocation"))
			return;
		::glUniform4f(location, v1, v2, v3, v4);
	}


	inline void ShaderProgram::bindUniformArray(const AnyString& name, uint count, uint components,
		const int* array) const
	{
		GLClearError();
		GLint location = ::glGetUniformLocation(pID, name.c_str());
		if (not GLTestError("ShaderProgram::bindUniformArray, glGetUniformLocation"))
			return;
		switch (components)
		{
			case 1:
				::glUniform1iv(location, (int) count, array);
				break;
			case 2:
				::glUniform2iv(location, (int) count, array);
				break;
			case 3:
				::glUniform3iv(location, (int) count, array);
				break;
			case 4:
				::glUniform4iv(location, (int) count, array);
				break;
			default:
				assert(false and "Invalid number of components : must be 1-4.");
		}
	}


	inline void ShaderProgram::bindUniformArray(const AnyString& name, uint count, uint components,
		const float* array) const
	{
		GLClearError();
		GLint location = ::glGetUniformLocation(pID, name.c_str());
		if (not GLTestError("ShaderProgram::bindUniformArray, glGetUniformLocation"))
			return;

		switch (components)
		{
			case 1:
				::glUniform1fv(location, (int) count, array);
				break;
			case 2:
				::glUniform2fv(location, (int) count, array);
				break;
			case 3:
				::glUniform3fv(location, (int) count, array);
				break;
			case 4:
				::glUniform4fv(location, (int) count, array);
				break;
			default:
				assert(false and "Invalid number of components : must be 1-4.");
		}
	}


	inline void ShaderProgram::bindUniformArray(const AnyString& name, uint count, uint components,
		const uint* array) const
	{
		assert(components >= 1 and components <= 4 and "Invalid number of components : must be 1-4.");
		GLClearError();
		GLint location = ::glGetUniformLocation(pID, name.c_str());
		if (not GLTestError("ShaderProgram::bindUniformArray, glGetUniformLocation"))
			return;
		switch (components)
		{
			case 1:
				::glUniform1uiv(location, (int) count, array);
				break;
			case 2:
				::glUniform2uiv(location, (int) count, array);
				break;
			case 3:
				::glUniform3uiv(location, (int) count, array);
				break;
			case 4:
				::glUniform4uiv(location, (int) count, array);
				break;
			default:
				assert(false and "Invalid number of components : must be 1-4.");
		}
	}


	inline void ShaderProgram::bindImage(const AnyString& name, const Texture::Ptr& texture,
		Vertex<>::Attribute value) const
	{
		GLClearError();
		::glBindTexture(GL_TEXTURE_2D, texture->id());
		::glBindImageTexture(0, texture->id(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
		if (not GLTestError("ShaderProgram::bindImage, glBindImageTexture"))
			return;
		bindUniform(name, value);
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

#endif // __YUNI_GFX3D_SHADERPROGRAM_HXX__
