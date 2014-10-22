#ifndef __YUNI_UI_CONTROL_PANEL_H__
# define __YUNI_UI_CONTROL_PANEL_H__

# include "../../yuni.h"
# include "../../core/color/rgba.h"
# include "control.h"

namespace Yuni
{
namespace UI
{
namespace Control
{


	//! A panel is an unmovable, by default transparent control meant to contain other controls.
	class Panel: public IControl
	{
	public:
		//! Smart pointer
		typedef Ancestor::SmartPtrType<Panel>::Ptr  Ptr;

	public:
		//! Constructor
		Panel(float x, float y, float width, float height);

		//! Constructor
		Panel(const Point2D<float>& position, const Point2D<float>& size);

		//! Virtual destructor
		virtual ~Panel() {}

		//! Draw the panel
		virtual void draw(DrawingSurface::Ptr& surface, float xOffset, float yOffset) const override;

		void color(const Color::RGBA<float>& newColor) { pColor = newColor; }
		const Color::RGBA<float>& color() const { return pColor; }

	protected:
		Color::RGBA<float> pColor;

	}; // class Panel



} // namespace Control
} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_CONTROL_PANEL_H__
