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
#ifndef __YUNI_GFX3D_TEXTURE_H__
# define __YUNI_GFX3D_TEXTURE_H__

# include "../../yuni.h"
# include "../../core/string.h"

namespace Yuni
{
namespace Gfx3D
{


	/*!
	** \brief Texture loading and management
	*/
	class Texture
	{
	public:
		enum DataType
		{
			UInt8,
			Int8,
			UInt16,
			Int16,
			UInt32,
			Int32,
			Float,
		};

	public:
		//! Smart pointer
		typedef SmartPtr<Texture>  Ptr;

		//! unsigned int IDs for OpenGL
		typedef uint  ID;


	public:
		/*!
		** \brief Create a new 2D texture
		**
		** \note It is valid to pass `nullptr` for the `data` parameter to delay construction.
		** \note Call `update()` to set the data afterwards.
		*/
		static Texture::Ptr New(uint width, uint height, uint colorDepth, DataType type = UInt8,
			const uint8* data = nullptr, bool mipmaps = true);

		/*!
		** \brief Create a new 3D texture
		**
		** \note It is valid to pass `nullptr` for the `data` parameter to delay construction.
		** \note Call `update()` to set the data afterwards.
		*/
		static Texture::Ptr New3D(uint width, uint height, uint depth, uint colorDepth,
			DataType type = UInt8, const uint8* data = nullptr, bool mipmaps = true);

		//! Create a new 2D multisample texture
		static Texture::Ptr NewMS(uint width, uint height, uint colorDepth,
			DataType type = UInt8, uint samples = 1, const uint8* data = nullptr);

		//! Load texture from file
		static Texture::Ptr LoadFromFile(const AnyString& filePath, bool mipmaps = true);

		//! Load skybox from a .box file. Returns a cubemap texture
		static Texture::Ptr LoadFromBoxFile(const AnyString& filePath);

		//! Load texture from encoded data in memory
		static Texture::Ptr LoadFromMemory(uint length, const uint8* data);

		//! Release several OpenGL textures at once
		static void ReleaseGLTextures(uint nbTextures, uint* textures);

	public:
		static uint PixelStoreAlignment;

	public:
		//! Destructor
		~Texture();

		/*!
		** \brief Resize the texture.
		**
		** \warning Invalidates all data in the texture !
		*/
		void resize(uint width, uint height);

		//! Update all the data for this texture
		void update(const unsigned char* data);

		//! Update the data for this texture
		void update(uint offsetX, uint offsetY, uint width, uint height, uint colorDepth, const uint8* data);

		//! Clear the whole texture to (0,0,0,0)
		void clear();
		//! Clear the given area of the texture to (0,0,0,0)
		void clear(uint offsetX, uint offsetY, uint width, uint height);
		//! Clear the whole texture to (1,1,1,1)
		void clearToWhite();

		//! Get the texture ID
		ID id() const;

		//! Texture width
		uint width() const;

		//! Texture height
		uint height() const;

		//! Texture depth
		uint depth() const;

		//! Texture color depth (in bytes per pixel)
		uint colorDepth() const;

		//! Data type
		DataType type() const;

	private:
		//! Private empty constructor
		Texture();

		//! Private copy constructor
		Texture(const Texture&);

		//! Private constructor that takes ownership of an already-loaded texture
		Texture(ID id, uint width, uint height, uint colorDepth, DataType type);

		//! Private constructor that takes ownership of a 3D already-loaded texture
		Texture(ID id, uint width, uint height, uint depth, uint colorDepth, DataType type);

	private:
		//! GL Texture ID
		ID pID;

		//! Texture width
		uint pWidth;

		//! Texture height
		uint pHeight;

		//! Texture depth (3D-texture only !)
		uint pDepth;

		//! Texture color depth (in bytes per pixel)
		uint pColorDepth;

		//! Data type
		DataType pType;

		//! Does the texture have mipmaps ?
		bool pMipmapped;

	}; // class Texture



} // namespace Gfx3D
} // namespace Yuni

#endif // __YUNI_GFX3D_TEXTURE_H__
