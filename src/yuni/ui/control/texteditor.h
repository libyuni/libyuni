#ifndef __YUNI_UI_CONTROL_TEXTEDITOR_H__
# define __YUNI_UI_CONTROL_TEXTEDITOR_H__

# include "../../yuni.h"
# include "../../core/color/rgb.h"
# include "../../core/color/rgba.h"
# include "../../core/math.h"
# include "../../core/functional/view.h"
# include "control.h"
# include "../displaymode.h"
# include "../dimension.h"
# include "../font.h"
# include "../theme.h"
# include <deque>

namespace Yuni
{
namespace UI
{
namespace Control
{


	//! A text is an multi-line area for text edition
	class TextEditor: public IControl
	{
	public:
		//! Smart pointer
		typedef Ancestor::SmartPtrType<TextEditor>::Ptr  Ptr;

	public:
		TextEditor(float x, float y, float maxWidth, float maxHeight):
			IControl(x, y, maxWidth, maxHeight),
			pText(),
			pCursorPos(0u, 0u),
			pDragPos(0u, 0u),
			pFont(Theme::Current()->monoFont),
			// White by default
			pColor(Theme::Current()->textColor),
			pBackColor(Theme::Current()->windowColor),
			pAntiAliased(true),
			pTabWidth(4u),
			pTopLineNb(1u),
			pLineHeight(144_pcp), // 1.44 ratio
			pHorizMargin(15),
			pVertMargin(10)
		{
			// We use percent parent on the font size (height) for line height calculation
			pConversion.unitPerPercentParent = pFont->size() * 96.0f / 72.0f / 100.0f;
		}

		TextEditor(const Point2D<float>& position, const Point2D<float>& maxSize):
			IControl(position, maxSize),
			pText(),
			pCursorPos(0u, 0u),
			pDragPos(0u, 0u),
			pFont(Theme::Current()->monoFont),
			pColor(Theme::Current()->textColor),
			pBackColor(Theme::Current()->windowColor),
			pAntiAliased(true),
			pTabWidth(4u),
			pTopLineNb(1u),
			pLineHeight(144_pcp), // 1.44 ratio
			pHorizMargin(15),
			pVertMargin(10)
		{
			// We use percent parent on the font size (height) for line height calculation
			pConversion.unitPerPercentParent = pFont->size() * 96.0f / 72.0f / 100.0f;
		}

		//! Virtual destructor
		virtual ~TextEditor() {}

		//! Draw the panel
		virtual void draw(DrawingSurface::Ptr& surface, float xOffset, float yOffset) const override;

		//! Get the text
		const String& text() const { return pText; }
		void text(const AnyString& newText) { invalidate(); pTopLineNb = 1u; pCursorPos(); pDragPos(); pLines.clear(); pText = newText; }

		//! Modify the font used
		void font(const UI::FTFont::Ptr& font)
		{
			if (pFont != font)
			{
				pFont = font;
				pConversion.unitPerPercentParent = pFont->size() * 96.0f / 72.0f / 100.0f;
				invalidate();
			}
		}
		//! Get the font
		const UI::FTFont::Ptr& font() const { return pFont; }

		//! Modify the color used for text
		void color(float r, float g, float b) { pColor.assign(r, g, b); invalidate(); }
		void color(float r, float g, float b, float a) { pColor.assign(r, g, b, a); invalidate(); }
		void color(const Color::RGB<float>& color) { pColor = color; invalidate(); }
		void color(const Color::RGBA<float>& color) { pColor = color; invalidate(); }
		//! Modify the color used for background
		void backColor(float r, float g, float b) { pBackColor.assign(r, g, b); invalidate(); }
		void backColor(float r, float g, float b, float a) { pBackColor.assign(r, g, b, a); invalidate(); }
		void backColor(const Color::RGB<float>& color) { pBackColor = color; invalidate(); }
		void backColor(const Color::RGBA<float>& color) { pBackColor = color; invalidate(); }

		Point2D<uint> cursorPos() const { return pCursorPos; }
		void cursorPos(uint line, uint column)
		{
			if (pLines.empty() && !pText.empty())
				reloadLines();
			pCursorPos.x = Math::Min(Math::Max(1u, line), (uint)pLines.size());
			pCursorPos.y = Math::Min(column, lineSize(pLines[pCursorPos.x - 1]));
			invalidate();
		}
		void cursorPos(Point2D<uint> lineColumn) { cursorPos(lineColumn.x, lineColumn.y); invalidate(); }

		//! Is the text anti-aliased ?
		bool antiAliased() const { return pAntiAliased; }
		//! Set text anti-aliasing
		void antiAliased(bool newValue) { pAntiAliased = newValue; invalidate(); }

		uint tabWidth() const { return pTabWidth; }
		void tabWidth(uint nbChars) { pTabWidth = nbChars; invalidate(); }

		//! Get line height
		const Dimension& lineHeight() const { return pLineHeight; }
		//! Get line height in pixels
		float lineHeightPixels() const { return pLineHeight(pConversion); }
		//! Set line height
		void lineHeight(const Dimension& newValue) { pLineHeight = newValue; }
		//! Set line height in pixels
		void lineHeightPixels(float newValue) { pLineHeight.reset(newValue, Dimension::uPixel); }

		virtual EventPropagation keyDown(Input::Key key) override;
		virtual EventPropagation charInput(const AnyString& str) override;
		virtual EventPropagation mouseDown(Input::IMouse::Button btn, float x, float y) override;
		virtual EventPropagation mouseUp(Input::IMouse::Button btn, float x, float y) override;
		virtual EventPropagation mouseMove(float x, float y) override;
		virtual EventPropagation mouseScroll(float delta, float x, float y) override;

	private:
		//! Scroll by a number of lines, negative scrolls down, positive scrolls up
		void scroll(float nbLines);

		void reloadLines() const
		{
			pLines.clear();
			pText.words("\n", [&](const AnyString& line) -> bool
			{
				pLines.push_back(line);
				return true;
			}, true);
		}

		float displayedLineCount() const
		{
			return (pSize.y - pVertMargin) / pLineHeight(pConversion);
		}

		//! Number of chars displayed on this line, tabs converted to spaces
		uint lineSize(uint lineNb) const
		{
			if (pLines.empty() && !pText.empty())
				reloadLines();

			return lineSize(pLines[lineNb - 1]);
		}


		//! Number of chars displayed on this line, tabs converted to spaces
		uint lineSize(const AnyString& line) const
		{
			return makeView(line.utf8begin(), line.utf8end())
				.map([&](const AnyString::Char& c) -> uint
				{
					return c == '\t' ? pTabWidth : 1u;
				})
				.sum();
		}

		//! Convert an X coordinate in pixels to the corresponding text column
		uint XToColumn(float x) const
		{
			if (x - pHorizMargin <= 0)
				return 0u;
			// TODO : I need the real advance of the clicked text to find the proper column value
			return (uint)Math::Round((x - pHorizMargin) / (pFont->size() / 1.25f)); // For now, use a not-quite-random ratio
		}

		//! Convert a Y coordinate in pixels to the corresponding text line
		uint YToLine(float y) const
		{
			if (y - pVertMargin <= 0)
				return pTopLineNb;
			return uint((y - pVertMargin) / pLineHeight(pConversion)) + pTopLineNb;
		}

		//! Convert a column number on screen to the corresponding X coordinate in pixels
		float columnToX(uint col) const
		{
			// TODO : I need the real advance of the clicked text to find the proper column value
			return (float)col * ((float)pFont->size() / 1.25f) + (float)pHorizMargin; // For now, use a not-quite-random ratio
		}

		//! Convert a line number on screen to the corresponding Y coordinate in pixels
		float lineToY(uint line) const
		{
			return (float)(line - pTopLineNb) * pLineHeight(pConversion) + (float)pVertMargin;
		}

		//! Get the index in the text that corresponds to the current cursor position
		uint cursorToByte(const Point2D<uint>& cursor) const
		{
			uint index = 0u;
			uint lineNb = cursor.y;

			pText.words("\n", [&](const AnyString& line) -> bool
			{
				if (lineNb > 0)
				{
					--lineNb;
					index += line.size() + 1 /*\n*/;
					return true;
				}
				else
				{
					if (cursor.x > 0)
					{
						auto it = line.utf8begin();
						it += cursor.x;
						index += it.rawOffset();
					}
					return false;
				}
			}, true);
			return index;
		}


		void byteToCursor(Point2D<uint>& cursor, uint byte) const
		{
			if (byte > 0)
			{
				AnyString part(pText, 0, Math::Min(byte, pText.size()));
				assert(part.utf8valid() and "invalid UTF8 string part");

				cursor.y = part.countChar('\n');
				if (cursor.y > 0)
				{
					auto pos = part.rfind('\n');
					if (pos < part.size())
					{
						++pos;
						AnyString lastline(part, pos);
						assert(lastline.utf8valid() and "invalid UTF8 sub string");
						cursor.x = part.utf8size();
					}
					else
						cursor.x = 0;
				}
				else
					cursor.x = part.utf8size();
			}
			else
				cursor.reset();
		}


	private:
		//! Text to display
		Clob pText;

		//! Store AnyStrings on each line to facilitate edition
		mutable std::deque<AnyString> pLines;

		//! Position of the edition cursor, in lines and columns
		Point2D<uint> pCursorPos;

		/*!
		** \brief Position of the dragging cursor
		**
		** This is the same as pCursorPos when not dragging and no text is selected
		** This is the opposite end of the selection if dragging / some text is selected
		*/
		Point2D<uint> pDragPos;

		//! Currently dragging ?
		bool pDragging;


		//! Font to use
		FTFont::Ptr pFont;

		//! Text color
		Color::RGBA<float> pColor;

		//! Background color
		Color::RGBA<float> pBackColor;

		//! Anti-alias the text ?
		bool pAntiAliased;

		//! Tab width (in number of spaces)
		uint pTabWidth;

		//! Line number of the top-most displayed line (for scrolling)
		uint pTopLineNb;

		//! Height of a line in % of the font pixel size
		Dimension pLineHeight;

		//! Horizontal margin (pixels)
		float pHorizMargin;

		//! Vertical margin (pixels)
		float pVertMargin;

		//! Conversion data for font and text units
		ConversionData pConversion;

	}; // class TextEditor



} // namespace Control
} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_CONTROL_TEXTEDITOR_H__
