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
#ifndef __YUNI_UI_CONTROL_SPLITTER_H__
# define __YUNI_UI_CONTROL_SPLITTER_H__

# include "../../yuni.h"
# include "control.h"
# include "panel.h"

namespace Yuni
{
namespace UI
{
namespace Control
{


	//! A splitter panel is a vertically- or horizontally-splitting container, each side containing a panel
	class Splitter: public IControl
	{
	public:
		//! Smart pointer
		typedef Ancestor::SmartPtrType<Splitter>::Ptr  Ptr;

	public:
		//! Orientation of the split
		enum SplitOrientation
		{
			soVertical,
			soHorizontal
		};

	public:
		//! Constructor
		Splitter(float x, float y, float width, float height):
			IControl(x, y, width, height),
			pOrient(soVertical),
			pSplitOffset(width / 2.0f)
		{
			addChild(new Panel(x, y, pSplitOffset, height));
			addChild(new Panel(x + pSplitOffset, y, pSplitOffset, height));
		}

		//! Constructor
		Splitter(const Point2D<float>& position, const Point2D<float>& size):
			Splitter(position.x, position.y, size.x, size.y)
		{}

		//! Virtual destructor
		virtual ~Splitter() {}

		//! Draw the splitter
		virtual void draw(DrawingSurface::Ptr& surface, float xOffset, float yOffset) const override;

		//! Get the current orientation
		SplitOrientation orientation() const { return pOrient; }
		//! Set the orientation
		void orientation(SplitOrientation newOrientation) { pOrient = newOrientation; }

		//! Get the offset at which the split appears (in pixels)
		float splitOffset() const { return pSplitOffset; }
		//! Set the offset at which the split appears (in pixels)
		void splitOffset(float newOffset) { pSplitOffset = newOffset; resizePanels(); }

		//! Left panel when vertical, top panel when horizontal
		UI::IControl::Ptr& firstPanel() { assert(pChildren.size() > 0); return pChildren[0]; }
		const UI::IControl::Ptr& firstPanel() const { assert(pChildren.size() > 0); return pChildren[0]; }

		//! Right panel when vertical, bottom panel when horizontal
		UI::IControl::Ptr& secondPanel() { assert(pChildren.size() > 1); return pChildren[1]; }
		const UI::IControl::Ptr secondPanel() const { assert(pChildren.size() > 1); return pChildren[1]; }

	private:
		//! Automatically resize the two contained panels depending on the split characteristics
		void resizePanels();

	private:
		//! Orientation of the split
		SplitOrientation pOrient;

		//! Position of the split
		float pSplitOffset;

	}; // class Splitter



} // namespace Control
} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_CONTROL_SPLITTER_H__
