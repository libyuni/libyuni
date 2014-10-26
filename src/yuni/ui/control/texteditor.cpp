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
		if (pLines.empty() && !pText.empty())
			reloadLines();

		Point2D<float> pos(pPosition.x + xOffset, pPosition.y + yOffset);

		// Draw background
		surface->drawFilledRectangle(pBackColor, pBackColor, pos.x, pos.y, pSize.x, pSize.y, 0.0f);

		surface->beginRectangleClipping(pos.x, pos.y, pSize.x, pSize.y);

		// Draw the text
		float pixelLineHeight = pLineHeight(pConversion);
		float x = pos.x + pHorizMargin;
		float y = pos.y + pVertMargin;
		uint lastVisibleLine = YToLine(pSize.y - pVertMargin);
		// Loop on lines of text
		AnyString line;
		for (uint lineNb = pTopLineNb; lineNb <= lastVisibleLine; ++lineNb)
		{
			line.assign(pLines[lineNb - 1]);
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
		}

		// Draw the cursor
		if (pCursorPos.x >= pTopLineNb && pCursorPos.x <= lastVisibleLine)
		{
			float cx = columnToX(pCursorPos.y);
			float cy = lineToY(pCursorPos.x);
			surface->drawLine(pColor, cx, cy, cx, cy + pLineHeight(pConversion), 1.0f);
		}

		// Draw line and column numbers
		surface->drawText(String(pCursorPos.x) << ':' << pCursorPos.y, pFont, pColor, pSize.x - 50, pSize.y - 15);

		surface->endClipping();
		pModified = false;
	}


	EventPropagation TextEditor::keyDown(Input::Key key)
	{
		switch (key)
		{
			case Input::Left:
				if (0 == pCursorPos.y && pCursorPos.x > 1)
					// When at beginning of line, move up
					cursorPos(pCursorPos.x - 1, lineSize(pCursorPos.x - 1));
				else
				{
					uint index = cursorIndex();
					if (index > 0 && pText[index - 1] == '\t')
						cursorPos(pCursorPos.x, pCursorPos.y - pTabWidth);
					else
						cursorPos(pCursorPos.x, pCursorPos.y - 1);
				}
				break;
			case Input::Right:
				if (lineSize(pCursorPos.x) == pCursorPos.y)
					// When at end of line, move down and go to beginning of line
					cursorPos(pCursorPos.x + 1, 0);
				else
					cursorPos(pCursorPos.x, pCursorPos.y + 1);
				break;
			case Input::Up:
				cursorPos(pCursorPos.x - 1, pCursorPos.y);
				break;
			case Input::Down:
				cursorPos(pCursorPos.x + 1, pCursorPos.y);
				break;
			case Input::Home:
				cursorPos(pCursorPos.x, 0);
				break;
			case Input::End:
				cursorPos(pCursorPos.x, lineSize(pCursorPos.x));
				break;
			case Input::PageUp:
				scroll(displayedLineCount());
				break;
			case Input::PageDown:
				scroll(-displayedLineCount());
				break;
			case Input::Delete:
			{
				bool endOfLine = (lineSize(pCursorPos.x) == pCursorPos.y);
				pText.erase(cursorIndex(), 1);
				if (endOfLine)
					reloadLines();
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
						cursorPos(pCursorPos.x - 1, lineSize(pCursorPos.x - 1));
					else
						cursorPos(pCursorPos.x, pCursorPos.y - 1);
					// Erase
					pText.erase(cursorIndex(), 1);
				}
				// TODO : Manage line addition / removal by fixing pLines
				// For now, reload all lines :
				reloadLines();
				invalidate();
				break;
			// Tab
			case '\t':
				pText.insert(cursorIndex(), str);
				cursorPos(pCursorPos.x, pCursorPos.y + str.size() * pTabWidth);
				break;
			// Carriage Return
			case '\r':
			// New Line / Line Feed
			case '\n':
				for (uint i = 0; i < str.size(); ++i)
					pText.insert(cursorIndex(), '\n');
				// TODO : Manage line addition / removal by fixing pLines
				// For now, reload all lines :
				reloadLines();
				cursorPos(pCursorPos.x + 1, 0);
				break;
			// Normal displayable characters
			default:
				// Non-displayable characters are ignored
				std::locale loc;
				if (!std::isgraph(str[0], loc))
					break;
				pText.insert(cursorIndex(), str);

				// Advance the cursor
				cursorPos(pCursorPos.x, pCursorPos.y + str.size());
				break;
		}
		return epStop;
	}


	EventPropagation TextEditor::mouseDown(Input::IMouse::Button btn, float x, float y)
	{
		if (btn == Input::IMouse::ButtonLeft)
		{
			cursorPos(YToLine(y), XToColumn(x));
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
			pDragPos(YToLine(y), XToColumn(x));
			pDragging = false;
			invalidate();
		}
		return epStop;
	}


	EventPropagation TextEditor::mouseMove(float x, float y)
	{
		if (pDragging)
		{
			pDragPos(YToLine(y), XToColumn(x));
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
		if (pLines.empty() && !pText.empty())
			reloadLines();

		uint oldTopLine = pTopLineNb;
		float newLineNb = (float)pTopLineNb - nbLines;
		float maxLineNb = (float)pLines.size();
		pTopLineNb = (uint)Math::Max(1.0f, Math::Min(maxLineNb - displayedLineCount() + 1, newLineNb));
		if (oldTopLine != pTopLineNb)
			invalidate();
	}


} // namespace Control
} // namespace UI
} // namespace Yuni
