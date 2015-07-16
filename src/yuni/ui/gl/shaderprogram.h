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
#ifndef __YUNI_UI_GL_SHADERPROGRAM_H__
# define __YUNI_UI_GL_SHADERPROGRAM_H__

# include "../../yuni.h"
# include "../../core/smartptr.h"
# include "shader.h"
# include "texture.h"
# include "vertex.h"
# include <map>


namespace Yuni
{
namespace Gfx3D
{

	/*!
	** \brief A shader program can be bound to a vertex shader, a fragment shader and a compute shader
	*/
	class ShaderProgram final
	{
	public:
		//! Smart pointer
		typedef SmartPtr<ShaderProgram>  Ptr;
		//! Cache for name locations in the shader program
		typedef std::map<String, int>  LocationCache;

	public:
		//! \name Constructors & Destructor
		//@{
		//! Constructor
		ShaderProgram();
		//! Constructor with the shaders already compiled
		ShaderProgram(const VertexShader::Ptr& vShader, const FragmentShader::Ptr& fShader);
		//! Constructor with the paths to the shaders
		ShaderProgram(const AnyString& vShaderPath, const AnyString& fShaderPath);
		//! Destructor
		~ShaderProgram();
		//@}

		//! Set the vertex shader
		void vertexShader(const VertexShader::Ptr& shader);
		//! Get the vertex shader
		const VertexShader::Ptr& vertexShader() const;

		//! Set the fragment shader
		void fragmentShader(const FragmentShader::Ptr& shader);
		//! Get the fragment shader
		const FragmentShader::Ptr& fragmentShader() const;

		//! Set the compute shader
		void computeShader(const ComputeShader::Ptr& shader);
		//! Get the compute shader
		const ComputeShader::Ptr& computeShader() const;

		//! Load the program
		bool load();

		//! Activate this program
		void activate() const;

		/*!
		** 'brief Deactivate this program
		**
		** \internal This method does not use any class data in the current
		**   implementation but it may change in the future
		*/
		static void deactivate();

		//! Is the program valid for use ?
		bool valid() const;

		//! Get the error message, empty string if none
		const String& errorMessage() const;

		/*!
		** \brief Bind an attribute index to a variable name in the shader program
		**
		** \warning This must be called before the program is linked !
		*/
		void bindAttribute(const AnyString& name, Vertex<>::Attribute attribIndex) const;

		/*!
		** \brief  Bind a uniform value to a name in the shader program
		**
		** \warning This must be called after the program is linked and activated !
		*/
		void bindUniform(const AnyString& name, int value) const;
		void bindUniform(const AnyString& name, uint value) const;
		void bindUniform(const AnyString& name, float value) const;
		void bindUniform(const AnyString& name, Vertex<>::Attribute value) const;
		void bindUniform(const AnyString& name, const Vector3D<int>& value) const;
		void bindUniform(const AnyString& name, const Vector3D<float>& value) const;
		void bindUniform(const AnyString& name, const Vector3D<uint>& value) const;
		void bindUniform(const AnyString& name, const Color::RGB<float>& value) const;
		void bindUniform(const AnyString& name, const Color::RGBA<float>& value) const;
		// Temporary prototype, we need a Vector4 or matrices
		void bindUniform(const AnyString& name, float v1, float v2, float v3, float v4) const;

		//! Bind a uniform array of values to a name in the shader program
		void bindUniformArray(const AnyString& name, uint count, uint components, const int* array) const;
		void bindUniformArray(const AnyString& name, uint count, uint components, const uint* array) const;
		void bindUniformArray(const AnyString& name, uint count, uint components, const float* array) const;

		//! Bind a texture as an image for compute shader use
		void bindImage(const AnyString& name, const Texture::Ptr& texture,
			Vertex<>::Attribute value) const;

	private:
		//! ID of the shader program
		mutable uint pID;
		//! Is the program already linked ?
		mutable bool pLinked;

		//! Current vertex shader
		VertexShader::Ptr pVertexShader;
		//! Current fragment shader
		FragmentShader::Ptr pFragmentShader;
		//! Current compute shader
		ComputeShader::Ptr pComputeShader;

		//! Error string storage
		String pError;
		//! Cache for uniform locations
		mutable LocationCache pUniformCache;

	}; // class ShaderProgram





} // namespace Gfx3D
} // namespace Yuni

# include "shaderprogram.hxx"

#endif // __YUNI_UI_GL_SHADERPROGRAM_H__
