#ifndef __YUNI_UI_WGLWINDOW_H__
# define __YUNI_UI_WGLWINDOW_H__

# include <windows.h>
# include <map>
# include "glwindow.h"
# include "../input/winmouse.h"
# include "../input/key.h"

namespace Yuni
{
namespace UI
{


	/*!
	** \brief Specific implementation of a window for OpenGL rendering under MS Windows
	*/
	class WGLWindow: public GLWindow
	{
	public:
		//! List of local GDI windows, by HWND
		typedef std::map<HWND, WGLWindow*> WindowList;

	public:
		//! Constructor
		WGLWindow(const AnyString& title, uint width, uint height, uint bitDepth, bool fullScreen):
			GLWindow(title, width, height, bitDepth, fullScreen)
		{
			pMouse = new Input::WinMouse();
		}

		//! Initialize the window
		virtual bool initialize() override;

		//! Main event loop
		virtual bool loop() override;

		//! Kill the window, release everything
		virtual void kill() override;

		//! Minimize the window
		virtual void minimize() override { if (wsMinimized != pState) ::ShowWindow(pHWnd, SW_MINIMIZE); }

		//! Minimize the window
		virtual void maximize() override { if (wsMaximized != pState) ::ShowWindow(pHWnd, SW_MAXIMIZE); }

		//! Minimize the window
		virtual void restore() override { if (wsNormal != pState) ::ShowWindow(pHWnd, SW_RESTORE); }

		//! Swap front and back buffers (OS-specific)
		virtual void swapBuffers() override { ::SwapBuffers(pHDC); }

		//! Refresh and swap buffers with a single polymorphic call
		virtual void refreshAndSwap() override { pRefreshFunc(); ::SwapBuffers(pHDC); }

		//! Set window icon
		virtual void setIcon(const AnyString& path) override;

		//! Enable / Disable vertical sync (OS-specific)
		virtual void vsync(bool enable) override;
		//! Current status of the vertical sync (OS-specific)
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
		/*!
		** \brief Callback method for windows events
		**
		** \param hWnd Handle for this window
		** \param uMsg Message
		** \param wParam Additional Message Information
		** \param lParam Additional Message Information
		*/
		static LRESULT CALLBACK MessageCallback(HWND handle, UINT uMsg, WPARAM wParam, LPARAM lParam);

		//! Find a window given its handle
		static WGLWindow* FindWindow(HWND handle);

		//! Register a window with its handle as a key
		static void RegisterWindow(HWND handle, WGLWindow* window);

		//! Unregister the window, happens when closing it
		static void UnregisterWindow(HWND handle);

		//! Current number of registered windows
		static uint WindowCount();

		//! Convert a Windows VK_* code to the equivalent Yuni code
		static Yuni::Input::Key ConvertInputKey(WPARAM keyCode);

		//! Enable full screen mode, use primary monitor with desktop resolution
		bool enableFullScreen();

		//! Disable full screen mode
		bool disableFullScreen();

		//! Window initialization, we need to be able to keep the context when reinitializing
		bool initWindow(bool keepContext);

		//! Close and release all the necessary stuff to reinitialize the window
		void closeWindowForReinit();

		//! Initialize the pixel format with default values
		bool initDefaultPixelFormat(const PIXELFORMATDESCRIPTOR& pfd);

		//! Initialize the pixel format for 4x multi-sampling (with 2x fallback)
		bool initMultisamplePixelFormat(const PIXELFORMATDESCRIPTOR& pfd);

	protected:
		//! Static list of registered GDI windows
		static WindowList sWindowList;

		// # ifdef YUNI_OS_WINDOWS
		friend LRESULT CALLBACK MessageCallback(HWND handle, UINT uMsg, WPARAM wParam, LPARAM lParam);
		// # endif

	private:
		HINSTANCE pHInstance;
		HWND pHWnd;
		HDC pHDC;
		HGLRC pHRC;

		Logs::Logger<> logs;

	}; // class WGLWindow






} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_WGLWINDOW_H__
