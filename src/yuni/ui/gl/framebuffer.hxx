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
		pSize(1, 1),
		pUsage(fbDraw)
	{}


	inline FrameBuffer::FrameBuffer(uint width, uint height):
		pID(-1),
		pTexture(nullptr),
		pDepth(-1),
		pSize(width, height),
		pUsage(fbDraw)
	{}


	inline FrameBuffer::~FrameBuffer()
	{
		::glDeleteRenderbuffers(1, (uint*)&pDepth);
		::glDeleteFramebuffers(1, (uint*)&pID);
	}


	inline void FrameBuffer::swap()
	{
		assert(pUsage == fbPingPong && "FrameBuffer::swap() has no meaning when usage is not Ping-Pong !");

		// Ping-pong !
		std::swap(pTexture, pBackTexture);
		::glFramebufferTexture2D(pUsage, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTexture->id(), 0);
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
