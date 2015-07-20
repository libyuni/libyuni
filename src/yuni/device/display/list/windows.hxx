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
#pragma once
#include "../../../core/system/windows.hdr.h"



namespace Yuni
{
namespace Device
{
namespace Display
{

	namespace // anonymous
	{

		static SingleMonitorFound* findMonitor(const wchar_t* monitorID, MonitorsFound& lst)
		{
			// Converting from wide char to multibyte in order to compare with Yuni::String
			const int sizeRequired = WideCharToMultiByte(CP_UTF8, 0, monitorID, -1, NULL, 0,  NULL, NULL);
			if (sizeRequired <= 0)
				return nullptr;

			String newID;
			newID.reserve((uint) sizeRequired);
			WideCharToMultiByte(CP_UTF8, 0, monitorID, -1, (char*)newID.data(), sizeRequired,  NULL, NULL);
			newID.resize(((uint) sizeRequired) - 1);

			uint i;
			for (i = 0; i < lst.size() and lst[i].first->guid() != newID; ++i)
			{
				// does nothing
			}

			return (i >= lst.size()) ? nullptr : &lst[i];
		}


		static void addResolutions(DISPLAY_DEVICEW& device, SmartPtr<OrderedResolutions> res)
		{
			DEVMODEW devMode;
			devMode.dmSize = (WORD)sizeof(devMode);
			devMode.dmDriverExtra = 32;

			for (uint i = 0; EnumDisplaySettingsW(device.DeviceName, i, &devMode); ++i)
			{
				(*res)[devMode.dmPelsWidth][devMode.dmPelsHeight][(uint8)devMode.dmBitsPerPel] = true;
			}
		}


		/*!
		 ** \brief Windows-specific implementation for the monitor / resolution list refresh
		 */
		static void refreshForWindows(MonitorsFound& lst)
		{
			DISPLAY_DEVICEW displayDevice;
			displayDevice.cb = (DWORD)sizeof(DISPLAY_DEVICEW);
			// Loop on all display devices
			for (uint countDevices = 0; EnumDisplayDevicesW(NULL, countDevices, (DISPLAY_DEVICEW*)&displayDevice, 0); ++countDevices)
			{
				// Ignore mirrored displays
				if (not (displayDevice.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) and (displayDevice.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))
				{
					DISPLAY_DEVICEW monitorDisplayDevice;
					monitorDisplayDevice.cb = (DWORD)sizeof(DISPLAY_DEVICEW);
					// A second call is necessary to get the monitor name associated with the display
					EnumDisplayDevicesW(displayDevice.DeviceName, 0, (DISPLAY_DEVICEW*)&monitorDisplayDevice, 0);
					bool mainDisplay = (0 != (displayDevice.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE));

					// Check if we have already stored info on this monitor
					SingleMonitorFound* monitorWithRes = findMonitor(monitorDisplayDevice.DeviceID, lst);
					bool newMonitor = (NULL == monitorWithRes);
					Monitor::Ptr monitor;
					SmartPtr<OrderedResolutions> res;

					if (newMonitor)
					{
						// Converting from wide char to multibyte in order to compare with Yuni::String
						const int sizeRequired = WideCharToMultiByte(CP_UTF8, 0, monitorDisplayDevice.DeviceString, -1, NULL, 0,  NULL, NULL);
						if (sizeRequired <= 0)
							continue;

						String newID;
						newID.reserve((uint) sizeRequired);
						WideCharToMultiByte(CP_UTF8, 0, monitorDisplayDevice.DeviceString, -1, (char*)newID.data(), sizeRequired,  NULL, NULL);
						newID.resize(((uint) sizeRequired) - 1);
						// Create the new monitor
						monitor = new Monitor(newID, (Monitor::Handle)monitorDisplayDevice.DeviceID, mainDisplay, true, true);
						res = new OrderedResolutions();
					}
					else
					{
						monitor = monitorWithRes->first;
						res = monitorWithRes->second;
					}

					// Add associated resolutions
					addResolutions(displayDevice, res);

					// Add the monitor and its resolutions to the list if necessary
					if (newMonitor)
						lst.push_back(SingleMonitorFound(monitor, res));
					if (countDevices > YUNI_DEVICE_MONITOR_COUNT_HARD_LIMIT)
						break;
				}
			}
		}



		static inline void refreshOSSpecific(MonitorsFound& lst)
		{
			refreshForWindows(lst);
		}


	} // anonymous namespace



} // namespace Display
} // namespace Device
} // namespace Yuni
