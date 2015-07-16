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
#ifndef __YUNI_UI_CONTROL_LISTBOX_H__
# define __YUNI_UI_CONTROL_LISTBOX_H__

# include "../../yuni.h"
# include "../../core/color/rgb.h"
# include "../../core/color/rgba.h"
# include "control.h"
# include "../displaymode.h"
# include "../dimension.h"
# include "../font.h"
# include "../theme.h"

namespace Yuni
{
namespace UI
{
namespace Control
{


	//! A list box is a multi-line area with line-by-line selection
	template<class ContentT>
	class ListBox: public IControl
	{
	public:
		//! Smart pointer
		typedef typename Ancestor::SmartPtrType<ListBox<ContentT> >::Ptr  Ptr;

	public:
		//! An element in the list
		class ListBoxElement
		{
		public:
			ListBoxElement(const AnyString& newLabel, ContentT newContent):
				label(newLabel),
				content(newContent)
			{}

		public:
			String label;
			ContentT content;

		}; // class ListBoxElement


		typedef std::vector<ListBoxElement>  ElementVector;

	public:
		//! Selection change callback
		Yuni::Bind<EventPropagation (IControl* sender, uint newIndex)>  onSelectionChange;


	public:
		ListBox(float x, float y, float maxWidth, float maxHeight):
			IControl(x, y, maxWidth, maxHeight),
			pIndex(0u),
			pClickedIndex(-1),
			pFont(Theme::Current()->monoFont),
			// White by default
			pColor(Theme::Current()->textColor),
			pBackColor(Theme::Current()->windowColor),
			pAntiAliased(true),
			pTopLineNb(0u),
			pLineHeight(161.8_pcp),
			pHorizMargin(15),
			pVertMargin(10)
		{
			// We use percent parent on the font size (height) for line height calculation
			pConversion.unitPerPercentParent = pFont->size() * 96.0f / 72.0f / 100.0f;
		}

		ListBox(const Point2D<float>& position, const Point2D<float>& maxSize):
			IControl(position, maxSize),
			pIndex(0u),
			pClickedIndex(-1),
			pFont(Theme::Current()->monoFont),
			pColor(Theme::Current()->textColor),
			pBackColor(Theme::Current()->windowColor),
			pAntiAliased(true),
			pTopLineNb(0u),
			pLineHeight(161.8_pcp),
			pHorizMargin(20),
			pVertMargin(10)
		{
			// We use percent parent on the font size (height) for line height calculation
			pConversion.unitPerPercentParent = pFont->size() * 96.0f / 72.0f / 100.0f;
		}

		//! Virtual destructor
		virtual ~ListBox() { }

		//! Draw the panel
		virtual void draw(DrawingSurface::Ptr& surface, float xOffset, float yOffset) const override;

		//! Clear the list
		void clear() { invalidate(); pIndex = 0u; pTopLineNb = 0u; pElements.clear(); }

		//! Get the label for the currently selected element
		const String& selectedLabel() const { return pElements[pIndex].label; }

		//! Get the currently selected element
		ContentT& selected()
		{
			assert(pIndex >= 0u && pIndex < pElements.size());
			return pElements[pIndex].content;
		}
		const ContentT& selected() const
		{
			assert(pIndex >= 0u && pIndex < pElements.size());
			return pElements[pIndex].content;
		}

		//! Select an index in the list
		bool select(uint newIndex, bool triggerEvent = false)
		{
			if (pIndex == newIndex)
				return true;
			if (newIndex >= pElements.size())
				return false;
			pIndex = newIndex;
			if (triggerEvent)
				onSelectionChange(this, pIndex);
			invalidate();
			return true;
		}

		ContentT& operator [] (uint index)
		{
			assert(index >= 0u && index < pElements.size());
			return pElements[index];
		}

		const ContentT& operator [] (uint index) const
		{
			assert(index >= 0u && index < pElements.size());
			return pElements[index];
		}

		template<class T>
		void appendElement(const AnyString& label, T& value)
		{
			pElements.push_back(ListBoxElement(label, (ContentT)value));
		}

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

		//! Is the text anti-aliased ?
		bool antiAliased() const { return pAntiAliased; }
		//! Set text anti-aliasing
		void antiAliased(bool newValue) { pAntiAliased = newValue; invalidate(); }

		//! Get line height
		const Dimension& lineHeight() const { return pLineHeight; }
		//! Get line height in pixels
		float lineHeightPixels() const { return pLineHeight(pConversion); }
		//! Set line height
		void lineHeight(const Dimension& newValue) { pLineHeight = newValue; }
		//! Set line height in pixels
		void lineHeightPixels(float newValue) { pLineHeight.reset(newValue, Dimension::uPixel); }

		//! Mouse button down override
		virtual EventPropagation mouseDown(Input::IMouse::Button btn, float x, float y) override;
		//! Mouse button down override
		virtual EventPropagation mouseUp(Input::IMouse::Button btn, float x, float y) override;
		//! Mouse wheel scroll override
		virtual EventPropagation mouseScroll(float delta, float x, float y) override;

	private:
		//! Elements to display
		ElementVector pElements;

		//! Currently selected index
		uint pIndex;

		//! When the user starts clicking in the list, store the index
		int pClickedIndex;


		//! Font to use
		FTFont::Ptr pFont;

		//! Text color
		Color::RGBA<float> pColor;

		//! Background color
		Color::RGBA<float> pBackColor;

		//! Anti-alias the text ?
		bool pAntiAliased;

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

	}; // class ListBox



} // namespace Control
} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_CONTROL_LISTBOX_H__

#include "listbox.hxx"
