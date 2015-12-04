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
		pHeight(1),
		pOffsetX(0),
		pOffsetY(0),
		pFillColor(0.0f, 0.0f, 0.0f, 0.0f), // Full transparent
		pDisplay(dmNone)
	{}


	inline PictureOverlay::PictureOverlay(const Gfx3D::Texture::Ptr& texture, int x, int y, uint width, uint height):
		pTexture(texture),
		pVisible(true),
		pX(x),
		pY(y),
		// Resize to the texture's size if `width` or `height` are left null
		pWidth(width ? width : !!texture ? texture->width() : 1),
		pHeight(height ? height : !!texture ? texture->height() : 1),
		pOffsetX(0),
		pOffsetY(0),
		pFillColor(0.0f, 0.0f, 0.0f, 0.0f), // Full transparent
		pDisplay(dmNone)
	{}


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

	inline void PictureOverlay::scale(float factor)
	{
		pWidth = (uint)((float)pWidth * factor);
		pHeight = (uint)((float)pHeight * factor);
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
