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
#ifndef __YUNI_UI_RENDERWINDOW_H__
# define __YUNI_UI_RENDERWINDOW_H__

# undef None
# include "../yuni.h"
# include "../core/bind.h"
# include "../core/string.h"
# include <list>
# include "input/key.h"
# include "input/keyboard.h"
# include "input/mouse.h"
# include "font.h"
# include "multisampling.h"
# include "gl/posteffect.h"
# include "gl/framebuffer.h"
# include "gl/view.h"


namespace Yuni
{
namespace UI
{


	/*!
	** \brief Abstraction of a window for graphic rendering
	*/
	class RenderWindow
	{
	public:
		//! Smart pointer
		typedef SmartPtr<RenderWindow>  Ptr;

		//! Delegate type for the Refresh callback
		typedef Yuni::Bind<void (void)>  RefreshDelegate;

		//! Delegate type for the Resize callback
		typedef Yuni::Bind<void (uint width, uint height)>  ResizeDelegate;

		//! Delegate type for the CleanUp callback
		typedef Yuni::Bind<void ()> CleanUpDelegate;


	public:
		//! State of the render window
		enum WindowState
		{
			wsNormal,
			wsMinimized,
			wsMaximized

		}; // enum WindowState


	public:
		//! Callback for window refresh
		RefreshDelegate onRefresh;

		//! Callback for window resize
		ResizeDelegate onResize;

		//! Callback for window clean-up, called before closing
		CleanUpDelegate onCleanUp;


	public:
		//! \name Constructors & Destructor
		//@{
		/*!
		** \brief Constructor
		*/
		RenderWindow(const AnyString& title, uint width, uint height, uint bitDepth, bool fullScreen);
		//! Destructor
		virtual ~RenderWindow();
		//@}

		//! \name Window manipulation
		//@{
		/*!
		** \brief Initialize the window
		**
		** This should be overridden and must be called with RenderWindow::initialize().
		** It cannot be const because if init fails, we may have to switch to fallback values.
		*/
		virtual bool initialize() { return pFB.initialize(Gfx3D::FrameBuffer::fbPingPong, pMultiSampling); }

		/*!
		** \brief Kill the window, release everything
		**
		** This is implementation-dependent
		*/
		virtual void kill();

		//! Main event loop
		virtual bool loop() = 0;

		/*!
		** \brief Move the window
		**
		** This is implementation-dependent
		*/
		virtual void move(uint left, uint top)
		{
			pLeft = left;
			pTop = top;
		}

		/*!
		** \brief Resize the window
		**
		** This is implementation-dependent
		*/
		virtual void resize(uint width, uint height)
		{
			pWidth = width;
			pHeight = height;
			pFB.resize(width, height);
			onResize(width, height);
		}

		/*!
		** \brief Minimize the window
		**
		** This is implementation-dependent
		*/
		virtual void minimize() = 0;

		/*!
		** \brief Minimize the window
		**
		** This is implementation-dependent
		*/
		virtual void maximize() = 0;

		/*!
		** \brief Restore the window
		**
		** This is implementation-dependent
		*/
		virtual void restore() = 0;
		//@}

		//! \name Callbacks
		//@{
		//! Refresh the display
		virtual void refresh() { onRefresh(); }

		//! Refresh the display then swap buffers
		virtual void refreshAndSwap() = 0;

		//! Manage mouse input events
		Input::IMouse& mouse()
		{
			assert(pMouse && "RenderWindow inheritors must instantiate the proper Mouse type !");
			return *pMouse;
		}

		//! Manage keyboard input events
		Input::Keyboard& keyboard() { return pKeyboard; }
		//@}

		//! \name Clear functions
		//@{
		//! Clear the rendering area to black
		virtual void clear() const = 0;
		//! Clear the rendering area to a given color
		virtual void clear(const Color::RGB<>& color) const = 0;

		//! Clear a rectangle in the rendering area to black
		virtual void clearRect(int x, int y, uint width, uint height) const = 0;

		//! Clear a view's surface to black
		void clearView(const UUID& viewID);
		void clearView(const View::Ptr& view);
		//@}

		//! Draw a textured quad on the whole window
		virtual void drawFullWindowQuad(const Gfx3D::Texture::Ptr& texture) const = 0;

		//! Swap front and back buffers (OS-specific)
		virtual void swapBuffers() = 0;

		//! Set window icon (OS-specific)
		virtual void setIcon(const AnyString& path) = 0;

		//! Enable / Disable vertical sync (OS-specific)
		virtual void vsync(bool enable) = 0;
		//! Current status of the vertical sync (OS-specific)
		virtual bool vsync() const = 0;

		//! State of the window : normal, minimized, maximized
		bool minimized() const { return wsMinimized == pState; }

		//! Do we currently have any kind of multisampling on ?
		bool multiSampling() const { return MultiSampling::msNone != pMultiSampling; }
		//! Change the type of multisampling applied
		virtual void multiSampling(MultiSampling::Type samplingType);
		//! Get the current multisampling multiplier, 1 if no multisampling
		uint samplingMultiplier() const { return MultiSampling::Multiplier(pMultiSampling); }

		//! Enable / Disable full screen (OS-specific)
		virtual void fullScreen(bool enable) = 0;
		//! Is the window full screen ? (OS-specific)
		virtual bool fullScreen() const = 0;

		//! Left position
		uint left() const { return pLeft; }

		//! Top position
		uint top() const { return pTop; }

		//! Width
		uint width() const { return pWidth; }

		//! Height
		uint height() const { return pHeight; }

		//! Color depth
		uint depth() const { return pBitDepth; }


		//! \name View management
		//@{
		//! Create a view
		View::Ptr createView(float x, float y, float w, float h, uint8 z = 127);

		//! Destroy a view, active view cannot be destroyed !
		bool destroyView(const UUID& id);

		//! Get the currently active view
		const View::Ptr& activeView() const;

		//! Get a specific view by ID, can return NULL !
		View::Ptr view(const UUID& id) const;

		//! Set a view active by ID
		bool activeView(const UUID& id);

		//! Set a view active
		bool activeView(const View::Ptr& view);

		//! Attach an existing view to this window
		void attachView(const View::Ptr& view);

		//! Draw one view
		void drawView(const UUID& id) const;

		//! Draw one view
		void drawView(const View::Ptr& view) const;

		//! Draw each view
		void drawAllViews() const;
		//@}

		//! Has the window already been killed ?
		bool killed() const { return pViewList.empty(); }


	protected:
		//! \name Events
		//@{
		void doMouseMove(int x, int y);
		void doMouseDown(Input::IMouse::Button btn);
		void doMouseUp(Input::IMouse::Button btn);
		void doMouseDblClick(Input::IMouse::Button btn);
		//! Up is +, Down is -
		void doMouseScroll(float delta);
		void doMouseHover(int x, int y);
		void doMouseLeave();

		void doKeyDown(Input::Key key);
		void doKeyUp(Input::Key key);
		void doCharInput(const AnyString& str);
		//@}

	protected:
		//! Text displayed in the title bar
		String pTitle;

		//! Window left position
		uint pLeft;
		//! Window top position
		uint pTop;
		//! Window width
		uint pWidth;
		//! Window height
		uint pHeight;

		//! Resolution width
		uint pResWidth;
		//! Resolution height
		uint pResHeight;
		//! Color depth in bits per pixel
		uint pBitDepth;

		//! Frame buffer for render-to-texture
		Gfx3D::FrameBuffer& pFB;

		//! Postprocessing effects
		mutable PostEffect::Vector pPostEffects;

		//! Is the window in full screen ?
		bool pFullScreen;

		//! Type of multisampling applied
		MultiSampling::Type pMultiSampling;

		//! Is the window maximized ? Minimized ?
		WindowState pState;

		//! Currently active view, all operations are done on this one
		View::Ptr pActiveView;
		//! List of all views on this window, sorted by increasing Z-order (back-to-front)
		View::List pViewList;

		//! Mouse data
		Input::IMouse* pMouse;

		Input::Keyboard pKeyboard;

		//! Default font cache
		mutable Font::Ptr pDefaultFont;

	}; // class RenderWindow




} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_RENDERWINDOW_H__
