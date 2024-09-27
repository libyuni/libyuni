/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** github: https://github.com/libyuni/libyuni/
** gitlab: https://gitlab.com/libyuni/libyuni/ (mirror)
*/
#pragma once

#ifdef YUNI_WINDOWSYSTEM_X11

# include "../../core/system/x11.hdr.h"

namespace Yuni
{
namespace Input
{


	//! Mouse input management under Windows
	class X11Mouse : public IMouse
	{
	public:
		//! Constructor
		X11Mouse(Display* display, Window window):
			pDisplay(display),
			pWindow(window),
			pVisible(true)
		{
			Window windowReturned;
			int rootX;
			int rootY;
			int winX;
			int winY;
			unsigned int maskReturn;

			::XQueryPointer(display, window, &windowReturned, &windowReturned,
				&rootX, &rootY, &winX, &winY, &maskReturn);
			pPos(rootX, rootY);
			pLastPos(pPos);
			pDragPos(pPos);
		}

		//! Virtual destructor
		virtual ~X11Mouse() {}

		virtual bool visible() const override
		{
			return pVisible;
		}

		virtual void showCursor() override
		{
			if (not pVisible)
			{
				pVisible = true;
				// Revert previous sabotage
				::XUndefineCursor(pDisplay, pWindow);
			}
		}

		virtual void hideCursor() override
		{
			pVisible = false;

			// Sabotage the cursor by setting an invisible one
			Cursor invisibleCursor;
			Pixmap bitmapNoData;
			XColor black;
			static const constexpr char noData[] = { 0,0,0,0,0,0,0,0 };
			black.red = black.green = black.blue = 0;

			bitmapNoData = ::XCreateBitmapFromData(pDisplay, pWindow, noData, 8, 8);
			invisibleCursor = ::XCreatePixmapCursor(pDisplay, bitmapNoData, bitmapNoData,
				&black, &black, 0, 0);
			::XDefineCursor(pDisplay, pWindow, invisibleCursor);
			::XFreeCursor(pDisplay, invisibleCursor);
			::XFreePixmap(pDisplay, bitmapNoData);
		}

		virtual void showCursor(bool visible) override
		{
			if (visible)
				showCursor();
			else
				hideCursor();
		}

	private:
		Display* pDisplay;
		Window pWindow;
		bool pVisible;

	}; // class X11Mouse


} // namespace Input
} // namespace Yuni

#endif // YUNI_WINDOWSYSTEM_X11
