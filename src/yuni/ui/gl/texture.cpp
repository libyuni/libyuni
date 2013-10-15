
#include <yuni/yuni.h>
#include <yuni/core/logs.h>
#include <yuni/core/string.h>
#include <yuni/core/system/windows.hdr.h>
#include <yuni/private/graphics/opengl/glew/glew.h>
#include <fstream>
#include <climits>

// Lib for image loading
#include "../stb_image.h"

#include "glerror.h"
#include "texture.h"


namespace Yuni
{
namespace Gfx3D
{


	Texture::Ptr Texture::LoadFromFile(const AnyString& filePath)
	{
		int width;
		int height;
		int depth;

		uint8* data = ::stbi_load(filePath.c_str(), &width, &height, &depth, 0); // 0 means auto-detect
		if (!data)
			return nullptr;
		Texture::Ptr texture = Texture::New(width, height, depth, data, true);

		// free buffer
		::stbi_image_free(data);

		return texture;
	}


	Texture::Ptr Texture::LoadFromMemory(uint length, const uint8* data)
	{
		int width;
		int height;
		int depth;

		if (!data)
			return nullptr;
		uint8* texData = ::stbi_load_from_memory(data, length, &width, &height, &depth, 0); // 0 means auto-detect
		if (!texData)
			return nullptr;

		Texture::Ptr texture = Texture::New((uint)width, (uint)height, (uint)depth, texData, true);

		// free buffer
		::stbi_image_free(texData);

		return texture;
	}


	Texture::Ptr Texture::LoadFromBoxFile(const AnyString& filePath)
	{
		std::ifstream fd(filePath.c_str());

		if (!fd)
			return nullptr;

		Yuni::Logs::Logger<> logs;

		uint texture;
		// Allocate a texture name
		::glGenTextures(1, &texture);
		GLTestError("Texture::LoadFromBoxFile");

		// Select our current texture
		::glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		// when texture area is small, bilinear filter the first mipmap
		::glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// when texture area is large, bilinear filter the first mipmap
		::glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// the texture ends at the edges (clamp)
		::glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		::glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Read the textures one by one
		const GLenum CubeMapEnums[] =
		{
			// Top
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			// Bottom
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			// Left
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			// Right
			GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			// Back
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			// Front
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		};

		int width;
		int height;
		int depth;

		#ifdef YUNI_OS_WINDOWS
		char textureFile[MAX_PATH + 1];
		#else
		char textureFile[PATH_MAX + 1];
		#endif
		for (uint i = 0; i < 6; ++i)
		{
			fd >> textureFile;
			uint8* data = ::stbi_load(textureFile, &width, &height, &depth, 0);
			if (!data)
			{
				::glDeleteTextures(1, &texture);
				return nullptr;
			}
			int format = DepthToGLEnum(depth);
			::glTexImage2D(CubeMapEnums[i], 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			::stbi_image_free(data);
		}
		::glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return new Texture(texture, width, height, depth);
	}


	void Texture::ReleaseGLTextures(uint nbTextures, uint* textures)
	{
		::glDeleteTextures(nbTextures, textures);
	}


	int Texture::DepthToGLEnum(uint depth)
	{
		int format;
		switch (depth)
		{
			case 3:
			case 6:
				format = GL_RGB;
				break;
			case 4:
			case 8:
				format = GL_RGBA;
				break;
			case 1:
				format = GL_RED;
				break;
			case 2:
				format = GL_RG;
				break;
			default:
				format = GL_RGB;
				break;
		}
		return format;
	}


	static int DepthToGLEnumInternal(uint depth)
	{
		int format;
		switch (depth)
		{
			case 3:
			case 6:
				format = GL_RGB8;
				break;
			case 4:
			case 8:
				format = GL_RGBA8;
				break;
			case 1:
				format = GL_R8;
				break;
			case 2:
				format = GL_RG8;
				break;
			default:
				format = GL_RGB8;
				break;
		}
		return format;
	}


	Texture::Ptr Texture::New(uint width, uint height, uint colorDepth,
		const uint8* data, bool mipmaps)
	{
		ID id;

		// Allocate a texture name
		::glGenTextures(1, &id);

		// Select our current texture
		::glBindTexture(GL_TEXTURE_2D, id);

		// when texture area is small, bilinear filter and average with the 2 closest mipmaps
		::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmaps ?
			GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		// when texture area is large, bilinear filter the first mipmap
		::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// the texture ends at the edges (clamp)
		::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// Color fusion mode : only used when no shader is activated
		::glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// This Enable right here is necessary in cases where no shader is activated
		// !!! However, it is freakishly buggy ! If you do it on every texture load,
		// !!! it will break the video player (it displays levels of red...)
		// So yeah, only do it once.
		static bool first = true;
		if (first)
		{
			::glEnable(GL_TEXTURE_2D);
			first = false;
		}

		// Set the texture in OpenGL
		GLenum format = (GLenum)DepthToGLEnum(colorDepth);
		::glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		// Build our texture mipmaps
		if (mipmaps)
			::glGenerateMipmap(GL_TEXTURE_2D);

		return new Texture(id, width, height, colorDepth);
	}


	Texture::Ptr Texture::New3D(uint width, uint height, uint depth, uint colorDepth,
		const uint8* data, bool mipmaps)
	{
		ID id;

		// Allocate a texture name
		::glGenTextures(1, &id);

		// Select our current texture
		::glBindTexture(GL_TEXTURE_2D, id);

		// when texture area is small, bilinear filter and average with the 2 closest mipmaps
		::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmaps ?
			GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		// when texture area is large, bilinear filter the first mipmap
		::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// the texture ends at the edges (clamp)
		::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// Color fusion mode : only used when no shader is activated
		::glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// This Enable right here is necessary in cases where no shader is activated
		// !!! However, it is freakishly buggy ! If you do it on every texture load,
		// !!! it will break the video player (it displays levels of red...)
		// So yeah, only do it once.
		static bool first = true;
		if (first)
		{
			::glEnable(GL_TEXTURE_2D);
			first = false;
		}

		// Set the texture in OpenGL
		GLenum format = (GLenum)DepthToGLEnum(colorDepth);
		::glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		// Build our texture mipmaps
		if (mipmaps)
			::glGenerateMipmap(GL_TEXTURE_2D);

		return new Texture(id, width, height, depth);
	}


	Texture::Texture(ID id, uint width, uint height, uint depth):
		pID(id),
		pWidth(width),
		pHeight(height),
		pDepth(depth)
	{
	}


	Texture::~Texture()
	{
		::glDeleteTextures(1, &pID);
	}


	void Texture::resize(uint width, uint height)
	{
		// Update sizes
		pWidth = width;
		pHeight = height;
		// Bind texture
		::glBindTexture(GL_TEXTURE_2D, pID);
		int format = DepthToGLEnum(pDepth);
		int formatInt = DepthToGLEnumInternal(pDepth);
		// Sadly, glTexSubImage2D does not do the trick, we need glTexImage2D
		::glTexImage2D(GL_TEXTURE_2D, 0, formatInt, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
		GLTestError("glTexImage2D Texture resize");
	}


	void Texture::update(uint offsetX, uint offsetY, uint width, uint height, uint depth,
		const unsigned char* data)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		assert(offsetX + width <= pWidth && "Texture update : X + width is out of bounds !");
		assert(offsetY + height <= pHeight && "Texture update : Y + height is out of bounds !");
		::glBindTexture(GL_TEXTURE_2D, pID);
		int format = DepthToGLEnum(depth);
		pDepth = depth;
		::glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY, width, height, format, GL_UNSIGNED_BYTE, data);
		if (!GLTestError("glTexSubImage2D Texture update"))
			std::cerr << "On texture "<< pID << std::endl;
	}


	void Texture::clear()
	{
		std::vector<uint8> data(pWidth * pHeight * 4, 0);
		update(0, 0, pWidth, pHeight, pDepth, &data[0]);
	}


	void Texture::clear(uint offsetX, uint offsetY, uint width, uint height)
	{
		std::vector<uint8> data(width * height * 4, 0);
		update(offsetX, offsetY, width, height, pDepth, &data[0]);
	}

	void Texture::clearToWhite()
	{
		std::vector<uint8> data(pWidth * pHeight * 4, 1);
		update(0, 0, pWidth, pHeight, pDepth, &data[0]);
	}


	Texture::ID Texture::id() const
	{
		return pID;
	}


	uint Texture::width() const
	{
		return pWidth;
	}


	uint Texture::height() const
	{
		return pHeight;
	}


	uint Texture::colorDepth() const
	{
		return pDepth;
	}



} // namespace Gfx3D
} // namespace Yuni
