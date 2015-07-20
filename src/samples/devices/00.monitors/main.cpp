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
#include <yuni/yuni.h>
#include <yuni/device/display/list.h>
#include <iostream>


using namespace Yuni;


void title(const String& t)
{
	std::cout << std::endl << "--- " << t << " ---" << std::endl << std::endl;
}


void printMonitor(Device::Display::Monitor::Ptr monitor)
{
	if (!monitor)
		return;

	// The name of the monitor
	std::cout << "Product name: `"
		<< (monitor->productName().empty() ? "<Unknown>" : monitor->productName())
		<< "`\n";

	// Its GUID
	std::cout << "guid:" << monitor->guid();
	if (monitor->primary())
		std::cout << ", primary";
	if (monitor->hardwareAcceleration())
		std::cout << ", Hardware-Accelerated";
	std::cout << std::endl;

	// All resolutions
	Device::Display::Resolution::Vector::const_iterator it;
	for (it = monitor->resolutions().begin(); it != monitor->resolutions().end(); ++it)
	{
		if (!(*it))
			std::cout << "ERROR" << std::endl;
		else
			std::cout << "  . " << (*it)->toString() << "\n";
	}

	// Space
	std::cout << std::endl;
}



int main(void)
{
	// A list of monitors
	Device::Display::List monitors;

	// The list must be refreshed according the current Operating System settings.
	// Without any parameters, the resolutions are filtered to be higher than
	// a minimal resolution (640x480x8 by default).
	//
	// To be sure to have exactly all resolutions, call `refresh()` like this :
	// monitors.refresh(0, 0, 0);
	//
	monitors.refresh();


	// Displaying information about all available monitors
	title("All available displays, the primary display included");
	Device::Display::List::const_iterator it;
	for (it = monitors.begin(); it != monitors.end(); ++it)
	{
		// Print information about a single monitor
		printMonitor(*it);
	}

	return 0;
}
