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

		//! Draw the given text to the given texture using this font
		virtual void draw(const AnyString&, Gfx3D::Texture::Ptr&, bool useKerning = true) const = 0;

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
		//! This might end up only be useful for the GL implementation
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
