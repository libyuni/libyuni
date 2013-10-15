
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
		GLXWindow(const AnyString& title, unsigned int width, unsigned int height, unsigned int bitDepth, bool fullScreen) :
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
