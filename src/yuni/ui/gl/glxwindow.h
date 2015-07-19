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
#ifndef __YUNI_UI_GLXWINDOW_H__
# define __YUNI_UI_GLXWINDOW_H__

#include <yuni/yuni.h>
#include <yuni/private/graphics/opengl/glew/glew.h>
#include <yuni/private/graphics/opengl/glew/glxew.h>
#include "glwindow.h"

namespace Yuni
{
namespace UI
{


	/*!
	** \brief Specific implementation of a window for OpenGL rendering under MS Windows
	*/
	class GLXWindow: public GLWindow
	{
	public:
		//! Constructor
		GLXWindow(const AnyString& title, uint width, uint height, uint bitDepth, bool fullScreen) :
			GLWindow(title, width, height, bitDepth, fullScreen)
		{}

		//! Virtual destructor
		virtual ~GLXWindow() {}

		//! Initialize the window
		virtual bool initialize() override;

		//! Main event loop
		virtual bool loop();

		//! Kill the window, release everything
		virtual void kill() override;

		//! Minimize the window
		virtual void minimize() override;

		//! Minimize the window
		virtual void maximize() override;

		//! Minimize the window
		virtual void restore() override;

		//! Swap front and back buffers (OS-specific)
		virtual void swapBuffers() override;

		//! Refresh and swap buffers with a single polymorphic call
		virtual void refreshAndSwap() override;

		//! Set window icon
		virtual void setIcon(const AnyString& path) override;

		//! Enable / Disable vertical sync
		virtual void vsync(bool enable) override;
		//! Current status of the vertical sync
		virtual bool vsync() const override;

		//! Does the window have Full Screen AntiAliasing / MultiSampling ?
		virtual bool antiAliasing() const override;
		/*!
		** \brief Should Full Screen AntiAliasing / MultiSampling be enabled ?
		**
		** Changing this value may kill and re-create the window.
		*/
		virtual void antiAliasing(bool enable) override;

		//! Enable / Disable full screen
		virtual void fullScreen(bool enable) override;
		//! Is the window full screen ?
		virtual bool fullScreen() const override;

	private:
		Display* pDisplay;
		int pScreen;
		Window pWindow;
		GLXContext pContext;
		XSetWindowAttributes pAttr;
		XWindowAttributes pWndAttr;

	}; // GLXWindow


} // UI
} // Yuni

#include "glxwindow.hxx"

#endif // __YUNI_UI_GLXWINDOW_H__
