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
#ifndef __YUNI_UI_GL_SHADER_H__
# define __YUNI_UI_GL_SHADER_H__

# include <yuni/yuni.h>
# include <yuni/core/dictionary.h>
# include <yuni/core/smartptr.h>
# include <yuni/core/string.h>
# include <yuni/core/noncopyable.h>



namespace Yuni
{
namespace Gfx3D
{

	//! Forward declaration
	class ShaderProgram;


	/*!
	** \brief Shader loading
	*/
	class IShader : public NonCopyable<IShader>
	{
	public:
		//! ID type
		typedef uint  ID;
		enum
		{
			invalidID = (uint) -1,
		};

	public:
		//! Constructor
		IShader();
		# ifdef YUNI_HAS_CPP_MOVE
		IShader(IShader&& rhs);
		# endif
		//! Virtual destructor
		virtual ~IShader() {}

		//! Load the source from file
		virtual bool loadFromFile(const AnyString& filePath) = 0;

		//! Load the source from memory
		virtual bool loadFromMemory(const AnyString& source) = 0;

		//! Is the shader valid ?
		bool valid() const;

		//! Get the ID
		uint id() const;

	protected:
		uint pID;

		//! Friend declaration : ShaderProgram
		friend class ShaderProgram;

	}; // class IShader





	/*!
	** \brief Vertex shader loading
	*/
	class VertexShader final : public IShader
	{
	public:
		// Smart pointer
		typedef SmartPtr<VertexShader>  Ptr;

		//! Dictionary
		typedef Dictionary<String, Ptr>::Hash  Map;

	public:
		//! Constructor
		VertexShader() {}
		VertexShader(const AnyString& filePath);
		//! Virtual destructor
		virtual ~VertexShader();

		virtual bool loadFromFile(const AnyString& filePath) override;
		virtual bool loadFromMemory(const AnyString& source) override;

	}; // class VertexShader


	/*!
	** \brief Fragment shader loading
	*/
	class FragmentShader final : public IShader
	{
	public:
		// Smart pointer
		typedef SmartPtr<FragmentShader>  Ptr;

		//! Dictionary
		typedef Dictionary<String, Ptr>::Hash  Map;

	public:
		//! Constructor
		FragmentShader() {}
		FragmentShader(const AnyString& filePath);
		//! Virtual destructor
		virtual ~FragmentShader();

		virtual bool loadFromFile(const AnyString& filePath) override;
		virtual bool loadFromMemory(const AnyString& source) override;

	}; // class FragmentShader


	/*!
	** \brief Compute shader loading
	*/
	class ComputeShader final : public IShader
	{
	public:
		// Smart pointer
		typedef SmartPtr<ComputeShader>  Ptr;

	public:
		//! Constructor
		ComputeShader() {}
		ComputeShader(const AnyString& filePath);
		//! Virtual destructor
		virtual ~ComputeShader();

		virtual bool loadFromFile(const AnyString& filePath) override;
		virtual bool loadFromMemory(const AnyString& source) override;

	}; // class ComputeShader






} // namespace Gfx3D
} // namespace Yuni


#include "shader.hxx"


#endif // __YUNI_UI_GL_SHADER_H__
