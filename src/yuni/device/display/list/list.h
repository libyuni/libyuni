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
#ifndef __YUNI_DEVICE_DISPLAY_LIST_LIST_H__
# define __YUNI_DEVICE_DISPLAY_LIST_LIST_H__

# include "../../../yuni.h"
# include "../../../core/string.h"
# include "../resolution.h"
# include "../monitor.h"


//! Name to use for the fail-safe device
# define YUNI_DEVICE_DISPLAY_LIST_FAIL_SAFE_NAME   "Fail-safe Device"



namespace Yuni
{
namespace Device
{
namespace Display
{

	/*!
	** \brief Informations about all available display
	**
	** The best way to enumerate all available resolutions of all monitors :
	** \code
	** int main()
	** {
	**	typedef Device::Display::List MonitorIterator;
	**	Device::Display::List monitors;
	**	monitors.refresh();
	**	for (MonitorIterator i = monitors.begin(); i != monitors.end(); ++i)
	**	{
	**		std::cout << " * Monitor : " << (*i)->name() << std::endl;
	**		// All resolutions for this monitor
	**		for (Display::Resolution::Vector::iterator r = (*i)->resolutions().begin();
	**			r != (*i)->resolutions().end(); ++r)
	**		{
	**			std::cout << "    . " << *r << std::endl;
	**		}
	**	}
	** }
	** \endcode
	** \warning This class is not thread-safe
	*/
	class List
	{
	private:
		//! Vector of Monitor
		typedef std::vector<Monitor::Ptr> MonitorVector;

	public:
		//! An interator
		typedef MonitorVector::iterator iterator;
		//! A const iterator
		typedef MonitorVector::const_iterator const_iterator;

	public:
		//! \name Constructors &* Destructor
		//@{
		//! Default constructor
		List();
		//! Copy constructor (the copy will share the same informations)
		List(const List& c);
		//! Destructor
		~List() {}
		//@}

		//! \name Refresh
		//@{
		/*!
		** \brief Refresh informations about the monitors
		**
		** In any cases, this method ensures to provide a valid primary display,
		** and an not empty list even if monitors could not be found. (it would
		** provide a default monitor with default resolutions in this case)
		**
		** \param minWidth The minimum acceptable width
		** \param minHeight The minimum acceptable height
		** \param minDepth The minimum acceptable depth
		** \return True if the operation succeeded
		*/
		bool refresh(uint32 minWidth = 640, uint32 minHeight = 480, uint8 minDepth = 8);
		//@}

		//! \name Primary display
		//@{
		/*!
		** \brief Get the primary display
		**
		** The result is guaranteed to be valid for use.
		*/
		Monitor::Ptr primary() const;
		//@}

		//! \name Searching
		//@{
		//! Get an iterator at the beginning of the list
		iterator begin();
		//! Get an iterator at the beginning of the list
		const_iterator begin() const;

		//! Get an iterator at the end of the list
		iterator end();
		//! Get an iterator at the end of the list
		const_iterator end() const;

		/*!
		** \brief Find a monitor by its handle
		**
		** \return A reference to a monitor. Use the method `valid()` to know
		** if the monitor has been found
		**
		** \param hwn The handle to find
		**
		** \see Monitor::valid()
		*/
		Monitor::Ptr findByHandle(const Monitor::Handle hwn) const;

		/*!
		** \brief Find a monitor by its guid
		**
		** \return A reference to a monitor. Use the method `valid()` to know
		** if the monitor has been found
		**
		** \param guid The guid to find
		**
		** \see Monitor::valid()
		*/
		Monitor::Ptr findByGUID(const String& guid) const;
		//@}


		//! Get the number of monitors
		size_t size() const;

		//! \name Operators
		//@{
		//! Operator `=` (the copy will share the same information)
		List& operator = (const List& rhs);
		//@}

	private:
		//! All available monitors
		MonitorVector  pMonitors;
		//! The primary display
		Monitor::Ptr pPrimary;
		//! A null monitor
		Monitor::Ptr pNullMonitor;

	}; // class List





} // namespace Display
} // namespace Device
} // namespace Yuni


/*!
** \brief The hard limit for monitor count
**
** \internal This define is especially usefull where a limited is required
** by the operating system
*/
# define YUNI_DEVICE_MONITOR_COUNT_HARD_LIMIT   64


# include "list.hxx"

#endif // __YUNI_DEVICE_DISPLAY_LIST_LIST_H__
