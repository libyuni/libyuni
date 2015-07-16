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
#ifndef __YUNI_UI_FONT_H__
# define __YUNI_UI_FONT_H__

# include "../yuni.h"
# include "../core/smartptr.h"
# include "../core/string.h"
# include "../core/color/rgba.h"
# include "gl/texture.h"

namespace Yuni
{
namespace UI
{


	/*!
	** \brief Factory for fonts
	*/
	class Font
	{
	public:
		//! Smart pointer
		typedef SmartPtr<Font>  Ptr;

	public:
		//! Constructor
		Font(const AnyString& name, float size, bool bold = false, bool italic = false, bool underline = false, bool strikeout = false):
			pID(0),
			pName(name),
			pSize(size > 0.0f ? size : 16.0f),
			pBold(bold),
			pItalic(italic),
			pUnderline(underline),
			pStrikeout(strikeout)
		{}

		//! Virtual destructor
		virtual ~Font()
		{}

		/*!
		** \brief Draw the given text using this font to the given texture
		** \note Expect the texture to be resized and cleared
		*/
		virtual void draw(const AnyString&, Gfx3D::Texture::Ptr&, bool antiAliased = true, bool useKerning = true, uint tabWidth = 4u) const = 0;

		/*!
		** \brief Measure a text drawn with this font
		** \see TextMeasurements
		*/
		virtual void measure(const AnyString&, uint& width, uint& height, int& ascent, int& descent,
			bool useKerning = true, uint tabWidth = 4u) const = 0;

		//! Is the font properly loaded and valid ?
		virtual bool valid() const { return true; }

		//! Get the font ID
		virtual uint id() const { return pID; }

		//! Font name
		const String& name() const { return pName; }

		//! Font absolute size (in points)
		float size() const { return pSize; }

		//! Font weight
		bool bold() const { return pBold; }

		//! Font is italic
		bool italic() const { return pItalic; }

		//! Font is underlined
		bool underline() const { return pUnderline; }

		//! Font is striked out
		bool strikeout() const { return pStrikeout; }

	protected:
		//! ID. !! This might end up only be useful for the GL implementation
		uint pID;

		//! Name of the font
		String pName;

		//! Size of the font (in points)
		float pSize;

		//! Is the font bold ?
		bool pBold;

		//! Is the font italic ?
		bool pItalic;

		//! Is the font underlined ?
		bool pUnderline;

		//! Is the font striked out ?
		bool pStrikeout;
	};

} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_FONT_H__
