#ifndef __YUNI_UI_GLXWINDOW_HXX__
# define __YUNI_UI_GLXWINDOW_HXX__

namespace Yuni
{
namespace UI
{


	inline void GLXWindow::swapBuffers()
	{
		//XNextEvent(pDisplay, &xev);
		::XGetWindowAttributes(pDisplay, pWindow, &pWndAttr);
		::glXSwapBuffers(pDisplay, pWindow);
	}

	inline void GLXWindow::refreshAndSwap()
	{
		pRefreshFunc();
		::XGetWindowAttributes(pDisplay, pWindow, &pWndAttr);
		::glXSwapBuffers(pDisplay, pWindow);
	}



} // UI
} // Yuni

#endif // __YUNI_UI_GLXWINDOW_HXX__
