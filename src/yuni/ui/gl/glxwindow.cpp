
// TODO: We should really do without this (using a better error management)
#include <iostream>
#include "../windowfactory.h"
#include "glxwindow.h"


namespace Yuni
{
namespace UI
{


	bool GLXWindow::initialize()
	{
		XVisualInfo* vinfo;
		int attrList[20];
		int indx = 0;

		if (!(pDisplay = XOpenDisplay (NULL)))
			return false;

		attrList[indx++] = GLX_USE_GL;
		attrList[indx++] = GLX_DEPTH_SIZE;
		attrList[indx++] = 1;
		attrList[indx++] = GLX_RGBA;
		attrList[indx++] = GLX_RED_SIZE;
		attrList[indx++] = 1;
		attrList[indx++] = GLX_GREEN_SIZE;
		attrList[indx++] = 1;
		attrList[indx++] = GLX_BLUE_SIZE;
		attrList[indx++] = 1;
		attrList[indx] = 0; // None

		vinfo = glXChooseVisual(pDisplay, DefaultScreen(pDisplay), attrList);
		if (vinfo == NULL)
		{
			std::cerr << "ERROR: Can't open window" << std::endl;
			return false;
		}
		Window root = DefaultRootWindow(pDisplay);

		pAttr.colormap = XCreateColormap(pDisplay, root, vinfo->visual, AllocNone);
		pAttr.background_pixel = BlackPixel(pDisplay, vinfo->screen);
		pAttr.border_pixel = BlackPixel(pDisplay, vinfo->screen);
		pWindow = XCreateWindow(pDisplay, root,
							30, 30, pWidth, pHeight, 0, vinfo->depth, CopyFromParent,
							vinfo->visual, CWBackPixel | CWBorderPixel | CWColormap, &pAttr);

		XMapWindow(pDisplay, pWindow);
		XStoreName(pDisplay, pWindow, "VERY SIMPLE APPLICATION");

		pContext = glXCreateContext(pDisplay, vinfo, NULL, True);
		if (pContext == NULL)
		{
			std::cerr << "glXCreateContext failed" << std::endl;
			return false;
		}
		if (!glXMakeCurrent(pDisplay, pWindow, pContext))
		{
			std::cout << "glXMakeCurrent failed" << std::endl;
			return false;
		}
		resize(pWidth, pHeight);
		if (!GLWindow::initialize())
		{
			std::cout << "GL initialization failed" << std::endl;
			return false;
		}
		return true;
	}

	void GLXWindow::kill()
	{
		if (pContext)
		{
			if (!glXMakeCurrent(pDisplay, 0, NULL))
			{
				printf("Could not release drawing context.\n");
			}
			glXDestroyContext(pDisplay, pContext);
			pContext = NULL;
		}
		// Switch back to original desktop resolution if we were in fs
		if (pFullScreen)
		{
// 			XF86VidModeSwitchToMode(pDisplay, pScreen, &GLWin.deskMode);
// 			XF86VidModeSetViewPort(pDisplay, pScreen, 0, 0);
		}
		XCloseDisplay(pDisplay);
		pDisplay = NULL;

		GLWindow::kill();
	}


	void GLXWindow::setIcon(const AnyString& path)
	{
		// TODO
	}


	bool GLXWindow::vsync() const
	{
		typedef int (*SwapGetIntervalProto)();
		SwapGetIntervalProto getSwapIntervalEXT = 0;
		const String extensions((const char*)glGetString(GL_EXTENSIONS));

		if (extensions.find("GLX_MESA_swap_control") != String::npos)
		{
			getSwapIntervalEXT = (SwapGetIntervalProto)glXGetProcAddress((const GLubyte*)"glXGetSwapIntervalMESA");
			if (getSwapIntervalEXT)
				return getSwapIntervalEXT();
		}

		// From what I read, default is false when no extension is present.
		return false;
	}


	void GLXWindow::vsync(bool active)
	{
		typedef int (*SwapIntervalProto)(int);
		SwapIntervalProto swapIntervalEXT = 0;
		const String extensions((const char*)glGetString(GL_EXTENSIONS));

		if (extensions.find("GLX_SGI_swap_control") != String::npos)
			swapIntervalEXT = (SwapIntervalProto)glXGetProcAddress((const GLubyte*)"glXSwapIntervalSGI");
		else
		{
			if (extensions.find("GLX_MESA_swap_control") != String::npos)
				swapIntervalEXT = (SwapIntervalProto)glXGetProcAddress((const GLubyte*)"glXSwapIntervalMESA");
		}

		if (swapIntervalEXT)
			swapIntervalEXT(active ? 1 : 0);
	}



	bool GLXWindow::loop()
	{
		XEvent event;
		while (true)
		{
			XNextEvent(pDisplay, &event);
			switch (event.type)
			{
				case ConfigureNotify:
					break;
				case Expose:
					break;
				case ButtonPress:
					break;
				/*case EnterWindow:
					break;
				case LeaveWindow:
					break;
				*/
				case MapNotify:
					break;
			}
		}
		return true;
	}


	RenderWindow* WindowFactory::CreateGLWindow(const AnyString& title, unsigned int width,
		unsigned int height, unsigned int bits, bool fullScreen)
	{
		RenderWindow* wnd = new GLXWindow(title, width, height, bits, fullScreen);
		if (!wnd->initialize())
		{
			wnd->kill();
			delete wnd;
			return NULL;
		}
		return wnd;
	}



} // namespace UI
} // namespace Yuni
