# include "framebuffer.h"

namespace Yuni
{
namespace Gfx3D
{


	bool FrameBuffer::initialize()
	{
		// Screen texture 1
		pTexture = Texture::New(pSize.x, pSize.y, 4 /* RGBA */, nullptr, false);
		// Screen texture 2
		pBackTexture = Texture::New(pSize.x, pSize.y, 4 /* RGBA */, nullptr, false);
		::glBindTexture(GL_TEXTURE_2D, 0);

		// Depth buffer
		uint id;
		::glGenRenderbuffers(1, &id);
		::glBindRenderbuffer(GL_RENDERBUFFER, id);
		::glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, pSize.x, pSize.y);
		::glBindRenderbuffer(GL_RENDERBUFFER, 0);
		pDepth = id;

		// Framebuffer to link everything together
		::glGenFramebuffers(1, &id);
		::glBindFramebuffer(GL_FRAMEBUFFER, id);
		::glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTexture->id(), 0);
		::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, pDepth);
		::glBindFramebuffer(GL_FRAMEBUFFER, 0);
		pID = id;

		GLenum status;
		if ((status = ::glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "Framebuffer failed to load: error " << status << std::endl;
			pTexture = nullptr;
			::glDeleteRenderbuffers(1, (uint*)&pDepth);
			pDepth = -1;
			::glDeleteFramebuffers(1, (uint*)&pID);
			pID = -1;
			return false;
		}

		return true;
	}


	void FrameBuffer::resize(uint width, uint height)
	{
		// Update the size internally
		pSize(width, height);

		// If the FB is not properly initialized, just update the size internally
		// for future use by initialize(), but do nothing else
		if (!valid())
			return;

		// Resize the texture
		pTexture->resize(pSize.x, pSize.y);
		GLTestError("glTexSubImage2D() texture resizing");
		::glBindRenderbuffer(GL_RENDERBUFFER, pDepth);
		::glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, pSize.x, pSize.y);
		::glBindRenderbuffer(GL_RENDERBUFFER, 0);
		GLTestError("glRenderBufferStorage() resizing");
	}



} // namespace Gfx3D
} // namespace Yuni
