#ifndef __YUNI_GFX3D_SHADER_H__
# define __YUNI_GFX3D_SHADER_H__

# include <yuni/yuni.h>
# include <yuni/core/dictionary.h>
# include <yuni/core/smartptr.h>
# include <yuni/core/string.h>

namespace Yuni
{
namespace Gfx3D
{

	//! Forward declaration
	class ShaderProgram;


	/*!
	** \brief Shader loading
	*/
	class IShader
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
		IShader(): pID((uint) -1)
		{}
		//! Virtual destructor
		virtual ~IShader() {}

		//! Load the source from file
		virtual bool loadFromFile(const AnyString& filePath) = 0;

		//! Load the source from memory
		virtual bool loadFromMemory(const AnyString& source) = 0;

		//! Is the shader valid ?
		bool valid() const { return pID != (uint) -1; }

		//! Get the ID
		uint id() const { return pID; }

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

#endif // __YUNI_GFX3D_SHADER_H__
