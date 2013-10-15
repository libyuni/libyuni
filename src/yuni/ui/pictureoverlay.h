#ifndef __YUNI_UI_PICTUREOVERLAY_H__
# define __YUNI_UI_PICTUREOVERLAY_H__

# include "../yuni.h"
# include "../core/smartptr.h"
# include "../core/color/rgb.h"
# include "../core/color/rgba.h"
# include <vector>
# include "gl/shaderprogram.h"
# include "gl/texture.h"

namespace Yuni
{
namespace UI
{

	/*!
	** \brief A picture overlay is a 2D picture that is displayed on screen on top of the 3D view
	*/
	class PictureOverlay
	{
	public:
		//! Smart pointer
		typedef SmartPtr<PictureOverlay>  Ptr;
		//! Vector
		typedef std::vector<Ptr>  Vector;

	public:
		//! Empty constructor
		PictureOverlay();

		//! Constructor
		PictureOverlay(const Gfx3D::Texture::Ptr& texture, int x, int y, unsigned int width, unsigned int height);

		//! Destructor
		~PictureOverlay();

		//! Draw the picture
		void draw(const Gfx3D::ShaderProgram::Ptr& shader) const;

		//! Modify the picture data
		void texture(const Gfx3D::Texture::Ptr& texture);
		//! Get the picture data
		const Gfx3D::Texture::Ptr& texture() const;

		//! Modify the transparent color used
		void color(const Color::RGB<float>& color);
		void color(const Color::RGBA<float>& color);
		//! Get the transparent color
		const Color::RGB<float>& color() const;

		//! Move the picture to another position
		void move(int x, int y);

		//! Get the X coordinate
		int x() const;
		//! Get the Y coordinate
		int y() const;

		//! Set the size of the picture
		void resize(uint width, uint height);

		//! Width of the picture (in pixels)
		uint width() const;
		//! Height of the picture (in pixels)
		uint height() const;

		void show() { pVisible = true; }
		void hide() { pVisible = false; }
		void show(bool visible) { pVisible = visible; }
		bool visible() const { return pVisible; }

	private:
		//! Picture to display
		Gfx3D::Texture::Ptr pTexture;

		/*!
		** \brief Is it hidden or visible ?
		**
		** \note This has nothing to do with being occluded, this is a mechanism to hide the overlay
		*/
		bool pVisible;

		//! X coordinate for the left of the picture (in pixels, left = 0, right = ScreenWidth)
		int pX;

		//! Y coordinate for the bottom of the picture (in pixels, top = 0, bottom = ScreenHeight)
		int pY;

		//! Width of the picture (in pixels)
		uint pWidth;

		//! Height of the picture (in pixels)
		uint pHeight;
	};


} // namespace UI
} // namespace Yuni

# include "pictureoverlay.hxx"

#endif // __YUNI_UI_PICTUREOVERLAY_H__
