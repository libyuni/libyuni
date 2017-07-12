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
#include "list.h"
#include <map>




namespace Yuni
{
namespace Device
{
namespace Display
{

	typedef std::map<uint32, std::map<uint32, std::map<uint8, bool> > >  OrderedResolutions;

	typedef std::pair<Ref<Monitor>, SmartPtr<OrderedResolutions> > SingleMonitorFound;

	typedef std::vector<SingleMonitorFound> MonitorsFound;


} // namespace Display
} // namespace Device
} // namespace Yuni




#ifdef YUNI_OS_MAC
#	include "macosx.hxx"
#else
#	ifdef YUNI_OS_WINDOWS
#		include "windows.hxx"
#	endif
#	ifdef YUNI_OS_UNIX
#		include "linux.hxx"
#	endif
#endif





namespace Yuni
{
namespace Device
{
namespace Display
{


	List::List()
		: pNullMonitor(make_ref<Monitor>(YUNI_DEVICE_DISPLAY_LIST_FAIL_SAFE_NAME))
	{
		pMonitors.push_back(pNullMonitor);
		pPrimary = pNullMonitor;
	}


	List::List(const List& c)
		: pMonitors(c.pMonitors)
		, pPrimary(c.pPrimary)
		, pNullMonitor(c.pNullMonitor)
	{}


	List& List::operator = (const List& rhs)
	{
		pMonitors    = rhs.pMonitors;
		pPrimary     = rhs.pPrimary;
		pNullMonitor = rhs.pNullMonitor;
		return *this;
	}


	bool List::refresh(uint32 minWidth, uint32 minHeight, uint8 minDepth)
	{
		pPrimary = pNullMonitor;
		pMonitors.clear();
		// Get the list of monitors from a specific OS-Dependant implementation
		// into a temporary mapo
		MonitorsFound lst;
		refreshOSSpecific(lst);
		// We will browse each monitor found to see if it is suitable for our needs
		// In this case, it will be added to the result list
		for (auto& it: lst)
		{
			OrderedResolutions& resolutions = *(it.second);
			// A monitor without any resolution is useless
			if (resolutions.empty()) // no available resolutions
				continue;
			// Keeping a reference to our monitor for code clarity
			auto& monitor = it.first;
			// Removing all its default resolutions
			monitor->clear();
			// Browsing all resolutions, in the reverse order
			// It is important since we must have the higher resolution at the
			// beginning
			{
				const OrderedResolutions::reverse_iterator& jEnd = resolutions.rend();
				for (OrderedResolutions::reverse_iterator j = resolutions.rbegin(); j != jEnd; ++j)
				{
					// j->first  : width
					// k->first  : height
					// k->second : color depth
					// Do not accept resolution with a width below minWidth
					if (j->first < minWidth)
						continue;
					for (auto k = j->second.rbegin(); k != j->second.rend(); ++k)
					{
						// Do not accept resolutions with a height below minHeight
						if (k->first < minHeight)
							continue;
						for (auto l = k->second.rbegin(); l != k->second.rend(); ++l)
						{
							if (l->first >= minDepth)
								*monitor += Resolution{j->first, k->first, l->first};
						}
					}
				}
			}
			if (not monitor->resolutions().empty()) // at least one resolution
			{
				pMonitors.push_back(monitor);
				if (monitor->primary())
					pPrimary = monitor;
			}
			// Hard limit
			if (pMonitors.size() == YUNI_DEVICE_MONITOR_COUNT_HARD_LIMIT)
				break;
		}
		// No available monitor/resolution
		// The list must not be empty
		if (pMonitors.empty())
		{
			pMonitors.push_back(pNullMonitor);
			return false;
		}
		return true;
	}


	Ref<Monitor> List::findByHandle(const Monitor::Handle hwn) const
	{
		for (auto& monitor: pMonitors)
		{
			if (hwn == monitor->handle())
				return monitor;
		}
		return pNullMonitor;
	}


	Ref<Monitor> List::findByGUID(const String& guid) const
	{
		for (auto& monitor: pMonitors)
		{
			if (guid == monitor->guid())
				return monitor;
		}
		return pNullMonitor;
	}


} // namespace Display
} // namespace Device
} // namespace Yuni
