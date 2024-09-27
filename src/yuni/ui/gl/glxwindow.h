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
#pragma once

#include <yuni/yuni.h>
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
			GLWindow(title, width, height, bitDepth, fullScreen),
			pPrevX(pLeft),
			pPrevY(pTop),
			pPrevWidth(width),
			pPrevHeight(height)
		{}

		//! Virtual destructor
		virtual ~GLXWindow() {}

		//! Initialize the window
		virtual bool initialize() override;

		//! Move the window
		virtual void move(uint left, uint top) override;

		//! Resize the window
		virtual void resize(unsigned int width, unsigned int height) override;

		//! Main event loop
		virtual bool loop();

		//! Kill the window, release everything
		virtual void kill() override;

		//! Minimize the window
		virtual void minimize() override {} // TODO

		//! Minimize the window
		virtual void maximize() override {} // TODO

		//! Minimize the window
		virtual void restore() override {} // TODO

		//! Swap front and back buffers (OS-specific)
		virtual void swapBuffers() override;

		//! Refresh and swap buffers with a single polymorphic call
		virtual void refreshAndSwap() override;

		//! Set window icon
		virtual void icon(const AnyString& path) override;

		//! Set window title
		virtual void title(const AnyString& path) override;

		//! Enable / Disable vertical sync
		virtual void vsync(bool enable) override;
		//! Current status of the vertical sync
		virtual bool vsync() const override;

		//! Enable / Disable full screen
		virtual void fullScreen(bool enable) override;

	private:
		Display* pDisplay;
		int pScreen;
		Window pWindow;
		GLXContext pContext;

		// Store values on full screen to restore previous window values
		int pPrevX;
		int pPrevY;
		uint pPrevWidth;
		uint pPrevHeight;

	}; // GLXWindow


} // UI
} // Yuni

#include "glxwindow.hxx"
