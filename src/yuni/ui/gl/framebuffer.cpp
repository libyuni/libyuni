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
# include "framebuffer.h"

namespace Yuni
{
namespace Gfx3D
{

	static GLenum UsageToGLUsage(FrameBuffer::Usage usage)
	{
		switch (usage)
		{
			case FrameBuffer::fbDraw:
			case FrameBuffer::fbPingPong:
				return GL_DRAW_FRAMEBUFFER;
				break;
			case FrameBuffer::fbRead:
				return GL_READ_FRAMEBUFFER;
			case FrameBuffer::fbReadDraw:
				return GL_FRAMEBUFFER;
				break;
		}
		return GL_FRAMEBUFFER;
	}


	bool FrameBuffer::initialize(Usage usage, UI::MultiSampling::Type msType, Texture::DataType type)
	{
		pUsage = usage;
		if (UI::MultiSampling::msNone == msType)
		{
			// Screen texture 1
			pTexture = Texture::New(pSize.x, pSize.y, 4 /* RGBA */, type, nullptr, false);
			if (fbPingPong == pUsage)
				// Screen texture 2
				pBackTexture = Texture::New(pSize.x, pSize.y, 4 /* RGBA */, type, nullptr, false);
		}
		else
		{
			uint samples = UI::MultiSampling::Multiplier(msType);
			// Screen texture 1
			pTexture = Texture::NewMS(pSize.x, pSize.y, 4 /* RGBA */, type, samples, nullptr);
			if (fbPingPong == pUsage)
				// Screen texture 2
				pBackTexture = Texture::NewMS(pSize.x, pSize.y, 4 /* RGBA */, type, samples, nullptr);
		}

		auto textureType = UI::MultiSampling::msNone == msType ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE;
		// Unbind
		::glBindTexture(textureType, 0);

		// Depth buffer
		uint id;
		::glGenRenderbuffers(1, &id);
		::glBindRenderbuffer(GL_RENDERBUFFER, id);
		::glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, pSize.x, pSize.y);
		::glBindRenderbuffer(GL_RENDERBUFFER, 0);
		pDepth = id;

		GLenum frameBufferUsage = UsageToGLUsage(pUsage);

		// Framebuffer to link everything together
		::glGenFramebuffers(1, &id);
		GLTestError("glGenFramebuffers frame buffer creation");
		::glBindFramebuffer(frameBufferUsage, id);
		GLTestError("glBindFramebuffers frame buffer binding");
		::glFramebufferTexture2D(frameBufferUsage, GL_COLOR_ATTACHMENT0, textureType, pTexture->id(), 0);
		GLTestError("glFramebufferTexture2D frame buffer color attachment");
		::glFramebufferRenderbuffer(frameBufferUsage, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, pDepth);
		GLTestError("glFramebufferRenderbuffer depth buffer attachment");
		::glBindFramebuffer(frameBufferUsage, 0);
		pID = id;

		GLenum status;
		if ((status = ::glCheckFramebufferStatus(frameBufferUsage)) != GL_FRAMEBUFFER_COMPLETE)
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


	void FrameBuffer::activate() const
	{
		if (not valid())
			return;
		::glBindFramebuffer(UsageToGLUsage(pUsage), pID);
		GLTestError("glBindFrameBuffer() binding");
	}


	void FrameBuffer::deactivate() const
	{
		// Unbind
		::glBindFramebuffer(UsageToGLUsage(pUsage), 0);
		GLTestError("glBindFrameBuffer() unbinding");
	}



	void FrameBuffer::resize(uint width, uint height)
	{
		// Update the size internally
		pSize(width, height);

		// If the FB is not properly initialized, just update the size internally
		// for future use by initialize(), but do nothing else
		if (not valid())
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
