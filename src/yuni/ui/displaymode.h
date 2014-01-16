#ifndef __YUNI_UI_DISPLAYMODE_H__
# define __YUNI_UI_DISPLAYMODE_H__


namespace Yuni
{
namespace UI
{

	enum DisplayMode
	{
		// Display the image aligned on the top-left of the overlay.
		// The image may be cropped or parts of the overlay may be left empty (transparent).
		// This is equivalent to Offset with `offsetX` and `offsetY` equal to 0.
		dmNone,

		// Grow / reduce the image to fill the overlay, maintaining aspect ratio.
		// The image may be cropped
		dmFill,

		// Grow / reduce the image to fit the overlay, maintaining aspect ratio.
		// Parts of the overlay may be left empty (transparent)
		dmFit,

		// Stretch the image on both dimensions to fit the overlay, losing aspect ratio.
		dmStretch,

		// Center the image in the overlay.
		// The image may be cropped or parts of the overlay may be left empty (transparent).
		dmCenter,

		// Offset the image to display a part of it in the overlay, uses `offsetX` and `offsetY`
		// The image may be cropped or parts of the overlay may be left empty (transparent).
		dmOffset

	}; // enum DisplayMode



} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_DISPLAYMODE_H__
