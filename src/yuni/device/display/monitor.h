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
#ifndef __YUNI_DEVICE_DISPLAY_MONITOR_H__
# define __YUNI_DEVICE_DISPLAY_MONITOR_H__

# include "../../yuni.h"
# include "resolution.h"
# include "../../core/string.h"
# include "../../core/smartptr/smartptr.h"


# if defined(YUNI_OS_MACOSX)
#	define YUNI_DEVICE_DISPLAY_HANDLE         uint32
#	define YUNI_DEVICE_DISPLAY_INVALIDHANDLE  (uint32(-1))
# else
#	if defined(YUNI_OS_WINDOWS)
#		define YUNI_DEVICE_DISPLAY_HANDLE         wchar_t*
#		define YUNI_DEVICE_DISPLAY_INVALIDHANDLE  0
#	else
/* Unixes - X11 */
#		define YUNI_DEVICE_DISPLAY_HANDLE         int
#		define YUNI_DEVICE_DISPLAY_INVALIDHANDLE  (int(-1))
#	endif
# endif



namespace Yuni
{
namespace Device
{
namespace Display
{


	/*!
	** \brief Information about a single Frame Buffer Device
	**
	** This class is not thread-safe
	*/
	class Monitor
	{
	public:
		//! The most suitable smart pointer to use with the class `Monitor`
		typedef SmartPtr<Monitor> Ptr;

		/*!
		** \brief Handle for a single monitor
		**
		** This handle is system-dependant
		** <ul>
		**    <li>Mac OS : The equivalent is `CGDirectDisplayID`</li>
		** </ul>
		*/
		typedef YUNI_DEVICE_DISPLAY_HANDLE  Handle;
		//! Value for an invalid handle
		static const Handle InvalidHandle;


	public:
		//! \name Constructors and Destructor
		//@{
		/*!
		** \brief Default constructor
		**
		** By default, some safe (assumed) resolutions will be available
		*/
		Monitor();

		/*!
		** \brief Constructor with values
		**
		** By default, some safe (assumed) resolutions will be available
		**
		** \param nm Name of the monitor
		** \param hwn Handle of the monitor
		** \param p Is the primary display
		** \param a Is hardware-accelerated
		** \param b Is a builtin device
		*/
		Monitor(const String& nm, const Monitor::Handle hwn = InvalidHandle, bool p = false,
			bool a = false, bool b = false);

		//! Constructor by copy
		Monitor(const Monitor& copy);

		//! Destructor
		~Monitor();
		//@}


		//! \name Handle
		//@{
		/*!
		** \brief Get the index of the current selected monitor
		*/
		Monitor::Handle handle() const;

		/*!
		** \brief Get if the handle of this monitor is valid
		*/
		bool valid() const;
		//@}


		//! \name Informations about the monitor
		//@{
		/*!
		** \brief Get the human readable name of the monitor, if any
		**
		** \return The name of the monitor (product), or an empty value if not available
		*/
		const String& productName() const;

		/*!
		** \brief Get an unique id for this screen
		**
		** This ID should be considered as unique only on the local computer.
		** This guid may be useful to load/save settings according the current monitor
		**
		** \return A md5 string
		*/
		const String& guid() const;

		/*!
		** \brief Get if this monitor is the primary display
		*/
		bool primary() const;

		/*!
		** \brief Get if the device is hardware accelerated
		*/
		bool hardwareAcceleration() const;

		/*!
		** \brief Get if the monitor is a builtin device
		**
		** \note Only available on Mac OS X v10.2 or later
		*/
		bool builtin() const;
		//@}


		//! \name Monitor Resolutions
		//@{
		/*!
		** \brief Get all available resolutions for this screen
		**
		** The returned value is guaranteed to not be empty and to be
		** a sorted descendant list.
		*/
		const Resolution::Vector& resolutions() const;

		/*!
		** \brief Get the recommended resolution for this device
		**
		** It is merely the highest available resolution
		** \return A valid and not null resolution
		*/
		Resolution::Ptr recommendedResolution() const;

		/*!
		** \brief Get if a resolution is valid for this monitor
		**
		** \param rhs The resolution to check
		** \return True if the resolution is valid, merely if this resolution is in the list
		*/
		bool resolutionIsValid(const Resolution::Ptr& rhs) const;

		/*!
		** \brief Remove all resolutions
		*/
		void clear();

		/*!
		** \brief Add a new resolution in the list
		**
		** \param[in] resolution The resolution to add
		** \internal It is a sorted descendant list. The first value must be the highest available value
		*/
		void add(const Resolution::Ptr& resolution);

		/*!
		** \brief Add some safe resolutions
		**
		** Add the standard (and assumed safe) resolution 1024x768 since it should be supported by any
		** recent monitor and video card.
		*/
		void addSafeResolutions();
		//@}


		//! \name Operators
		//@{
		//! Append a resolution
		Monitor& operator += (Resolution* rhs);
		//! Append a resolution
		Monitor& operator += (const Resolution::Ptr& rhs);
		//! Append a resolution
		Monitor& operator << (Resolution* rhs);
		//! Append a resolution
		Monitor& operator << (const Resolution::Ptr& rhs);
		//@}


	protected:
		//! The index of the monitor
		Monitor::Handle pHandle;
		//! Name of the current monitor
		String pProductName;
		/*!
		** \brief All resolutions
		** \internal It is a sorted descendant list. The first value must be the highest available value
		*/
		Resolution::Vector pResolutions;
		//! Primary
		bool pPrimary;
		//! Hardware Acceleration
		bool pHardwareAcceleration;
		//! Builtin device
		bool pBuiltin;

		//! The MD5 for the guid - avoid multiple md5 calculations
		// mutable: to allow guid() const
		mutable String pMD5Cache;

		# ifdef YUNI_OS_WINDOWS
		wchar_t pDeviceID[128];
		# endif

	}; // class Monitor





} // namespace Display
} // namespace Device
} // namespace Yuni

# include "monitor.hxx"

#endif // __YUNI_DEVICE_DISPLAY_MONITOR_H__
