#include "texteditor.h"

namespace Yuni
{
namespace UI
{
namespace Control
{


	void TextEditor::draw(DrawingSurface::Ptr& surface, bool root) const
	{
		if (!pVisible)
			return;

		Point2D<float> pos(pPosition);
		// If the text is the root control, use absolute coordinates
		if (root)
			pos(0.0f, 0.0f);
		surface->beginRectangleClipping(pos.x, pos.y, pSize.x, pSize.y);
		float yOffset = 0.0f;
		pText.words("\n", [&] (const AnyString& line)
		{
			// Any line not starting with '{' is ignored (comment)
			if (!line.empty())
			{
				// if ('\r' == line[line.length()])
				// 	line.truncate(line.length() - 1);
				surface->drawText(line, pFont, pColor, pos.x, pos.y + yOffset);
			}
			yOffset += pLineSpacing;
			return true;
		});
		surface->endClipping();
		pModified = false;
	}



} // namespace Control
} // namespace UI
} // namespace Yuni
