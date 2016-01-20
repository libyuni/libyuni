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
/*
**  This code is heavily modified by the Yuni project but largely inspired by CMacs :
**  Created by Richard Ross on 4/23/12.
**  Copyright (c) 2012 Ultimate Computer Services, Inc. All rights reserved.
*/
#pragma once

# include "../../yuni.h"
# include "glwindow.h"
# include "CMacsTypes.h"
//# include "../input/winmouse.h"
# include "../input/key.h"

namespace Yuni
{
namespace UI
{


	/*!
	** \brief Specific implementation of a window for OpenGL rendering under OS X / Cocoa
	*/
	class NSGLWindow: public GLWindow
	{
	public:
		//! List of local windows, by id
		typedef std::map<id, NSGLWindow*> WindowList;

	public:
		//! Constructor
		NSGLWindow(const AnyString& title, uint width, uint height, uint bitDepth, bool fullScreen):
			GLWindow(title, width, height, bitDepth, fullScreen)
		{
			//pMouse = new Input::OSXMouse();
		}
		//! Destructor
		~NSGLWindow();

		//! Initialize the window
		virtual bool initialize() override;

		//! Main event loop
		virtual bool loop() override;

		//! Kill the window, release everything
		virtual void kill() override;

		//! Minimize the window
		virtual void minimize() override { /*if (wsMinimized != pState) ::ShowWindow(pHWnd, SW_MINIMIZE);*/ }

		//! Minimize the window
		virtual void maximize() override { /*if (wsMaximized != pState) ::ShowWindow(pHWnd, SW_MAXIMIZE);*/ }

		//! Restore the window when minimized
		virtual void restore() override { /*if (wsNormal != pState) ::ShowWindow(pHWnd, SW_RESTORE);*/ }

		//! Force a window resize event
		virtual void resize(uint width, uint height) override {}

		//! Swap front and back buffers (OS-specific)
		virtual void swapBuffers() override { ::glFlush(); }

		//! Refresh and swap buffers with a single polymorphic call
		virtual void refreshAndSwap() override { onRefresh(); ::glFlush(); }

		//! Set window icon
		virtual void icon(const AnyString& path) override {}

		//! Set window title
		virtual void title(const AnyString& path) override {}

		//! Enable / Disable vertical sync (OS-specific)
		virtual void vsync(bool enable) override {}
		//! Current status of the vertical sync (OS-specific)
		virtual bool vsync() const override { return true; }

		/*!
		** \brief Should Full Screen AntiAliasing / MultiSampling be enabled ?
		**
		** Changing this value may kill and re-create the window.
		*/
	    virtual void multiSampling(MultiSampling::Type samplingType) override {}

		//! Enable / Disable full screen
		virtual void fullScreen(bool enable) override {}
		//! Is the window full screen ?
		virtual bool fullScreen() const override { return false; }

	protected:
		//! Find a window given its handle
		static NSGLWindow* FindWindow(id windowId);

		//! Register a window with its handle as a key
		static void RegisterWindow(id windowId, NSGLWindow* window);

		//! Unregister the window, happens when closing it
		static void UnregisterWindow(id windowId);

		// Event callbacks
		static void OnDrawRect(id self, SEL _cmd, CMRect rect);
		static void OnMouseDown(id self, SEL _cmd, void* event);
		static void OnWindowClose();

	private:
		//! Convert a Windows VK_* code to the equivalent Yuni code
		//		static Yuni::Input::Key ConvertInputKey(WPARAM keyCode);

		//! Enable full screen mode, use primary monitor with desktop resolution
		bool enableFullScreen();

		//! Disable full screen mode
		bool disableFullScreen();

		//! Window initialization
		bool initWindow();

		//! Only resize the internal data, this supposes the window is actually already resized
		void internalResize(uint width, uint height);

	protected:
		//! Static list of registered GDI windows
		static WindowList sWindowList;

	private:
		id pID;

		Logs::Logger<> logs;

	}; // class NSGLWindow






} // namespace UI
} // namespace Yuni