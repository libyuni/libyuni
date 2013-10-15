#ifndef __YUNI_UI_TEXTOVERLAY_H__
# define __YUNI_UI_TEXTOVERLAY_H__

# include "../yuni.h"
# include "../core/smartptr.h"
# include "../core/string.h"
# include "../core/color/rgb.h"
# include "../core/color/rgba.h"
# include <vector>
# include "ftfont.h"
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
		TextOverlay(const AnyString& text, int x, int y, const Color::RGBA<float>& color);

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
		uint width() const;

		//! Current height of the internal texture
		uint height() const;

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

		//! Move the text to another position
		void move(int x, int y);

		//! Get the X coordinate
		int x() const;
		//! Get the Y coordinate
		int y() const;

	private:
		//! Text to display
		String pText;

		//! Store previous text to avoid resizing and updating texture for naught
		String pPrevText;

		//! Rendered texture
		Gfx3D::Texture::Ptr pTexture;

		//! X coordinate for the left of the text (in pixels, 0 -> ScreenWidth = left -> right)
		int pX;

		//! Y coordinate for the bottom of the text (in pixels, 0 -> ScreenHeight = top -> bottom)
		int pY;

		//! Font to use
		FTFont::Ptr pFont;

		//! Color to use
		Color::RGBA<float> pColor;

		//! Material
		Gfx3D::MaterialSurface::Ptr pMaterial;

		//! Anti-alias the text ?
		bool pAntiAliased;

		//! Track modification
		bool pModified;
	};


} // namespace UI
} // namespace Yuni

# include "textoverlay.hxx"

#endif // __YUNI_UI_TEXTOVERLAY_H__
