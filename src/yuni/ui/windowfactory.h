
#ifndef __YUNI_UI_WINDOW_FACTORY__
# define __YUNI_UI_WINDOW_FACTORY__

# include "../yuni.h"
# include "../core/string.h"

namespace Yuni
{
namespace UI
{


	//! Forward declaration
	class RenderWindow;


	//! Factory for creating platform-specific windows
	class WindowFactory
	{
	public:
		static RenderWindow* CreateGLWindow(const AnyString& title, uint width,
			uint height, uint bitDepth, bool fullScreen);

	private:
		WindowFactory() {}
		WindowFactory(const WindowFactory&) {}

	}; // class WindowFactory



} // UI
} // Yuni

#endif // __YUNI_UI_WINDOW_FACTORY__
