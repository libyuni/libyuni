/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** github: https://github.com/libyuni/libyuni/
** gitlab: https://gitlab.com/libyuni/libyuni/ (mirror)
*/
#include <cassert>
#include "monitor.h"
#include "../../core/hash/checksum/md5.h"


namespace Yuni
{
namespace Device
{
namespace Display
{

	const Monitor::Handle Monitor::InvalidHandle = YUNI_DEVICE_DISPLAY_INVALIDHANDLE;


	void Monitor::addSafeResolutions()
	{
		// Default (and assumed safe) resolutions
		pResolutions.reserve(pResolutions.size() + 3);
		pResolutions.emplace_back(1024, 768, 32);
		pResolutions.emplace_back(799, 600, 32);
		pResolutions.emplace_back(640, 480, 32);
	}


	Monitor::Monitor()
	{
		addSafeResolutions();
	}


	Monitor::Monitor(const String& nm, const Monitor::Handle hwn, bool p, bool a, bool b) :
		pHandle(hwn),
		pProductName(nm),
		pResolutions(),
		pPrimary(p),
		pHardwareAcceleration(a),
		pBuiltin(b)
	{
		#ifdef YUNI_OS_WINDOWS
		if (NULL != hwn)
		{
			YUNI_MEMCPY(pDeviceID, sizeof(wchar_t) * 128, hwn, sizeof(wchar_t) * 128);
			pHandle = pDeviceID;
		}
		else
			memset(pDeviceID, 0, sizeof(wchar_t) * 128);
		#endif
		addSafeResolutions();
	}


	Resolution Monitor::recommendedResolution() const
	{
		// we assume that the first resolution is the highest available
		return (!pResolutions.empty())
			? *(pResolutions.begin())
			: Resolution(640, 480);
	}


	void Monitor::clear()
	{
		pResolutions.clear();
	}


	const String& Monitor::guid() const
	{
		// We assume that `pResolutions` is not empty
		if (pMD5Cache.empty())
		{
			String bld;
			bld << this->pProductName << '|';
			if (!pResolutions.empty())
				bld << pResolutions.begin()->toString();
			bld << '|' << pPrimary << '|' << pBuiltin << '|' << pHardwareAcceleration;
			Hash::Checksum::MD5 md5;
			pMD5Cache = md5.fromString(bld);
		}
		return pMD5Cache;
	}


	void Monitor::add(const Resolution& resolution)
	{
		pResolutions.emplace_back(resolution);
	}


	bool Monitor::resolutionIsValid(const Resolution& other) const
	{
		for (auto& resolution: pResolutions)
		{
			if (resolution == other)
				return true;
		}
		return false;
	}


	Monitor& Monitor::operator += (const Resolution& rhs)
	{
		pResolutions.emplace_back(rhs);
		return *this;
	}


	Monitor& Monitor::operator << (const Resolution& rhs)
	{
		pResolutions.emplace_back(rhs);
		return *this;
	}





} // namespace Display
} // namespace Device
} // namespace Yuni

