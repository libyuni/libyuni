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

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>



namespace Yuni
{
namespace Device
{
namespace Display
{


	static void refreshForX11(MonitorsFound& lst)
	{
		::Display* display = XOpenDisplay(NULL);
		if (display)
		{
			const Monitor::Handle scCount = ScreenCount(display);

			for (Monitor::Handle i = 0; i != scCount; ++i)
			{
				int depCount;
				int* depths = XListDepths(display, i, &depCount);

				Monitor::Ptr newMonitor(new Monitor(String(), i, (0 == i), true, false));
				SmartPtr<OrderedResolutions> res(new OrderedResolutions());

				// All resolutions
				int count;
				XRRScreenSize* list = XRRSizes(display, i, &count);
				for (int i = 0; i < count; ++i)
				{
					XRRScreenSize* it = list + i;
					for (int j = 0; j < depCount; ++j)
					{
						const int d = *(depths + j);
						if (d == 8 || d == 16 || d == 24 || d == 32)
							(*res)[it->width][it->height][(uint8) d] = true;
					}
				}

				lst.push_back(SingleMonitorFound(newMonitor, res));
				XFree(depths);
			}
			XCloseDisplay(display);
		}
	}


	static void refreshOSSpecific(MonitorsFound& lst)
	{
		refreshForX11(lst);
	}





} // namespace Display
} // namespace Device
} // namespace Yuni

