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
#include "../system/windows.hdr.h"
#include "file.h"
#ifndef YUNI_OS_WINDOWS
# include <dlfcn.h>
#else
# include "../../core/string/wstring.h"
#endif
#include <stdio.h>
#include "../../io/file.h"
#include "../../io/directory.h"


#ifdef YUNI_OS_WINDOWS
# define YUNI_DYNLIB_DLOPEN(X)      ::LoadLibraryW(X)
#else
# define YUNI_DYNLIB_DLOPEN(X,M)    ::dlopen(X,M)
#endif

#ifdef YUNI_OS_WINDOWS
# define YUNI_DYNLIB_DLCLOSE(X)     ::FreeLibrary(X)
#else
# define YUNI_DYNLIB_DLCLOSE(X)     ::dlclose(X)
#endif

#ifdef YUNI_OS_WINDOWS
# define YUNI_DYNLIB_DLSYM(X, Y)    ::GetProcAddress(X,Y)
#else
# define YUNI_DYNLIB_DLSYM(X, Y)    ::dlsym(X,Y)
#endif




namespace Yuni
{
namespace DynamicLibrary
{


	// Implementation of the static variable
	const File::Handle File::NullHandle = nullptr;


	namespace // anonymous
	{

		/*!
		** \brief Try to find a file from a single path, a filename and a prefix
		**
		** \param[out] out A temporary string, where to write the absolute filename
		** \param prefix The prefix to use for the filename
		** \return True if the filename in `s` exists and should be loaded, False otherwise
		**/
		template<class StringT>
		static inline bool FindLibraryFile(StringT& out, /*const StringT2& path,*/ const AnyString& filename, const char* prefix)
		{
			# define TEST_THEN_LOAD(EXT) \
				out.clear(); \
				/*if (!path.empty()) */ \
					/*out << path << IO::Separator; */ \
				out << prefix << filename << EXT; \
				if (IO::File::Exists(out)) \
					return true

			# ifdef YUNI_OS_DARWIN
			TEST_THEN_LOAD(".dylib");
			TEST_THEN_LOAD(".bundle");
			# endif

			# ifdef YUNI_OS_AIX
			TEST_THEN_LOAD(".a");
			# endif
			# ifdef YUNI_OS_HPUX
			TEST_THEN_LOAD(".sl");
			# endif

			# ifdef YUNI_OS_WINDOWS
			TEST_THEN_LOAD(".dll");
			# else
			TEST_THEN_LOAD(".so");
			# endif

			return false;
			# undef TEST_THEN_LOAD
		}


		/*!
		** \brief Try to find a file from a list of paths, a filename and a prefix
		**
		** \return True if the filename in `s` exists and should be loaded, False otherwise
		**/
		static inline bool FindLibrary(String& out, const AnyString& filename)
		{
			return (FindLibraryFile(out, filename, "lib") or FindLibraryFile(out, filename, ""));
		}


	} // anonymous namespace




	File::File(const AnyString& filename, Relocation relocation, Visibility visibility) :
		pHandle(NullHandle)
	{
		(void) loadFromFile(filename, relocation, visibility);
	}


	File::File(const AnyString& filename) :
		pHandle(NullHandle)
	{
		(void) loadFromFile(filename, relocationLazy, visibilityDefault);
	}


	File::File() :
		pHandle(NullHandle)
	{}


	File::~File()
	{
		if (NullHandle != pHandle)
			YUNI_DYNLIB_DLCLOSE(pHandle);
	}


	bool File::loadFromFile(const AnyString& filename, File::Relocation r, File::Visibility v)
	{
		// No filename
		if (not filename.empty())
		{
			// If the file name is absolute, there is no need for research
			if (IO::IsAbsolute(filename))
				return loadFromRawFilename(filename, r, v);

			// A temporary string, where to write the absolute filename
			String s;
			s.reserve(512);

			// Search paths
			// TODO : find a far more efficient way for doing this
			if (FindLibrary(s, filename))
				return loadFromRawFilename(s, r, v);
		}

		// Make sure the library has been unloaded
		// This unloading would have been done by `loadFromRawFilename()` if
		// something was found
		unload();
		// We have found nothing :(
		return false;
	}


	void File::unload()
	{
		if (loaded())
		{
			YUNI_DYNLIB_DLCLOSE(pHandle);
			pHandle = NullHandle;
			pFilename.clear();
			pFilename.shrink();
		}
	}






	# ifdef YUNI_OS_WINDOWS

	// Specific implementation for the Windows platform
	bool File::loadFromRawFilename(const AnyString& filename, File::Relocation, File::Visibility)
	{
		// Unload the library if already loaded
		unload();

		if (not filename.empty())
		{
			// Loading
			Private::WString<true> buffer(filename);
			if (buffer.empty())
				return false;

			pHandle = YUNI_DYNLIB_DLOPEN(buffer.c_str());
			if (NullHandle != pHandle)
			{
				pFilename = filename;
				return true;
			}
		}
		return false;
	}

	# else

	// Specific implementation for the Unix platforms
	bool File::loadFromRawFilename(const AnyString& filename, File::Relocation r, File::Visibility v)
	{
		// Unload the library if already loaded
		unload();

		if (not filename.empty())
		{
			// The mode
			int mode = ((relocationLazy == r) ? RTLD_LAZY : RTLD_NOW);
			if (visibilityDefault != v)
				mode |= ((visibilityGlobal == v) ? RTLD_GLOBAL : RTLD_LOCAL);

			// Loading
			pHandle = YUNI_DYNLIB_DLOPEN(filename.c_str(), mode);
			if (NullHandle != pHandle)
			{
				pFilename = filename;
				return true;
			}
		}
		return false;
	}

	# endif



	bool File::hasSymbol(const AnyString& name) const
	{
		return NullHandle != pHandle
			and NULL != (Symbol::Handle) (YUNI_DYNLIB_DLSYM(pHandle, name.c_str()));
	}


	Symbol File::resolve(const AnyString& name) const
	{
		return NullHandle != pHandle
			? (Symbol::Handle) (YUNI_DYNLIB_DLSYM(pHandle, name.c_str()))
			: nullptr;
	}




} // namespace DynamicLibrary
} // namespace Yuni

