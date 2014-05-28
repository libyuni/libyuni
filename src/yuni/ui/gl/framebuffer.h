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
