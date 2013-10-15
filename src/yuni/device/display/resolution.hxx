#ifndef __YUNI_DEVICE_DISPLAY_RESOLUTION_HXX__
# define __YUNI_DEVICE_DISPLAY_RESOLUTION_HXX__


namespace Yuni
{
namespace Device
{
namespace Display
{


	inline uint Resolution::width() const
	{
		return pWidth;
	}


	inline uint Resolution::height() const
	{
		return pHeight;
	}


	inline uint8 Resolution::bitPerPixel() const
	{
		return pBitsPerPixel;
	}


	inline bool Resolution::operator == (const Resolution& rhs) const
	{
		return rhs.pWidth == pWidth and rhs.pHeight == pHeight
			and rhs.pBitsPerPixel == pBitsPerPixel;
	}


	inline bool Resolution::operator != (const Resolution& rhs) const
	{
		return !(*this == rhs);
	}


	inline bool Resolution::operator <= (const Resolution& rhs) const
	{
		return pWidth <= rhs.pWidth and pHeight <= rhs.pHeight
			and pBitsPerPixel <= rhs.pBitsPerPixel;
	}


	inline bool Resolution::operator >= (const Resolution& rhs) const
	{
		return pWidth >= rhs.pWidth and pHeight >= rhs.pHeight
			and pBitsPerPixel >= rhs.pBitsPerPixel;
	}




} // namespace Display
} // namespace Device
} // namespace Yuni

#endif // __YUNI_DEVICE_DISPLAY_RESOLUTION_H__
