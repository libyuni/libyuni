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
		pResolutions.push_back(new Resolution(1024, 768, 32));
		pResolutions.push_back(new Resolution(800, 600, 32));
		pResolutions.push_back(new Resolution(640, 480, 32));
	}


	Monitor::Monitor() :
		pHandle(Monitor::InvalidHandle),
		pPrimary(false),
		pHardwareAcceleration(false),
		pBuiltin(false)
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


	Monitor::Monitor(const Monitor& copy) :
		pHandle(copy.pHandle),
		pProductName(copy.pProductName),
		pResolutions(copy.pResolutions),
		pPrimary(copy.pPrimary),
		pHardwareAcceleration(copy.pHardwareAcceleration),
		pBuiltin(copy.pBuiltin)
	{}


	Monitor::~Monitor()
	{
		pResolutions.clear();
	}


	Resolution::Ptr Monitor::recommendedResolution() const
	{
		// we assume that the first resolution is the highest available
		return (!pResolutions.empty())
			? *(pResolutions.begin())
			: new Resolution(640, 480);
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
				bld << (*pResolutions.begin())->toString();
			bld << '|' << pPrimary << '|' << pBuiltin << '|' << pHardwareAcceleration;

			Hash::Checksum::MD5 md5;
			pMD5Cache = md5.fromString(bld);
		}
		return pMD5Cache;
	}


	void Monitor::add(const Resolution::Ptr& resolution)
	{
		pResolutions.push_back(resolution);
	}


	bool Monitor::resolutionIsValid(const Resolution::Ptr& rhs) const
	{
		if (!(!rhs)) // The pointer must be valid
		{
			// Browse all available resolutions
			// The lookup should be done in the usual way since it is a sorted descendant list
			Resolution::Vector::const_iterator end = pResolutions.end();
			for (Resolution::Vector::const_iterator it = pResolutions.begin(); it != end; ++it)
			{
				if (*(*it) == *rhs)
					return true;
			}
		}
		return false;
	}


	Monitor& Monitor::operator += (Resolution* rhs)
	{
		pResolutions.push_back(rhs);
		return *this;
	}


	Monitor& Monitor::operator += (const Resolution::Ptr& rhs)
	{
		pResolutions.push_back(rhs);
		return *this;
	}


	Monitor& Monitor::operator << (Resolution* rhs)
	{
		pResolutions.push_back(rhs);
		return *this;
	}


	Monitor& Monitor::operator << (const Resolution::Ptr& rhs)
	{
		pResolutions.push_back(rhs);
		return *this;
	}





} // namespace Display
} // namespace Device
} // namespace Yuni

