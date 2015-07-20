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


	RenderWindow* WindowFactory::CreateGLWindow(const AnyString& title, uint width,
		uint height, uint bits, bool fullScreen)
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
