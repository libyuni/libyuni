/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** gitlab: https://gitlab.com/libyuni/libyuni/
** github: https://github.com/libyuni/libyuni/ {mirror}
*/

#include "../../yuni.h"
#include <iostream>
#include "../windowfactory.h"
#include "glxwindow.h"


namespace Yuni
{
namespace UI
{


	namespace // anonymous
	{

		// X11 key codes
		// source : http://www.comptechdoc.org/os/linux/howlinuxworks/linux_hlkeycodes.html
		const Input::Key KeyConversions[] =
		{
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::Escape,
			Input::Key::N1,
			Input::Key::N2,
			Input::Key::N3,
			Input::Key::N4,
			Input::Key::N5,
			Input::Key::N6,
			Input::Key::N7,
			Input::Key::N8,
			Input::Key::N9,
			Input::Key::N0,
			Input::Key::Minus,
			Input::Key::Equal,
			Input::Key::Backspace,
			Input::Key::Tab,
			Input::Key::Q,
			Input::Key::W,
			Input::Key::E,
			Input::Key::R,
			Input::Key::T,
			Input::Key::Y,
			Input::Key::U,
			Input::Key::I,
			Input::Key::O,
			Input::Key::P,
			Input::Key::BracketL,
			Input::Key::BracketR,
			Input::Key::Enter,
			Input::Key::ControlL,
			Input::Key::A,
			Input::Key::S,
			Input::Key::D,
			Input::Key::F,
			Input::Key::G,
			Input::Key::H,
			Input::Key::J,
			Input::Key::K,
			Input::Key::L,
			Input::Key::Semicolon,
			Input::Key::Quote,
			Input::Key::Backquote,
			Input::Key::ShiftL,
			Input::Key::Backslash,
			Input::Key::Z,
			Input::Key::X,
			Input::Key::C,
			Input::Key::V,
			Input::Key::B,
			Input::Key::N,
			Input::Key::M,
			Input::Key::Comma,
			Input::Key::Dot,
			Input::Key::Slash,
			Input::Key::ShiftR,
			Input::Key::Asterisk,
			Input::Key::AltL,
			Input::Key::Space,
			Input::Key::CapsLock,
			Input::Key::F1,
			Input::Key::F2,
			Input::Key::F3,
			Input::Key::F4,
			Input::Key::F5,
			Input::Key::F6,
			Input::Key::F7,
			Input::Key::F8,
			Input::Key::F9,
			Input::Key::F10,
			Input::Key::NumLock,
			Input::Key::ScrollLock,
			Input::Key::KP7,
			Input::Key::KP8,
			Input::Key::KP9,
			Input::Key::KPMinus,
			Input::Key::KP4,
			Input::Key::KP5,
			Input::Key::KP6,
			Input::Key::KPPlus,
			Input::Key::KP1,
			Input::Key::KP2,
			Input::Key::KP3,
			Input::Key::KPInsert,
			Input::Key::KPDelete,
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::F11,
			Input::Key::F12,
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::KPEnter,
			Input::Key::ControlR,
			Input::Key::KPSlash,
			Input::Key::PrintScreen,
			Input::Key::AltR,
			Input::Key::Invalid,
			Input::Key::Home,
			Input::Key::Up,
			Input::Key::PageUp,
			Input::Key::Left,
			Input::Key::Right,
			Input::Key::End,
			Input::Key::Down,
			Input::Key::PageDown,
			Input::Key::Insert,
			Input::Key::Delete,
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::Invalid,
			Input::Key::Pause
		};


		bool GetBestFBConfig(GLXFBConfig& bestConfig, XVisualInfo*& bestVinfo, ::Display* display, const int* visualAttribs, uint nbSamples = 0)
		{
			static const PFNGLXCHOOSEFBCONFIGPROC glXChooseFBConfig =
				(PFNGLXCHOOSEFBCONFIGPROC)::glXGetProcAddress((const GLubyte*)"glXChooseFBConfig");
			static const PFNGLXGETVISUALFROMFBCONFIGPROC glXGetVisualFromFBConfig =
				(PFNGLXGETVISUALFROMFBCONFIGPROC)::glXGetProcAddress((const GLubyte*)"glXGetVisualFromFBConfig");
			static const PFNGLXGETFBCONFIGATTRIBPROC glXGetFBConfigAttrib =
				(PFNGLXGETFBCONFIGATTRIBPROC)::glXGetProcAddress((const GLubyte*)"glXGetFBConfigAttrib");
			int fbCount;
			GLXFBConfig* fbc = glXChooseFBConfig(display, DefaultScreen(display), visualAttribs, &fbCount);
			if (!fbc)
				return false;

			// Pick the FB config/visual with the most samples per pixel
			int foundIndex = -1;
			XVisualInfo* vi = nullptr;
			for (int i = 0; i < fbCount; ++i)
			{
				vi = glXGetVisualFromFBConfig(display, fbc[i]);
				if (vi)
				{
					int samp_buf;
					int samples;
					glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf);
					glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLES, &samples);

					if ((uint)samples == nbSamples && (nbSamples == 0 || samp_buf))
					{
						foundIndex = i;
						break;
					}
				}
				::XFree(vi);
			}
			if (foundIndex == -1)
				return false;
			bestVinfo = vi;
			bestConfig = fbc[foundIndex];
			::XFree(fbc);
			return true;
		}

	} // namespace anonymous



	bool GLXWindow::initialize()
	{
		if (!(pDisplay = ::XOpenDisplay(NULL)))
			return false;

		static const int visualAttribs[] = {
			GLX_X_RENDERABLE    , True,
			GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
			GLX_RENDER_TYPE     , GLX_RGBA_BIT,
			GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
			GLX_RGBA,
/*			GLX_RED_SIZE        , 8,
			GLX_GREEN_SIZE      , 8,
			GLX_BLUE_SIZE       , 8,
			GLX_ALPHA_SIZE      , 8,
			GLX_DEPTH_SIZE      , 24,
			GLX_STENCIL_SIZE    , 8,*/
			GLX_DOUBLEBUFFER    , True,
/*			GLX_SAMPLE_BUFFERS  , 1,
			GLX_SAMPLES         , 4,*/
			0
		};

		::XVisualInfo* vinfo = nullptr;
		GLXFBConfig fbConfig;
		if (not GetBestFBConfig(fbConfig, vinfo, pDisplay, visualAttribs))
		{
			std::cerr << "Failed to get valid FBConfig" << std::endl;
			return false;
		}

		Window root = DefaultRootWindow(pDisplay);

		XSetWindowAttributes attr;
		attr.colormap = ::XCreateColormap(pDisplay, root, vinfo->visual, AllocNone);
		attr.background_pixel = BlackPixel(pDisplay, vinfo->screen);
		attr.border_pixel = BlackPixel(pDisplay, vinfo->screen);
		// Window creation
		pWindow = ::XCreateWindow(pDisplay, root,
			30, 30, pWidth, pHeight, 0, vinfo->depth, CopyFromParent,
			vinfo->visual, CWBackPixel | CWBorderPixel | CWColormap, &attr);

		::XStoreName(pDisplay, pWindow, "Yuni Window");

		::XSelectInput(pDisplay, pWindow,
			ExposureMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
			KeyPressMask | KeyReleaseMask | EnterWindowMask | LeaveWindowMask | StructureNotifyMask);

		// We need a dummy context to initialize everything
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
			static const int contextAttribs[] =
				{
					GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
#if 0
//#ifdef GL_VERSION_3_3
					// 3.3 deprecates glMatrix* features and we need them for now
					GLX_CONTEXT_MINOR_VERSION_ARB, 3,
#else
					GLX_CONTEXT_MINOR_VERSION_ARB, 0,
#endif
					//GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
					0
				};
			pContext = glXCreateContextAttribsARB(pDisplay, fbConfig, 0, True, contextAttribs);

			// Sync to ensure any errors generated are processed.
			::XSync(pDisplay, False);
		}

		if (not ::glXIsDirect(pDisplay, pContext))
			std::cout << "Warning : GL context is not hardware-accelerated !" << std::endl;

		if (not ::glXMakeCurrent(pDisplay, pWindow, pContext))
		{
			std::cerr << "glXMakeCurrent failed" << std::endl;
			return false;
		}
		if (not GLWindow::initialize())
		{
			std::cerr << "GL initialization failed" << std::endl;
			return false;
		}

		// Show window
		::XMapWindow(pDisplay, pWindow);

		GLWindow::resize(pWidth, pHeight);
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

	void GLXWindow::move(uint left, uint top)
	{
		GLWindow::move(left, top);
		::XMoveWindow(pDisplay, pWindow, left, top);
	}

	void GLXWindow::resize(unsigned int width, unsigned int height)
	{
		GLWindow::resize(width, height);
		::XResizeWindow(pDisplay, pWindow, width, height);
	}


	void GLXWindow::fullScreen(bool enable)
	{
		typedef struct
		{
			unsigned long flags;
			unsigned long functions;
			unsigned long decorations;
			long input_mode;
			unsigned long status;
		} MWMHints;

		enum
		{
			MWM_HINTS_FUNCTIONS = (1L << 0),
			MWM_HINTS_DECORATIONS =  (1L << 1),
			MWM_FUNC_ALL = (1L << 0),
			MWM_FUNC_RESIZE = (1L << 1),
			MWM_FUNC_MOVE = (1L << 2),
			MWM_FUNC_MINIMIZE = (1L << 3),
			MWM_FUNC_MAXIMIZE = (1L << 4),
			MWM_FUNC_CLOSE = (1L << 5),
			MWM_DECOR_ALL = (1L << 0),

			PROP_MWM_HINTS_ELEMENTS = 5
		};

		MWMHints hints;
		::memset(&hints, 0, sizeof(hints));
		// Specify that we're changing the window decorations.
		hints.flags = MWM_HINTS_DECORATIONS;
		// Set whether to remove window decorations
		hints.decorations = enable ? 0 : MWM_DECOR_ALL;

		::Atom property = ::XInternAtom(pDisplay, "_MOTIF_WM_HINTS", False);
		::XChangeProperty(pDisplay, pWindow, property, property, 32, PropModeReplace, (uint8*)&hints, PROP_MWM_HINTS_ELEMENTS);

		int newX;
		int newY;
		uint newWidth;
		uint newHeight;
		if (not enable)
		{
			newX = pPrevX;
			newY = pPrevY;
			newWidth = pPrevWidth;
			newHeight = pPrevHeight;
		}
		else
		{
			pPrevX = pLeft;
			pPrevY = pTop;
			pPrevWidth = width();
			pPrevHeight = height();
			newX = 0;
			newY = 0;
			::Screen* screen = DefaultScreenOfDisplay(pDisplay);
			newWidth = screen->width;
			newHeight = screen->height;
		}

		::XMoveResizeWindow(pDisplay, pWindow, newX, newY, newWidth, newHeight);
		GLWindow::move(newX, newY);
		GLWindow::resize(newWidth, newHeight);

		if (enable)
			::XMapRaised(pDisplay, pWindow);
		else
			::XMapWindow(pDisplay, pWindow);
		pFullScreen = enable;
	}


	bool GLXWindow::loop()
	{
		XEvent event;

		auto mask = ExposureMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
			KeyPressMask | KeyReleaseMask | EnterWindowMask | LeaveWindowMask | StructureNotifyMask;
		while (::XCheckWindowEvent(pDisplay, pWindow, mask, &event))
		{
			//::XNextEvent(pDisplay, &event);
			switch (event.type)
			{
				case ConfigureNotify:
					GLWindow::move(event.xconfigure.x, event.xconfigure.y);
					GLWindow::resize(event.xconfigure.width, event.xconfigure.height);
					break;
				case Expose:
					refreshAndSwap();
					break;
				case ButtonPress:
					doMouseDown((Input::IMouse::Button)event.xbutton.button);
					break;
				case ButtonRelease:
					doMouseUp((Input::IMouse::Button)event.xbutton.button);
					break;
				case MotionNotify:
					doMouseMove(event.xbutton.x, event.xbutton.y);
					break;
				case KeyPress:
					doKeyDown(KeyConversions[event.xkey.keycode]);
					break;
				case KeyRelease:
					doKeyUp(KeyConversions[event.xkey.keycode]);
					break;
				case EnterNotify:
					break;
				case LeaveNotify:
					doMouseLeave();
					break;
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
