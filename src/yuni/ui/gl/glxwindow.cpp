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

#include "../../yuni.h"
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

		if (!(pDisplay = ::XOpenDisplay(NULL)))
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

		vinfo = ::glXChooseVisual(pDisplay, DefaultScreen(pDisplay), attrList);
		if (vinfo == NULL)
		{
			std::cerr << "ERROR: Can't open window" << std::endl;
			return false;
		}
		Window root = DefaultRootWindow(pDisplay);

		pAttr.colormap = ::XCreateColormap(pDisplay, root, vinfo->visual, AllocNone);
		pAttr.background_pixel = BlackPixel(pDisplay, vinfo->screen);
		pAttr.border_pixel = BlackPixel(pDisplay, vinfo->screen);
		pWindow = ::XCreateWindow(pDisplay, root,
			30, 30, pWidth, pHeight, 0, vinfo->depth, CopyFromParent,
			vinfo->visual, CWBackPixel | CWBorderPixel | CWColormap, &pAttr);

		::XMapWindow(pDisplay, pWindow);
		::XStoreName(pDisplay, pWindow, "Yuni Window");

		pContext = ::glXCreateContext(pDisplay, vinfo, nullptr, True);
		if (pContext == NULL)
		{
			std::cerr << "glXCreateContext failed" << std::endl;
			return false;
		}

		typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

		glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
		glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
			::glXGetProcAddressARB((const GLubyte *) "glXCreateContextAttribsARB");

		// Try to get a context with a higher GL version
		if (glXCreateContextAttribsARB)
		{
			// Get a matching FB config
			static const int visualAttribs[] =
				{
					GLX_X_RENDERABLE    , True,
					GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
					GLX_RENDER_TYPE     , GLX_RGBA_BIT,
					GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
					GLX_RED_SIZE        , 8,
					GLX_GREEN_SIZE      , 8,
					GLX_BLUE_SIZE       , 8,
					GLX_ALPHA_SIZE      , 8,
					GLX_DEPTH_SIZE      , 24,
					GLX_STENCIL_SIZE    , 8,
					GLX_DOUBLEBUFFER    , True,
					//GLX_SAMPLE_BUFFERS  , 1,
					//GLX_SAMPLES         , 4,
					0
				};

			PFNGLXCHOOSEFBCONFIGPROC glXChooseFBConfig = 0;
			glXChooseFBConfig = (PFNGLXCHOOSEFBCONFIGPROC)::glXGetProcAddress((const GLubyte*)"glXChooseFBConfig");

			int fbCount;
			GLXFBConfig* fbConfig = glXChooseFBConfig(pDisplay, DefaultScreen(pDisplay), visualAttribs, &fbCount);

			static const int contextAttribs[] =
				{
					GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
					GLX_CONTEXT_MINOR_VERSION_ARB, 3,
					//GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
					0
				};
			pContext = glXCreateContextAttribsARB(pDisplay, *fbConfig, 0, True, contextAttribs);

			// Sync to ensure any errors generated are processed.
			::XSync(pDisplay, False);
		}

		if (!::glXMakeCurrent(pDisplay, pWindow, pContext))
		{
			std::cout << "glXMakeCurrent failed" << std::endl;
			return false;
		}
		resize(pWidth, pHeight);
		if (!GLWindow::initialize())
		{
			std::cerr << "GL initialization failed" << std::endl;
			return false;
		}

		pMouse = new Input::X11Mouse(pDisplay, pWindow);
		return true;
	}

	void GLXWindow::kill()
	{
		if (pContext)
		{
			if (!::glXMakeCurrent(pDisplay, 0, nullptr))
			{
				std::cerr << "Could not release drawing context." << std::endl;
			}
			::glXDestroyContext(pDisplay, pContext);
			pContext = nullptr;
		}
		// Switch back to original desktop resolution if we were in fs
		if (pFullScreen)
		{
// 			XF86VidModeSwitchToMode(pDisplay, pScreen, &GLWin.deskMode);
// 			XF86VidModeSetViewPort(pDisplay, pScreen, 0, 0);
		}
		::XCloseDisplay(pDisplay);
		pDisplay = nullptr;

		GLWindow::kill();
	}


	void GLXWindow::icon(const AnyString& path)
	{
		// TODO
	}


	void GLXWindow::title(const AnyString& path)
	{
		::XStoreName(pDisplay, pWindow, path.c_str());
	}


	bool GLXWindow::vsync() const
	{
		typedef int (*SwapGetIntervalProto)();
		SwapGetIntervalProto getSwapIntervalEXT = 0;
		const String extensions((const char*)::glGetString(GL_EXTENSIONS));

		if (extensions.find("GLX_MESA_swap_control") != String::npos)
		{
			getSwapIntervalEXT = (SwapGetIntervalProto)::glXGetProcAddress((const GLubyte*)"glXGetSwapIntervalMESA");
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
		const String extensions((const char*)::glGetString(GL_EXTENSIONS));

		if (extensions.find("GLX_SGI_swap_control") != String::npos)
			swapIntervalEXT = (SwapIntervalProto)::glXGetProcAddress((const GLubyte*)"glXSwapIntervalSGI");
		else
		{
			if (extensions.find("GLX_MESA_swap_control") != String::npos)
				swapIntervalEXT = (SwapIntervalProto)::glXGetProcAddress((const GLubyte*)"glXSwapIntervalMESA");
		}

		if (swapIntervalEXT)
			swapIntervalEXT(active ? 1 : 0);
	}



	bool GLXWindow::loop()
	{
		XEvent event;
		while (true)
		{
			::XNextEvent(pDisplay, &event);
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
