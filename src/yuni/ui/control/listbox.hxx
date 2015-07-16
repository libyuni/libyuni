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
#pragma once

namespace Yuni
{
namespace UI
{
namespace Control
{


	template<class ContentT>
	void ListBox<ContentT>::draw(DrawingSurface::Ptr& surface, float xOffset, float yOffset) const
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
		// Loop on elements
		for (const auto& element : pElements)
		{
			if (lineNb++ < pTopLineNb)
				continue;
			// Ignore empty labels
			if (!element.label.empty())
			{
				if (pIndex == lineNb - 1)
					// Currently selected -> invert colors
					surface->drawTextOnColor(element.label, pFont, pBackColor, pColor, x, y);
				else
					surface->drawText(element.label, pFont, pColor, x, y);
			}
			y += pixelLineHeight;
			// Stop if we are outside the rectangle (for optim)
			if (y >= pSize.y)
				break;
		}

		surface->endClipping();
		pModified = false;
	}


	template<class Content>
	EventPropagation ListBox<Content>::mouseScroll(float delta, float, float)
	{
		uint oldTopLine = pTopLineNb;
		float newLineNb = (float)pTopLineNb - delta;
		float maxLineNb = (float)pElements.size();
		float displayedLineCount = pSize.y / pLineHeight(pConversion);
		pTopLineNb = (uint)Math::Max(0.0f, Math::Min(maxLineNb - displayedLineCount, newLineNb));
		if (oldTopLine != pTopLineNb)
			invalidate();
		return epStop;
	}


	template<class Content>
	EventPropagation ListBox<Content>::mouseDown(Input::IMouse::Button btn, float, float y)
	{
		if (btn == Input::IMouse::ButtonLeft)
		{
			// Store selected index, wait mouseUp to confirm selection
			pClickedIndex = pTopLineNb + Math::Floor((y - pPosition.y) / pLineHeight(pConversion));
			if (pClickedIndex < 0 || (uint)pClickedIndex >= pElements.size())
				pClickedIndex = -1;
		}
		return epStop;
	}


	template<class Content>
	EventPropagation ListBox<Content>::mouseUp(Input::IMouse::Button btn, float, float y)
	{
		if (btn == Input::IMouse::ButtonLeft && pClickedIndex > -1)
		{
			// Store selected index, wait mouseUp to confirm selection
			uint newIndex = pTopLineNb + Math::Floor((y - pPosition.y) / pLineHeight(pConversion));
			if (newIndex == (uint)pClickedIndex)
			{
				pIndex = newIndex;
				onSelectionChange(this, pIndex);
				invalidate();
			}
			pClickedIndex = -1;
		}
		return epStop;
	}



} // namespace Control
} // namespace UI
} // namespace Yuni
