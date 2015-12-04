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
#ifndef __YUNI_UI_TEXTOVERLAY_H__
# define __YUNI_UI_TEXTOVERLAY_H__

# include "../yuni.h"
# include "../core/smartptr.h"
# include "../core/string.h"
# include "../core/color/rgb.h"
# include "../core/color/rgba.h"
# include <vector>
# include "ftfont.h"
# include "pictureoverlay.h"
# include "gl/materialsurface.h"
# include "gl/shaderprogram.h"
# include "gl/texture.h"

namespace Yuni
{
namespace UI
{

	/*!
	** \brief A text overlay is a 2D line of text that is displayed on screen on top of the 3D view
	*/
	class TextOverlay
	{
	public:
		//! Smart pointer
		typedef SmartPtr<TextOverlay>  Ptr;
		//! Vector
		typedef std::vector<Ptr>  Vector;

	public:
		//! Empty constructor
		TextOverlay();

		//! Constructor
		TextOverlay(const FTFont::Ptr& font);

		//! Constructor
		TextOverlay(const AnyString& text, float x, float y, const Color::RGBA<float>& color);

		//! Destructor
		~TextOverlay();

		//! Replace the text in this overlay
		TextOverlay& operator = (const AnyString& newText);

		//! Concatenate more text in this overlay
		String& operator << (const AnyString& newText);

		//! Update the internal representation
		void update();

		//! Draw the text using the given shader
		void draw(const Gfx3D::ShaderProgram::Ptr& shader) const;
		void draw(const Gfx3D::ShaderProgram::Ptr& shader, const Color::RGB<float>& backColor) const;		void draw(const Gfx3D::ShaderProgram::Ptr& shader, const Color::RGBA<float>& backColor) const;

		//! Clear the text
		String& clear();

		//! Get the text
		String& text();
		const String& text() const;

		//! Modify the font used
		void font(const UI::FTFont::Ptr& font);
		//! Get the font
		const UI::FTFont::Ptr& font() const;

		//! Get the material
		const Gfx3D::MaterialSurface::Ptr& material() const;

		//! Current width of the internal texture
		float width() const;

		//! Current height of the internal texture
		float height() const;

		//! Text ascent (Y bearing = distance from the top of the text to the baseline)
		int ascent() const { return pAscent; }

		//! Modify the color used
		void color(float r, float g, float b);
		void color(float r, float g, float b, float a);
		void color(const Color::RGB<float>& color);
		void color(const Color::RGBA<float>& color);

		//! Apply a texture to the text
		void texture(const Gfx3D::Texture::Ptr& texture);

		//! Is the text anti-aliased ?
		bool antiAliased() const;
		//! Set text anti-aliasing
		void antiAliased(bool newValue);

		//! Number of space characters written for one tab character
		uint tabWidth() const { return pTabWidth; }
		void tabWidth(uint nbChars) { pTabWidth = nbChars; }

		//! Use the Y coordinate as the text baseline rather than the top of the text ?
		bool drawOnBaseline() const { return pDrawOnBaseline; }
		void drawOnBaseline(bool onBaseline) { pDrawOnBaseline = onBaseline; }

		//! Move the text to another position
		void move(float x, float y);

		//! Get the X coordinate
		float x() const;
		//! Get the Y coordinate
		float y() const;

		//! Convert to a new picture overlay (e.g. for resizing)
		PictureOverlay::Ptr toPicture() const;

	private:
		//! Text to display
		String pText;

		//! Store previous text to avoid resizing and updating texture for naught
		String pPrevText;

		//! Rendered texture
		Gfx3D::Texture::Ptr pTexture;

		//! X coordinate for the left of the text (in pixels, 0 -> ScreenWidth = left -> right)
		float pX;

		//! Y coordinate for the bottom of the text (in pixels, 0 -> ScreenHeight = top -> bottom)
		float pY;

		//! Text ascent (Y bearing = distance from the top of the text to the baseline)
		int pAscent;

		//! Font to use
		FTFont::Ptr pFont;

		//! Text color
		Color::RGBA<float> pColor;

		//! Material
		Gfx3D::MaterialSurface::Ptr pMaterial;

		//! Anti-alias the text ?
		bool pAntiAliased;

		//! Tab width
		uint pTabWidth;

		//! Track modification
		bool pModified;

		//! Use the Y coordinate as the text baseline rather than the top of the text
		bool pDrawOnBaseline;
	};


} // namespace UI
} // namespace Yuni

# include "textoverlay.hxx"

#endif // __YUNI_UI_TEXTOVERLAY_H__
