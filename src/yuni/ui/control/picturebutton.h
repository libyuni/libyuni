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
		PictureButton(int x, int y, uint width, uint height):
			IControl(x, y, width, height)
		{}

		PictureButton(const Point2D<>& position, const Point2D<uint>& size):
			IControl(position, size)
		{}

		//! Virtual destructor
		virtual ~PictureButton() {}

		void image(const Gfx3D::Texture::Ptr& image) { pImage = image; }

		void imageClicking(const Gfx3D::Texture::Ptr& image) { pImageClicking = image; }

		void imageHovering(const Gfx3D::Texture::Ptr& image) { pImageHovering = image; }

		//! Launch a click event
		virtual void mouseDown(Input::IMouse::Button btn, int, int) override
		{
			if (btn != Input::IMouse::ButtonLeft)
				return;
			pBeingClicked = true;
			if (!(!pImageClicking))
				pModified = true;
		}

		virtual void mouseUp(Input::IMouse::Button btn, int, int) override
		{
			if (btn != Input::IMouse::ButtonLeft)
				return;
			if (pBeingClicked)
				pOnClick();
			pBeingClicked = false;
		}

		//! Bind the onClick event
		template<class FuncT>
		void onClick(const FuncT& function) { pOnClick.bind(function); }

		//! Bind the onClick event
		template<class ClassT, class MethodT>
		void onClick(const ClassT& object, const MethodT& method) { pOnClick.bind(object, method); }

		//! Draw the button on the surface
		virtual void draw(DrawingSurface::Ptr& surface, bool root);

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
