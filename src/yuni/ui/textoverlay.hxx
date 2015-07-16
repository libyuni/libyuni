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
#ifndef __YUNI_UI_TEXTOVERLAY_HXX__
# define __YUNI_UI_TEXTOVERLAY_HXX__

# include "theme.h"


namespace Yuni
{
namespace UI
{


	inline TextOverlay::TextOverlay():
		pText(),
		pPrevText(),
		pTexture(nullptr),
		pX(0),
		pY(0),
		pAscent(0),
		pFont(Theme::Current()->font),
		// White by default
		pColor(1.0f, 1.0f, 1.0f, 1.0f),
		pMaterial(new Gfx3D::MaterialSurfaceColored("TextColor", 1.0f, 1.0f, 1.0f)),
		pAntiAliased(true),
		pTabWidth(4u),
		pModified(true),
		pDrawOnBaseline(false)
	{
	}


	inline TextOverlay::TextOverlay(const FTFont::Ptr& font):
		pText(),
		pPrevText(),
		pTexture(nullptr),
		pX(0),
		pY(0),
		pAscent(0),
		pFont(font),
		// White by default
		pColor(1.0f, 1.0f, 1.0f, 1.0f),
		pMaterial(new Gfx3D::MaterialSurfaceColored("TextColor", 1.0f, 1.0f, 1.0f)),
		pAntiAliased(true),
		pTabWidth(4u),
		pModified(true),
		pDrawOnBaseline(false)
	{
	}


	inline TextOverlay::TextOverlay(const AnyString& text, float x, float y, const Color::RGBA<float>& color):
		pText(text),
		pPrevText(),
		pTexture(nullptr),
		pX(x),
		pY(y),
		pAscent(0),
		pFont(Theme::Current()->font),
		pColor(color),
		pMaterial(new Gfx3D::MaterialSurfaceColored("TextColor", color)),
		pAntiAliased(true),
		pTabWidth(4u),
		pModified(true),
		pDrawOnBaseline(false)
	{
	}


	inline TextOverlay::~TextOverlay()
	{
	}



	inline TextOverlay& TextOverlay::operator = (const AnyString& newText)
	{
		pModified = true;
		pPrevText = pText;
		pText = newText;
		return *this;
	}

	inline String& TextOverlay::operator << (const AnyString& newText)
	{
		pModified = true;
		pPrevText = pText;
		return pText << newText;
	}


	inline String& TextOverlay::clear()
	{
		pModified = true;
		pPrevText = pText;
		return pText.clear();
	}


	inline String& TextOverlay::text()
	{
		// We are not sure that it will be modified, but it will probably be
		pModified = true;
		pPrevText = pText;
		return pText;
	}

	inline const String& TextOverlay::text() const
	{
		return pText;
	}


	inline void TextOverlay::font(const UI::FTFont::Ptr& font)
	{
		if (font != pFont)
		{
			pModified = true;
			pFont = font;
		}
	}

	inline const UI::FTFont::Ptr& TextOverlay::font() const
	{
		return pFont;
	}


	inline float TextOverlay::width() const
	{
		return nullptr != pTexture ? (float)pTexture->width() : 0.0f;
	}


	inline float TextOverlay::height() const
	{
		return nullptr != pTexture ? (float)pTexture->height() : 0.0f;
	}


	inline void TextOverlay::color(float r, float g, float b)
	{
		pModified = true;
		pColor.assign(r, g, b);
		pMaterial = new Gfx3D::MaterialSurfaceColored("TextColor", r, g, b);
	}

	inline void TextOverlay::color(float r, float g, float b, float a)
	{
		pModified = true;
		pColor.assign(r, g, b, a);
		pMaterial = new Gfx3D::MaterialSurfaceColored("TextColor", r, g, b, a);
	}

	inline void TextOverlay::color(const Color::RGB<float>& color)
	{
		pModified = true;
		pColor = color;
		pMaterial = new Gfx3D::MaterialSurfaceColored("TextColor", color);
	}

	inline void TextOverlay::color(const Color::RGBA<float>& color)
	{
		pModified = true;
		pColor = color;
		pMaterial = new Gfx3D::MaterialSurfaceColored("TextColor", color);
	}


	inline bool TextOverlay::antiAliased() const
	{
		return pAntiAliased;
	}

	inline void TextOverlay::antiAliased(bool newValue)
	{
		pAntiAliased = newValue;
	}


	inline void TextOverlay::texture(const Gfx3D::Texture::Ptr& texture)
	{
		pMaterial = new Gfx3D::MaterialSurfaceTextured(texture);
	}


	inline void TextOverlay::move(float x, float y)
	{
		pX = x;
		pY = y;
	}

	inline float TextOverlay::x() const
	{
		return pX;
	}

	inline float TextOverlay::y() const
	{
		return pY;
	}



} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_TEXTOVERLAY_HXX__
