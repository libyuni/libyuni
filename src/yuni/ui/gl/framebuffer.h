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
#ifndef __YUNI_GFX3D_FRAMEBUFFER_H__
# define __YUNI_GFX3D_FRAMEBUFFER_H__

# include "../../yuni.h"
# include "../../core/smartptr.h"
# include "../../core/point2D.h"
# include "../multisampling.h"
# include "texture.h"

namespace Yuni
{
namespace Gfx3D
{

	/*!
	** \brief A frame buffer is a virtual screen used for render-to-texture
	**
	** This frame buffer can render back and forth between 2 textures (ping-pong texturing)
	** This is useful for multiple postprocessing filters, but requires 2 full-screen-sized textures
	*/
	class FrameBuffer
	{
	public:
		//! Smart pointer
		typedef SmartPtr<FrameBuffer>  Ptr;

		//! Frame buffer ID
		typedef int  ID;

	public:
		//! Type of usage for this framebuffer
		enum Usage
		{
			fbDraw,
			fbPingPong,
			fbRead,
			fbReadDraw
		};

	public:
		/*!
		** \brief Constructor
		*/
		FrameBuffer();
		/*!
		** \brief Constructor
		**
		** \param width Pixel width of the frame buffer
		** \param height Pixel height of the frame buffer
		*/
		FrameBuffer(uint width, uint height);

		//! Destructor
		~FrameBuffer();

		/*!
		** \brief Initialize the frame buffer
		*/
		bool initialize(Usage usage, UI::MultiSampling::Type msType = UI::MultiSampling::msNone, Texture::DataType type = Texture::UInt8);

		//! Activate the framebuffer
		void activate() const;

		//! Deactivate the framebuffer
		void deactivate() const;

		/*!
		** \brief Swap front and back textures (for ping-pong rendering)
		**
		** \warning The frame buffer must be activated !
		*/
		void swap();

		//! Resize the framebuffer and the underlying texture.
		void resize(uint width, uint height);

		//! \name Accessors
		//@{
		//! Framebuffer width
		uint width() const;

		//! Framebuffer height
		uint height() const;

		//! Texture, nullptr if none
		const Texture::Ptr& texture() const;

		//! Is the frame buffer valid for rendering ?
		bool valid() const;
		//@}

	private:
		//! ID of the frame buffer
		ID pID;

		//! Underlying texture : target of the framebuffer rendering
		Texture::Ptr pTexture;

		//! Back texture, for ping-pong rendering
		Texture::Ptr pBackTexture;

		//! Depth buffer linked to the frame buffer
		ID pDepth;

		//! Buffer size
		Point2D<uint> pSize;

		//! Usage of the framebuffer
		Usage pUsage;

	}; // class FrameBuffer



} // namespace Gfx3D
} // namespace Yuni

# include "framebuffer.hxx"

#endif // __YUNI_GFX3D_FRAMEBUFFER_H__
