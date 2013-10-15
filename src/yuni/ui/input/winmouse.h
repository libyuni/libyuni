#ifndef __YUNI_UI_INPUT_WINMOUSE_H__
# define __YUNI_UI_INPUT_WINMOUSE_H__

# include "../../core/system/windows.hdr.h"
# include "mouse.h"

namespace Yuni
{
namespace Input
{


	//! Mouse input management under Windows
	class WinMouse : public IMouse
	{
	public:
		//! Constructor
		WinMouse()
		{
			POINT p;
			::GetCursorPos(&p);
			pPos(p.x, p.y);
			pLastPos(pPos);
			pDragPos(pPos);
		}

		//! Virtual destructor
		virtual ~WinMouse() {}

		virtual void showCursor() { ::ShowCursor(true); }

		virtual void hideCursor() { ::ShowCursor(false); }

		virtual void showCursor(bool visible) { ::ShowCursor(visible); }

	}; // class WinMouse



} // namespace Input
} // namespace Yuni


#endif // __YUNI_UI_INPUT_WINMOUSE_H__
