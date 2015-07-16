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
#ifndef __YUNI_UI_DRAWINGSURFACE_H__
# define __YUNI_UI_DRAWINGSURFACE_H__

# include "../../yuni.h"
# include "../../core/string.h"
# include "../../core/color/rgba.h"
# include "../../core/point2D.h"
# include <cassert>
# include "../ftfont.h"
# include "texture.h"
# include "../displaymode.h"

namespace Yuni
{
namespace UI
{

	//! Forward declaration
	class DrawingSurfaceImpl;


	/*!
	** \brief A rectangle surface for 2D drawing
	*/
	class DrawingSurface
	{
	public:
		//! Smart pointer
		typedef SmartPtr<DrawingSurface>  Ptr;


	public:
		//! Constructor to an empty transparent surface
		DrawingSurface(float width, float height);

		//! Destructor
		~DrawingSurface();

		//! Begin modifications on the surface
		void begin();

		//! Apply modifications on the surface
		void commit();

		//! Discard modifications on the surface
		void rollback();

		//! Clear the surface
		void clear();

		//! Resize the surface
		void resize(float width, float height);

		//! Draw text from a given coordinate
		void drawText(const String& text, const FTFont::Ptr& font, const Color::RGBA<float>& color,
			float x, float y, uint tabWidth = 4u, bool drawOnBaseline = false);

		//! Draw text from a given coordinate with a given background color
		void drawTextOnColor(const String& text, const FTFont::Ptr& font,
			const Color::RGBA<float>& color, const Color::RGBA<float>& backColor, float x, float y,
			uint tabWidth = 4u);

		//! Draw text centered in a rectangle
		void drawTextInRect(const String& text, const FTFont::Ptr& font,
			const Color::RGBA<float>& color, float x, float y, float width, float height,
			uint tabWidth = 4u);

		//! Draw text centered in a rectangle with a given background color
		void drawTextOnColorInRect(const String& text, const FTFont::Ptr& font,
			const Color::RGBA<float>& color, const Color::RGBA<float>& backColor,
			float x, float y, float width, float height, uint tabWidth = 4u);

		//! Draw an anti-aliased line on the surface
		void drawLine(const Color::RGBA<float>& color, float startX, float startY,
			float endX, float endY, float lineWidth);

		//! Draw a line anti-aliased with a given background color
		void drawLine(const Color::RGBA<float>& color, const Color::RGBA<float>& bgColor,
			float startX, float startY, float endX, float endY, float lineWidth);

		//! Draw an empty rectangle (borders only)
		void drawRectangle(const Color::RGBA<float>& frontColor,
			const Color::RGBA<float>& backColor, float x, float y, float width, float height,
			float lineWidth);

		//! Draw a rectangle filled with a background color on the surface
		void drawFilledRectangle(const Color::RGBA<float>& frontColor,
			const Color::RGBA<float>& backColor, float x, float y, float width, float height,
			float lineWidth);

		/*!
		** \brief Draw a rectangular image
		**
		** \warning `imageOpacity` is used only for the image, the alpha from `fillColor` is used for out of bounds opacity
		*/
		void drawImage(const Gfx3D::Texture::Ptr& texture, float x, float y,
			float width, float height, const Color::RGBA<float>& fillColor,
			DisplayMode dispMode = dmNone,
			float offsetX = 0, float offsetY = 0, float imageOpacity = 1.0f);

		//! Fill with a color. Use clipping to control the filled area
		void fill(const Color::RGBA<float>& color);

		//! Activate clipping inside a rectangle
		void beginRectangleClipping(float x, float y, float width, float height);

		//! Disable last clipping, must be called in pair with begin*Clipping
		void endClipping();

		//! Get the texture containing the rendering of this surface
		const Gfx3D::Texture::Ptr& texture() const;

		//! Get the current width of the surface
		float width() const;

		//! Get the current height of the surface
		float height() const;

	private:
		//! Update the texture according to the changes in the surface
		void updateTexture();

	private:
		DrawingSurfaceImpl* pImpl;

	}; // class DrawingSurface


} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_DRAWINGSURFACE_H__
