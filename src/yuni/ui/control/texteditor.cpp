#include "texteditor.h"

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
					surface->drawText(line, pFont, pColor, x, y, pTabWidth);
				}
			}
			y += pixelLineHeight;
		}

		// Draw the cursor
		if (pCursorPos.x >= pTopLineNb && pCursorPos.x <= lastVisibleLine)
		{
			float cx = ColumnToX(pCursorPos.y);
			float cy = LineToY(pCursorPos.x);
			surface->drawLine(pColor, cx, cy, cx, cy + pLineHeight(pConversion), 1.0f);
		}

		surface->endClipping();
		pModified = false;
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
		if (pLines.empty() && !pText.empty())
			reloadLines();

		uint oldTopLine = pTopLineNb;
		float newLineNb = (float)pTopLineNb - delta;
		float maxLineNb = (float)pLines.size();
		float displayedLineCount = (pSize.y - pVertMargin) / pLineHeight(pConversion);
		pTopLineNb = (uint)Math::Max(1.0f, Math::Min(maxLineNb - displayedLineCount + 1, newLineNb));
		if (oldTopLine != pTopLineNb)
			invalidate();
		return epStop;
	}



} // namespace Control
} // namespace UI
} // namespace Yuni
