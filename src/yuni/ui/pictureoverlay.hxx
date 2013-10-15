#ifndef __YUNI_UI_PICTUREOVERLAY_HXX__
# define __YUNI_UI_PICTUREOVERLAY_HXX__


namespace Yuni
{
namespace UI
{


	inline PictureOverlay::PictureOverlay():
		pTexture(nullptr),
		pVisible(true),
		pX(0),
		pY(0),
		pWidth(1),
		pHeight(1)
	{
	}


	inline PictureOverlay::PictureOverlay(const Gfx3D::Texture::Ptr& texture, int x, int y, uint width, uint height):
		pTexture(texture),
		pVisible(true),
		pX(x),
		pY(y),
		pWidth(width),
		pHeight(height)
	{
	}


	inline PictureOverlay::~PictureOverlay()
	{}


	inline void PictureOverlay::texture(const Gfx3D::Texture::Ptr& texture)
	{
		pTexture = texture;
	}

	inline const Gfx3D::Texture::Ptr& PictureOverlay::texture() const
	{
		return pTexture;
	}


	inline void PictureOverlay::move(int x, int y)
	{
		pX = x;
		pY = y;
	}


	inline int PictureOverlay::x() const
	{
		return pX;
	}

	inline int PictureOverlay::y() const
	{
		return pY;
	}


	inline void PictureOverlay::resize(uint width, uint height)
	{
		pWidth = width;
		pHeight = height;
	}


	inline unsigned int PictureOverlay::width() const
	{
		return pWidth;
	}

	inline unsigned int PictureOverlay::height() const
	{
		return pHeight;
	}



} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_PICTUREOVERLAY_HXX__
