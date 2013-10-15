#ifndef __YUNI_GFX3D_FRAMEBUFFER_HXX__
# define __YUNI_GFX3D_FRAMEBUFFER_HXX__

# include <yuni/private/graphics/opengl/glew/glew.h>
# include <cassert>
# include "glerror.h"
# include "texture.h"

namespace Yuni
{
namespace Gfx3D
{



	inline FrameBuffer::FrameBuffer():
		pID(-1),
		pTexture(nullptr),
		pDepth(-1),
		pSize(1, 1)
	{}


	inline FrameBuffer::FrameBuffer(uint width, uint height):
		pID(-1),
		pTexture(nullptr),
		pDepth(-1),
		pSize(width, height)
	{}


	inline FrameBuffer::~FrameBuffer()
	{
		::glDeleteRenderbuffers(1, (uint*)&pDepth);
		::glDeleteFramebuffers(1, (uint*)&pID);
	}


	inline void FrameBuffer::activate() const
	{
		if (!valid())
			return;
		::glBindFramebuffer(GL_FRAMEBUFFER, pID);
		GLTestError("glBindFrameBuffer() binding");
	}


	inline void FrameBuffer::deactivate() const
	{
		// Unbind
		::glBindFramebuffer(GL_FRAMEBUFFER, 0);
		GLTestError("glBindFrameBuffer() unbinding");
	}


	inline void FrameBuffer::swap()
	{
		// Ping-pong !
		std::swap(pTexture, pBackTexture);
		::glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTexture->id(), 0);
	}


	inline uint FrameBuffer::width() const
	{
		return pSize.x;
	}


	inline uint FrameBuffer::height() const
	{
		return pSize.y;
	}


	inline const Texture::Ptr& FrameBuffer::texture() const
	{
		return pTexture;
	}


	inline bool FrameBuffer::valid() const
	{
		return pTexture >= 0 && pDepth >= 0 && pID >= 0;
	}



} // namespace Gfx3D
} // namespace Yuni

#endif // __YUNI_GFX3D_FRAMEBUFFER_HXX__
