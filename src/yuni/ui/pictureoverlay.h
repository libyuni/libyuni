#ifndef __YUNI_UI_PICTUREOVERLAY_H__
# define __YUNI_UI_PICTUREOVERLAY_H__

# include "../yuni.h"
# include "../core/smartptr.h"
# include <vector>
# include "gl/shaderprogram.h"
# include "gl/texture.h"

namespace Yuni
{
namespace UI
{

	/*!
	** \brief A picture overlay is a 2D picture meant to be displayed on screen over a 3D view
	*/
	class PictureOverlay
	{
	public:
		enum Display
		{
			// Display the image aligned on the top-left of the overlay.
			// The image may be cropped or parts of the overlay may be left empty (transparent).
			// This is equivalent to Offset with `offsetX` and `offsetY` equal to 0.
			podNone,

			// Grow / reduce the image to fill the overlay, maintaining aspect ratio.
			// The image may be cropped
			podFill,

			// Grow / reduce the image to fit the overlay, maintaining aspect ratio.
			// Parts of the overlay may be left empty (transparent)
			podFit,

			// Stretch the image on both dimensions to fit the overlay, losing aspect ratio.
			podStretch,

			// Center the image in the overlay.
			// The image may be cropped or parts of the overlay may be left empty (transparent).
			podCenter,

			// Offset the image to display a part of it in the overlay, uses `offsetX` and `offsetY`
			// The image may be cropped or parts of the overlay may be left empty (transparent).
			podOffset
		};

	public:
		//! Smart pointer
		typedef SmartPtr<PictureOverlay>  Ptr;
		//! Vector
		typedef std::vector<Ptr>  Vector;

	public:
		//! Empty constructor
		PictureOverlay();

		//! Constructor. Passing 0 as a dimension indicates to use the value from the texture.
		PictureOverlay(const Gfx3D::Texture::Ptr& texture, int x, int y, uint width = 0, uint height = 0);

		//! Destructor
		~PictureOverlay();

		//! Draw the picture
		void draw(const Gfx3D::ShaderProgram::Ptr& shader) const;

		//! Modify the picture data
		void texture(const Gfx3D::Texture::Ptr& texture);
		//! Get the picture data
		const Gfx3D::Texture::Ptr& texture() const;

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

		//! Enable visibility
		void show() { pVisible = true; }
		//! Disable visibility
		void hide() { pVisible = false; }
		//! Set visibility
		void show(bool visible) { pVisible = visible; }
		//! Get visibility
		bool visible() const { return pVisible; }

		//! Get the fill color
		const Color::RGBA<float>& fillColor() const { return pFillColor; }
		//! Set the fill color
		void fillColor(const Color::RGBA<float>& color) { pFillColor = color; }

		//! Current display mode
		Display display() const { return pDisplay; }
		//! Set display mode to Fit
		void fit() { pDisplay = podFit; }
		//! Set display mode to Fill
		void fill() { pDisplay = podFill; }
		//! Set display mode to Center
		void center() { pDisplay = podCenter; }
		//! Set display mode to Stretch
		void stretch() { pDisplay = podStretch; }
		//! Set display mode to Offset and set offset values
		void offset(int x, int y) { pDisplay = podOffset; pOffsetX = x; pOffsetY = y; }
		//! Get current offset (might not be used if display mode is not podOffset)
		Point2D<int> offset() const { return Point2D<int>(pOffsetX, pOffsetY); }

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

		//! Offset of the overlay over the image in X (only used in Offset display)
		int pOffsetX;

		//! Offset of the overlay over the image in Y (only used in Offset display)
		int pOffsetY;

		//! Fill color for when a part of the overlay is empty
		Color::RGBA<float> pFillColor;

		//! Type of display of the image inside the overlay
		Display pDisplay;
	};


} // namespace UI
} // namespace Yuni

# include "pictureoverlay.hxx"

#endif // __YUNI_UI_PICTUREOVERLAY_H__
