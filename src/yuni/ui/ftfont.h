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
#ifndef __YUNI_UI_FTFONT_H__
# define __YUNI_UI_FTFONT_H__

# include "../yuni.h"
# include "font.h"
# include "gl/texture.h"

namespace Yuni
{
namespace UI
{


	//! Forward declaration
	struct FTFontImpl;


	/*!
	** \brief Freetype font implementation
	*/
	class FTFont: public Font
	{
	public:
		//! Smart pointer
		typedef SmartPtr<FTFont>  Ptr;

	public:
		//! Constructor
		FTFont(const AnyString& name, float size, bool bold = false, bool italic = false, bool underline = false, bool strikeout = false);

		//! Virtual destructor
		virtual ~FTFont();

		/*!
		** \brief Draw the given text using this font to the given texture
		** \note Expect the texture to be resized and cleared
		*/
		virtual void draw(const AnyString& text, Gfx3D::Texture::Ptr& texture,
			bool antiAliased = true, bool useKerning = true, uint tabWidth = 4u) const override;

		/*!
		** \brief Measure a text drawn with this font
		** \see TextMeasurements
		*/
		virtual void measure(const AnyString& text, uint& width, uint& height, int& ascent, int& descent,
			bool useKerning = true, uint tabWidth = 4u) const override;
		//! Initialize the font
		bool initialize();

		//! Reset the font cache
		void reset();

		//! Is the font valid ?
		bool valid() const;

	private:
		//! PImpl pattern (to avoid freetype dependencies in the header)
		FTFontImpl* pImpl;

	}; // class FTFont



} // namespace UI
} // namespace Yuni


#endif // __YUNI_UI_FTFONT_H__
