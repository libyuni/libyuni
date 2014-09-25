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

		Point2D<float> pos(pPosition.x + xOffset, pPosition.y + yOffset);

		// Draw background
		surface->drawFilledRectangle(pBackColor, pBackColor, pos.x, pos.y, pSize.x, pSize.y, 0.0f);

		surface->beginRectangleClipping(pos.x + pHorizMargin, pos.y + pVertMargin,
			pSize.x - 2 * pHorizMargin, pSize.y - 2 * pVertMargin);

		// Draw the text
		float pixelLineHeight = pLineHeight(pConversion);
		float x = pos.x + pHorizMargin;
		float y = pos.y + pVertMargin;
		uint lineNb = 0u;
		// Loop on lines of text
		pText.words("\n", [&] (AnyString& line)
		{
			if (lineNb++ < pTopLineNb)
				return true;
			if (!line.empty())
			{
				// Crop the `\r` (CR) if necessary
				if ('\r' == line[line.length() - 1])
					line.truncate(line.length() - 1);
				// Ignore empty lines (test a second time to catch single "\r" lines)
				if (!line.empty())
				{
					surface->drawTextOnColor(line, pFont, pColor, pBackColor, x, y);
				}
			}
			y += pixelLineHeight;
			// Stop if we are outside the rectangle (for optim)
			return y < pSize.y;
		}, true);

		surface->endClipping();
		pModified = false;
	}


	EventPropagation TextEditor::mouseScroll(float delta, float, float)
	{
		uint oldTopLine = pTopLineNb;
		float newLineNb = (float)pTopLineNb - delta;
		float maxLineNb = (float)(pText.countChar('\n') + 1);
		float displayedLineCount = pSize.y / pLineHeight(pConversion);
		pTopLineNb = (uint)Math::Max(0.0f, Math::Min(maxLineNb - displayedLineCount + 1, newLineNb));
		if (oldTopLine != pTopLineNb)
			invalidate();
		return epStop;
	}



} // namespace Control
} // namespace UI
} // namespace Yuni
