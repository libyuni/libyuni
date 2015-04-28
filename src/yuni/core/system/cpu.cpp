
#include "cpu.h"

#if defined(YUNI_OS_DARWIN) || defined(YUNI_OS_FREEBSD) || defined(YUNI_OS_NETBSD) || defined(YUNI_OS_OPENBSD)
# include <sys/param.h>
# include <sys/sysctl.h>
#endif
#if defined(YUNI_OS_LINUX) || defined(YUNI_OS_CYGWIN)
# include <unistd.h>
#endif
#include "windows.hdr.h"




namespace Yuni
{
namespace System
{
namespace CPU
{


	#if defined(YUNI_OS_WINDOWS) || defined(YUNI_OS_CYGWIN)
	# define YUNI_CPU_COUNT_HAS_IMPLEMENTATION
	uint Count()
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		return si.dwNumberOfProcessors;
	}
	#endif


	#if defined(YUNI_OS_LINUX) || defined(YUNI_OS_DARWIN) || defined(YUNI_OS_FREEBSD) || defined(YUNI_OS_NETBSD) || defined(YUNI_OS_OPENBSD)
	# define YUNI_CPU_COUNT_HAS_IMPLEMENTATION
	uint Count()
	{
		// The number of processors online (capable of running processes)
		long cpus = sysconf(_SC_NPROCESSORS_ONLN);
		return (cpus < 1) ? 1 : (uint) cpus;
	}
	#endif




	#ifndef YUNI_CPU_COUNT_HAS_IMPLEMENTATION
	#  warning "The method Yuni::System::CPU::Count() has not been implemented for the current platform"
	uint Count()
	{
		return 1; // Default value
	}
	#endif





} // namespace CPU
} // namespace System
} // namespace Yuni

