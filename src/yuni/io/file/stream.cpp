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
#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

// We need to remove strict ansi compliance under MinGW
// otherwise _wfopen and _fileno are not defined...
// And we need to do it here, before the first inclusion of <stdio.h>
#ifdef __STRICT_ANSI__
# undef __STRICT_ANSI__
#endif

#include <string.h>
#include "stream.h"
#include "../../core/string.h"
#include "../../core/string/wstring.h"

#ifdef YUNI_OS_WINDOWS
# include "../../core/system/windows.hdr.h"
# include <io.h>
# include <stdio.h>    // _fileno
#else
# include <sys/file.h> // lock
#endif

#ifndef YUNI_OS_MSVC
# include <unistd.h>   // ftruncate
#endif

#if defined(YUNI_HAS_POSIX_FALLOCATE) || defined(YUNI_OS_MAC)
# include <fcntl.h>
#endif

#ifdef YUNI_OS_WINDOWS
# define FILENO(X)  _fileno(X)
#else
# define FILENO(X)  fileno(X)
#endif



namespace Yuni
{
namespace IO
{
namespace File
{


	# ifdef YUNI_OS_WINDOWS
	namespace // anonymous
	{

		static Stream::HandleType OpenFileOnWindows(const AnyString& filename, int mode)
		{
			Private::WString<> wfilenm(filename);
			if (wfilenm.empty())
				return NULL;

			FILE* f;
			# ifdef YUNI_OS_MSVC
			{
				if (0 != _wfopen_s(&f, wfilenm.c_str(), OpenMode::ToWCString(mode)))
					return NULL;
			}
			# else
			{
				f = _wfopen(wfilenm.c_str(), OpenMode::ToWCString(mode));
			}
			# endif
			return f;
		}

	} // anonymous namespace
	# endif




	Stream::Stream(const AnyString& filename, int mode) :
		pFd(NULL)
	{
		open(filename, mode);
	}



	bool Stream::open(const AnyString& filename, int mode)
	{
		// Close the file if already opened
		if (pFd)
			(void)::fclose(pFd);

		# ifdef YUNI_OS_WINDOWS
		pFd = OpenFileOnWindows(filename, mode);
		# else
		// It is mandatory to open file with the flag O_CLOEXEC to avoid race
		// conditions with fork
		// fopen should used O_CLOEXEC as one of the option. However, at the current
		// state, not all operating systems do that.
		// So we have to do it by ourselves with open and fdopen.
		/*int flag = O_CLOEXEC;
		if (0 != (mode & OpenMode::read) and 0 != (mode & OpenMode::write))
			flag |= O_RDWR;
		else if (0 != (mode & OpenMode::read))
			flag |= O_RDONLY;
		else if (0 != (mode & OpenMode::write))
			flag |= O_WRONLY;

		if (0 != (mode & OpenMode::truncate))
			flag |= O_TRUNC;
		else if (0 != (mode & OpenMode::append))
			flag |= O_APPEND;

		if (0 != (mode & ~OpenMode::read))
			flag |= O_CREAT;

		int fd = ::open(filename.c_str(), flag);
		if (fd < 0) // error
			pFd = nullptr;
		else
			pFd = ::fdopen(fd, OpenMode::ToCString(mode));*/
		pFd = ::fopen(filename.c_str(), OpenMode::ToCString(mode));
		# endif

		return (NULL != pFd);
	}



	bool Stream::close()
	{
		if (pFd)
		{
			if (0 == ::fclose(pFd))
			{
				pFd = NULL;
				return true;
			}
			return false;
		}
		return true;
	}


	bool Stream::seek(ssize_t offset, SeekOrigin origin)
	{
		switch (origin)
		{
			case seekOriginBegin:   return (0 == ::fseek(pFd, (long) offset, SEEK_SET));
			case seekOriginCurrent: return (0 == ::fseek(pFd, (long) offset, SEEK_CUR));
			case seekOriginEnd:     return (0 == ::fseek(pFd, (long) offset, SEEK_END));
		}
		return false;
	}



	bool Stream::lockShared()
	{
		# ifndef YUNI_OS_WINDOWS
		return pFd ? (0 == flock(FILENO(pFd), LOCK_SH)) : false;
		# else
		// warning The implementation is missing on Windows (#346)
		assert("Stream::lock: the implementation is missing on Windows, see ticket #346");
		return false;
		# endif
	}


	bool Stream::lockExclusive()
	{
		# ifndef YUNI_OS_WINDOWS
		return pFd ? (0 == flock(FILENO(pFd), LOCK_EX)) : false;
		# else
		// warning The implementation is missing on Windows (#346)
		assert("Stream::lock: the implementation is missing on Windows, see ticket #346");
		return false;
		# endif
	}


	void Stream::unlock()
	{
		# ifndef YUNI_OS_WINDOWS
		if (pFd)
			flock(FILENO(pFd), LOCK_UN);
		# else
		// warning The implementation is missing on Windows (#346)
		assert("Stream::lock: the implementation is missing on Windows, see ticket #346");
		# endif
	}


	uint64 Stream::read(char* buffer, uint64 size)
	{
		//TODO We should not read with block too large (especially on 32bits)
		// systems. It can make the process/thread interruptible  too long
		return (uint64) ::fread(buffer, 1, (size_t)size, pFd);
	}


	uint64 Stream::write(const char* buffer, uint64 size)
	{
		//TODO We should not write with block too large (especially on 32bits)
		// systems. It can make the process/thread interruptible  too long
		return (uint64) ::fwrite(buffer, 1, (size_t)size, pFd);
	}



	# if !defined(YUNI_HAS_POSIX_FALLOCATE) and !defined(YUNI_OS_MAC)

	static bool TruncateFileDefault(Stream& file, uint64 size)
	{
		// Default implementation

		// Getting the current end of file
		if (not file.seekFromEndOfFile(0))
			return false;
		ssize_t end = (ssize_t) file.tell();

		#	ifndef YUNI_OS_MSVC
		bool result = (0 == ::ftruncate(FILENO(file.nativeHandle()), (off_t) size));
		#	else
		bool result = (0 == _chsize_s(FILENO(file.nativeHandle()), (sint64) size));
		#	endif
		if (result)
		{
			// if the file was already bigger than the new size, there is nothing to do
			if ((uint64)end >= size)
				return true;

			if (not file.seekFromBeginning(end))
				return false;

			enum
			{
				bufferSize = 1024 * 1024
			};
			size -= (uint64) end;

			if (size)
			{
				char* zero = new char[bufferSize];
				(void)::memset(zero, '\0', sizeof(char) * bufferSize);

				while (size > bufferSize)
				{
					if (bufferSize != file.write(zero, bufferSize))
					{
						delete[] zero;
						return false;
					}
					size -= bufferSize;
				}

				if (size)
				{
					if (size != file.write(zero, size))
					{
						delete[] zero;
						return false;
					}
				}
				delete[] zero;
			}
			return true;
		}
		return false;
	}

	# endif



	bool Stream::truncate(uint64 size, bool ensureAllocation)
	{
		if (pFd)
		{
			int fd = FILENO(pFd);

			if (not ensureAllocation)
			{
				# ifndef YUNI_OS_MSVC
				return (0 == ::ftruncate(fd, (off_t) size));
				# else
				return (0 == _chsize_s(fd, (sint64) size));
				# endif
			}
			else
			{
				# ifdef YUNI_HAS_POSIX_FALLOCATE
				return (0 == posix_fallocate(fd, 0, (off_t) size));
				# else

				# ifdef YUNI_OS_MAC
				// On OS X, we can use fcntl(F_PREALLOCATE) to emulate posix_fallocate
				// (but ftruncate must be called anyway)
				fstore_t store;
				memset(&store, 0, sizeof(store));
				store.fst_flags    = F_ALLOCATECONTIG;
				store.fst_posmode  = F_PEOFPOSMODE;
				store.fst_length   = (off_t) size;

				if (-1 == fcntl(fd, F_PREALLOCATE, &store))
				{
					// OK, perhaps we are too fragmented, allocate non-continuous
					store.fst_flags = F_ALLOCATEALL;
					if (-1 == fcntl(fd, F_PREALLOCATE, &store))
						return false;
				}
				return (0 == ::ftruncate(fd, (off_t) size));

				# else

				return TruncateFileDefault(*this, size);

				# endif // OS X
				# endif // POSIX_FALLOCATE
			}
		}
		return false;
	}




} // namespace File
} // namespace IO
} // namespace Yuni

