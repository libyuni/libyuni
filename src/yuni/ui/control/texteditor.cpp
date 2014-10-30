#include "texteditor.h"
#include <locale>

namespace Yuni
{
namespace UI
{
namespace Control
{


	void TextEditor::draw(DrawingSurface::Ptr& surface, float xOffset, float yOffset) const
	{
		if (!pVisible)
			return;

		Point2D<float> pos(pPosition.x + xOffset, pPosition.y + yOffset);

		// Draw background
		surface->drawFilledRectangle(pBackColor, pBackColor, pos.x, pos.y, pSize.x, pSize.y, 0.0f);

		surface->beginRectangleClipping(pos.x, pos.y, pSize.x, pSize.y);

		// Draw the text
		float pixelLineHeight = pLineHeight(pConversion);
		float x = pos.x + pHorizMargin;
		float y = pos.y + pVertMargin;
		const uint lastVisibleLine = YToLine(pSize.y - pVertMargin);
		// Loop on lines of text
		uint lineNb = pScrollPos.y;
		AnyString text(pText, cursorToByte(pScrollPos));
		text.words("\n", [&](AnyString& line) -> bool
		{
			if (lineNb++ > lastVisibleLine)
				return false;
			// TODO : x offset ?
			if (!line.empty())
			{
				// Crop trailing `\r` (CR) if necessary
				if ('\r' == line[line.length() - 1])
					line.truncate(line.length() - 1);
				// Ignore empty lines (test a second time to catch single "\r" lines)
				if (!line.empty())
				{
					// y is the baseline position, not the top
					surface->drawText(line, pFont, pColor, x, y - pixelLineHeight / 2, pTabWidth, true);
				}
			}
			y += pixelLineHeight;
			return true;
		}, true);

		// Draw the cursor
		if (pCursorPos.y >= pScrollPos.y && pCursorPos.y <= lastVisibleLine)
		{
			float cx = columnToX(pCursorPos.x);
			float cy = lineToY(pCursorPos.y);
			surface->drawLine(pColor, cx, cy, cx, cy + pixelLineHeight, 1.0f);
		}

		// Draw line and column numbers
		surface->drawText(String(pCursorPos.y + 1) << ':' << pCursorPos.x, pFont, pColor, pSize.x - 50, pSize.y - 15);

		surface->endClipping();
		pModified = false;
	}


	void TextEditor::cursorMoveLeft(uint offset)
	{
		uint index = Math::Max(offset, cursorToByte(pCursorPos)) - offset;
		byteToCursor(pCursorPos, index);
		invalidate();
	}

	void TextEditor::cursorMoveRight(uint offset)
	{
		uint index = cursorToByte(pCursorPos) + offset;
		byteToCursor(pCursorPos, index);
		invalidate();
	}

	void TextEditor::cursorMoveUp(uint offset)
	{
		uint end = cursorToByte(pCursorPos);
		if (0 == end)
			return;

		AnyString part(pText, 0, end);

		for (uint i = end - 1; i > 0 && offset > 0; --i)
		{
			if ('\n' == part[i])
			{
				end = i;
				--offset;
			}
		}
		uint start = part.rfind('\n');
		if (start >= part.size())
			start = 0;
		else
			++start;
		byteToCursor(pCursorPos, Math::Min(start + pCursorPos.x, end));
		invalidate();
	}

	void TextEditor::cursorMoveDown(uint offset)
	{
		uint start = cursorToByte(pCursorPos);
		AnyString part(pText, start);

		uint lineStart = 0u;
		for (uint i = 0; i < part.size() && offset > 0; ++i)
		{
			if ('\n' == part[i])
			{
				lineStart = i + 1;
				--offset;
			}
		}
		uint absLineStart = start + lineStart;
		byteToCursor(pCursorPos, Math::Min(absLineStart + pCursorPos.x, currentLine(absLineStart).size()));
		invalidate();
	}


	void TextEditor::cursorBeginningOfLine()
	{
		pCursorPos.x = 0;
		invalidate();
	}


	void TextEditor::cursorEndOfLine()
	{
		uint index = cursorToByte(pCursorPos);
		AnyString part(pText, index);
		uint end = part.find('\n');
		AnyString cut(part, 0, end);
		pCursorPos.x += columnCount(cut);
		invalidate();
	}


	void TextEditor::cursorBeginningOfText()
	{
		pCursorPos.reset();
		invalidate();
	}


	void TextEditor::cursorEndOfText()
	{
		byteToCursor(pCursorPos, pText.size());
		invalidate();
	}


	EventPropagation TextEditor::keyDown(Input::Key key)
	{
		switch (key)
		{
			case Input::Left:
				cursorMoveLeft();
				break;
			case Input::Right:
				cursorMoveRight();
				break;
			case Input::Up:
				cursorMoveUp();
				break;
			case Input::Down:
				cursorMoveDown();
				break;
			case Input::Home:
				cursorBeginningOfLine();
				break;
			case Input::End:
				cursorEndOfLine();
				break;
			case Input::PageUp:
				scroll(displayedLineCount());
				break;
			case Input::PageDown:
				scroll(-displayedLineCount());
				break;
			case Input::Delete:
			{
				pText.erase(cursorToByte(pCursorPos), 1);
				invalidate();
				break;
			}
			default:
				break;
		}
		return epStop;
	}


	EventPropagation TextEditor::charInput(const AnyString& str)
	{
		switch (str[0])
		{
			// Backspace
			case 0x08:
				for (uint i = 0; i < str.size(); ++i)
				{
					// Cannot use backspace when at beginning of file
					if (0 == pCursorPos.y && 1 == pCursorPos.x)
						return epStop;
					// When at beginning of line but not on first line, move up
					if (0 == pCursorPos.y && pCursorPos.x > 1)
						cursorPos(pCursorPos.x - 1, columnCount(pCursorPos.x - 1));
					else
						cursorPos(pCursorPos.x, pCursorPos.y - 1);
					// Erase
					pText.erase(cursorToByte(pCursorPos), 1);
				}
				invalidate();
				break;
			// Tab
			case '\t':
				pText.insert(cursorToByte(pCursorPos), str);
				cursorPos(pCursorPos.x, pCursorPos.y + str.size() * pTabWidth);
				break;
			// Carriage Return
			case '\r':
			// New Line / Line Feed
			case '\n':
				for (uint i = 0; i < str.size(); ++i)
					pText.insert(cursorToByte(pCursorPos), '\n');
				++pCursorPos.y;
				invalidate();
				break;
			// Normal displayable characters
			default:
				// Non-displayable characters are ignored
				std::locale loc;
				if (!std::isgraph(str[0], loc))
					break;
				pText.insert(cursorToByte(pCursorPos), str);

				// Advance the cursor
				pCursorPos.x += str.utf8size();
				invalidate();
				break;
		}
		return epStop;
	}


	EventPropagation TextEditor::mouseDown(Input::IMouse::Button btn, float x, float y)
	{
		if (btn == Input::IMouse::ButtonLeft)
		{
			cursorPos(XToColumn(x), YToLine(y));
			pDragPos(pCursorPos);
			pDragging = true;
			invalidate();
		}
		return epStop;
	}


	EventPropagation TextEditor::mouseUp(Input::IMouse::Button btn, float x, float y)
	{
		if (btn == Input::IMouse::ButtonLeft)
		{
			pDragPos(XToColumn(x), YToLine(y));
			pDragging = false;
			invalidate();
		}
		return epStop;
	}


	EventPropagation TextEditor::mouseMove(float x, float y)
	{
		if (pDragging)
		{
			pDragPos(XToColumn(x), YToLine(y));
			invalidate();
		}
		return epContinue;
	}


	EventPropagation TextEditor::mouseScroll(float delta, float, float)
	{
		scroll(delta);
		return epStop;
	}


	void TextEditor::scroll(float nbLines)
	{
		uint oldTopLine = pScrollPos.y;
		float newLineNb = ((float)pScrollPos.y > nbLines) ? (float)pScrollPos.y - nbLines : 0.0f;
		float maxLineNb = (float)lineCount();
		pScrollPos.y = (uint)Math::Min(maxLineNb - displayedLineCount(), newLineNb);
		if (oldTopLine != pScrollPos.y)
			invalidate();
	}


} // namespace Control
} // namespace UI
} // namespace Yuni
