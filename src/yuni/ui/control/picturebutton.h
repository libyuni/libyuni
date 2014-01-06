#ifndef __YUNI_UI_CONTROL_PICTUREBUTTON_H__
# define __YUNI_UI_CONTROL_PICTUREBUTTON_H__

# include "../../yuni.h"
# include "control.h"
# include "../font.h"

namespace Yuni
{
namespace UI
{
namespace Control
{


	//! A picture button is an image acting as a clickable control that triggers an event
	class PictureButton: public IControl
	{
	public:
		//! Type of bind for the picturebutton callback
		typedef Yuni::Bind<void (void)>  Callback;

	public:
		PictureButton()
		{}

		PictureButton(int x, int y, uint width, uint height):
			IControl(x, y, width, height),
			pBeingClicked(false)
		{}

		PictureButton(const Point2D<>& position, const Point2D<uint>& size):
			IControl(position, size),
			pBeingClicked(false)
		{}

		//! Virtual destructor
		virtual ~PictureButton() {}

		//! Image shown on the button
		void image(const Gfx3D::Texture::Ptr& image) { pImage = image; }

		//! Image displayed while clicking (same as standard image if `null`)
		void imageClicking(const Gfx3D::Texture::Ptr& image) { pImageClicking = image; }

		//! Image displayed while hovering (same as standard image if `null`)
		void imageHovering(const Gfx3D::Texture::Ptr& image) { pImageHovering = image; }

		//! On mouse button down
		virtual void mouseDown(Input::IMouse::Button btn, int, int) override
		{
			if (btn != Input::IMouse::ButtonLeft)
				return;
			pBeingClicked = true;
			if (!(!pImageClicking))
				pModified = true;
		}

		//! On mouse button up
		virtual void mouseUp(Input::IMouse::Button btn, int, int) override
		{
			if (btn != Input::IMouse::ButtonLeft)
				return;
			if (pBeingClicked)
				pOnClick();
			pBeingClicked = false;
			pModified = true;
		}

		//! Bind the onClick event
		template<class FuncT>
		void onClick(const FuncT& function) { pOnClick.bind(function); }

		//! Bind the onClick event
		template<class ClassT, class MethodT>
		void onClick(const ClassT& object, const MethodT& method) { pOnClick.bind(object, method); }

		//! Draw the button on the surface
		virtual void draw(DrawingSurface::Ptr& surface, bool root) override;

	private:
		Callback pOnClick;

		Gfx3D::Texture::Ptr pImage;

		Gfx3D::Texture::Ptr pImageClicking;

		Gfx3D::Texture::Ptr pImageHovering;

		bool pBeingClicked;

	}; // class Button



} // namespace Control
} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_CONTROL_BUTTON_H__
