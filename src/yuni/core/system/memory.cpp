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
// memory.h: The relative path is to avoid conflict with <memory.h>, which
// can sometimes occur...
#include "../system/memory.h"

// The define SYSTEM_MEMORY_IS_IMPLEMENTED will be used to know if
// a valid implementation is available.
// Otherwise we will use some fallbacks and generate a compiler warning


#ifdef YUNI_OS_WINDOWS
# include "windows.hdr.h"
#endif

#if defined(YUNI_OS_LINUX) || defined(YUNI_OS_CYGWIN)
# include <string.h>
# include <sys/sysinfo.h> // sysinfo (2)
#endif

#ifdef YUNI_OS_MAC
#include <sys/types.h>
#include <sys/sysctl.h>
#include <unistd.h>
#include <mach/mach.h>
#endif

#include <stdio.h>
#include <stdlib.h>




namespace Yuni
{
namespace System
{
namespace Memory
{


#ifdef YUNI_OS_WINDOWS
#define SYSTEM_MEMORY_IS_IMPLEMENTED

	uint64 Total()
	{
		// see http://msdn.microsoft.com/en-us/library/aa366589(VS.85).aspx
		MEMORYSTATUSEX statex;
		statex.dwLength = (DWORD) sizeof(statex);
		return (GlobalMemoryStatusEx(&statex)) ? (uint64)statex.ullTotalPhys
			: (uint64)defaultTotal;
	}


	uint64 Available()
	{
		// see http://msdn.microsoft.com/en-us/library/aa366589(VS.85).aspx
		MEMORYSTATUSEX statex;
		statex.dwLength = (DWORD) sizeof(statex);
		return (GlobalMemoryStatusEx(&statex)) ? (uint64)statex.ullAvailPhys
			: (uint64)defaultAvailable;
	}


	bool Usage::update()
	{
		// see http://msdn.microsoft.com/en-us/library/aa366589(VS.85).aspx
		MEMORYSTATUSEX statex;
		statex.dwLength = (DWORD) sizeof(statex);

		if (GlobalMemoryStatusEx(&statex))
		{
			total = (uint64)statex.ullTotalPhys;
			available = (uint64)statex.ullAvailPhys;
			return true;
		}
		available = defaultAvailable;
		total = defaultTotal;
		return false;
	}

#endif // YUNI_OS_WINDOWS






#if defined(YUNI_OS_LINUX) || defined(YUNI_OS_CYGWIN)
#define SYSTEM_MEMORY_IS_IMPLEMENTED

	/*!
	** \brief Read a line from a file
	*/
	static inline int fgetline(FILE* fp, char* s, int maxlen)
	{
		int i = 0;
		char c;

		while ((c = (char)fgetc(fp)) != EOF)
		{
			if (c == '\n')
			{
				*s = '\0';
				return i;
			}
			if (i >= maxlen)
				return i;

			*s++ = c;
			++i;
		}

		return (!i) ? EOF : i;
	}


	static uint64 readvalue(char* line)
	{
		// Here is a sample for /proc/meminfo :
		//
		// MemTotal:      1929228 kB
		// MemFree:         12732 kB
		// Buffers:         72176 kB
		// Cached:        1076572 kB
		// SwapCached:     151412 kB
		// Active:        1491184 kB
		// Inactive:       190832 kB
		// HighTotal:           0 kB
		// HighFree:            0 kB
		// LowTotal:      1929228 kB
		// LowFree:         12732 kB
		// SwapTotal:     2096472 kB
		// SwapFree:      1732964 kB
		// Dirty:             736 kB
		// Writeback:           0 kB
		// AnonPages:      512004 kB
		// Mapped:         702148 kB
		// Slab:           154320 kB
		// PageTables:      34712 kB
		// NFS_Unstable:        0 kB
		// Bounce:              0 kB
		// CommitLimit:   3061084 kB
		// Committed_AS:  1357596 kB
		// VmallocTotal: 34359738367 kB
		// VmallocUsed:    263492 kB
		// VmallocChunk: 34359474679 kB
		// HugePages_Total:     0
		// HugePages_Free:      0
		// HugePages_Rsvd:      0
		// Hugepagesize:     2048 kB

		// Trimming the string from the begining
		while (*line == ' ' and *line != '\0')
			++line;
		const char* first = line;

		// Looking for the end of the number
		while (*line != ' ' and *line != '\0')
			++line;
		// Tagging the end of the number
		*line = '\0';

		# ifdef YUNI_OS_32
		return (uint64) atol(first) * 1024u;
		# else
		return (uint64) atoll(first) * 1024u;
		# endif
	}


	bool Usage::update()
	{
		// The only good way to retrieve the memory usage is to read /proc/meminfo.
		// The routine sysinfo (2) is not enough since it does not take care of
		// the cache memory, returning a not valid amount of available physsical
		// memory for our uses.
		FILE* fd;
		if ((fd = fopen("/proc/meminfo", "r")))
		{
			// Resetting the amount of the total physical memory
			total = 0;
			// The amount of the available physical memory is the sum of 3 values :
			// MemFree, Cached and Buffers.
			available = 0;

			// A small buffer
			char line[90];
			// A counter to reduce to abort as soon as we have read all
			int remains = 8;

			// Analysing each line in /proc/meminfo, until the end-of-file or
			// until we have read the 4 values that interrest us.
			while (EOF != fgetline(fd, line, (uint)sizeof(line)))
			{
				if (!strncmp("MemTotal:", line, 9))
				{
					total = readvalue(line + 10);
					if (!(remains >> 1))
						break;
				}
				if (!strncmp("MemFree:", line, 8))
				{
					available += readvalue(line + 9);
					if (!(remains >> 1))
						break;
				}
				if (!strncmp("Cached:", line, 7))
				{
					available += readvalue(line + 8);
					if (!(remains >> 1))
						break;
				}
				if (!strncmp("Buffers:", line, 8))
				{
					available += readvalue(line + 9);
					if (!(remains >> 1))
						break;
				}
			}

			// Closing /proc/meminfo
			fclose(fd);

			// Checking the amount of the total physical memory, which can not be equal to 0
			if (!total)
			{
				total = (uint64) defaultTotal;
				return false;
			}
			return true;
		}

		// Error, using default values
		total     = (uint64) defaultTotal;
		available = (uint64) defaultAvailable;
		return false;
	}


	uint64 Available()
	{
		return Usage().available;
	}


	uint64 Total()
	{
#ifdef YUNI_OS_LINUX
		// Directly using sysinfo (2), which should be faster than parsing /proc/meminfo
		struct sysinfo s;
		return (!sysinfo(&s)) ? (s.mem_unit * s.totalram) : (uint64) defaultTotal;
#else
		return Usage().total;
#endif
	}

#endif // YUNI_OS_LINUX



#ifdef YUNI_OS_MAC
#define SYSTEM_MEMORY_IS_IMPLEMENTED

	uint64 Total()
	{
		int mib[2] = {CTL_HW, HW_MEMSIZE};
		uint64 memory;
		size_t len = sizeof(uint64);
		return (!sysctl(mib, 2, &memory, &len, NULL, 0)) ? memory : (uint64) defaultTotal;
	}


	uint64 Available()
	{
		// Good readings :)
		// http://www.booktou.com/node/148/0321278542/ch06lev1sec3.html

		// Host
		host_name_port_t host = mach_host_self();
		// Statistics about the VM
		vm_statistics_data_t vm_stat;
		mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
		host_statistics(host, HOST_VM_INFO, (host_info_t) &vm_stat, &count);
		// A page size
		vm_size_t  page_size;
		host_page_size(host, &page_size);

		return (uint64) (vm_stat.free_count) * (uint64) page_size;
	}


	bool Usage::update()
	{
		// Available
		available = Available();

		// Total
		int mib[2] = {CTL_HW, HW_MEMSIZE};
		size_t len = sizeof(uint64);
		size_t sttotal;
		if (sysctl(mib, 2, &sttotal, &len, NULL, 0))
		{
			total = (uint64) defaultTotal;
			return false;
		}
		total = (uint64) sttotal;
		return true;
	}

#endif // YUNI_OS_MAC








#ifndef SYSTEM_MEMORY_IS_IMPLEMENTED
#warning Yuni::System::Memory: The implementation is missing for this operating system

	uint64 Total()
	{
		return defaultTotal;
	}

	uint64 Available()
	{
		return defaultAvailable;
	}

	bool Usage::update()
	{
		available = defaultAvailable;
		total     = defaultTotal;
		return false;
	}

#endif // Fallback



} // namespace Memory
} // namespace System
} // namespace Yuni


