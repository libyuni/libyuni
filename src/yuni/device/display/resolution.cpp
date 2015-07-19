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
#include "resolution.h"
#include "../../core/hash/checksum/md5.h"
#include "../../core/math.h"
#include <cassert>


namespace Yuni
{
namespace Device
{
namespace Display
{

	Resolution::Resolution(const Resolution& rhs)
		: pWidth(rhs.pWidth)
		, pHeight(rhs.pHeight)
		, pBitsPerPixel(rhs.pBitsPerPixel)
	{}


	Resolution::Resolution(uint width, uint height, uint8 bits)
	{
		assert(width  <= (uint) maximumWidth
			and "display resolution: width greater than Device::Display::Resolution::maximumWidth");
		assert(height <= (uint) maximumHeight
			and "display resolution: height greater than Device::Display::Resolution::maximumHeight");

		pBitsPerPixel = (32 == bits ||  24 == bits || 16 == bits || 8 == bits)
			? (uint8) bits : (uint8)32;
		pWidth  = Math::MinMax<uint>(width,  minimumWidth,  maximumWidth);
		pHeight = Math::MinMax<uint>(height, minimumHeight, maximumHeight);
	}


	String Resolution::toString() const
	{
		String ret;
		ret << pWidth << 'x' << pHeight;
		if (pBitsPerPixel)
			ret << " (" << (int)pBitsPerPixel << "Bits)";
		return ret;
	}


	bool Resolution::operator < (const Resolution& rhs) const
	{
		if (pWidth < rhs.pWidth)
			return true;

		if (pWidth == rhs.pWidth)
		{
			if (pHeight < rhs.pHeight)
				return true;
			if (pHeight == rhs.pHeight)
				return (pBitsPerPixel < rhs.pBitsPerPixel);
		}
		return false;
	}


	bool Resolution::operator > (const Resolution& rhs) const
	{
		if (pWidth > rhs.pWidth)
			return true;

		if (pWidth == rhs.pWidth)
		{
			if (pHeight > rhs.pHeight)
				return true;
			if (pHeight == rhs.pHeight)
				return (pBitsPerPixel > rhs.pBitsPerPixel);
		}
		return false;
	}


	std::ostream& Resolution::print(std::ostream& out) const
	{
		out << pWidth << "x" << pHeight;
		if (pBitsPerPixel)
			out << " (" << (int) pBitsPerPixel << "Bits)";
		return out;
	}


	Resolution& Resolution::operator = (const Resolution& p)
	{
		pWidth = p.pWidth;
		pHeight = p.pHeight;
		pBitsPerPixel = p.pBitsPerPixel;
		return (*this);
	}




} // namespace Display
} // namespace Device
} // namespace Yuni

