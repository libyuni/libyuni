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
#include "../../yuni.h"
#include "../../core/string.h"
#include "../../core/bind.h"
#include <map>
#ifdef SearchPath
#	undef SearchPath // On Windows, SearchPath is a macro...
#endif


namespace Yuni
{
namespace IO
{

	/*!
	** \brief Find files or folders from one or several search paths
	**
	** \code
	** IO::SearchPath searchpath;
	** searchpath.directories.push_back(".");
	** searchpath.extensions.push_back(".dylib");
	** searchpath.extensions.push_back(".a");
	** searchpath.prefixes.push_back("");
	**
	** const String& location = searchpath.find("mylib");
	** if (!location)
	**	std::cerr << "impossible to find something\n";
	** else
	**	std::cout << "found: " << location << std::endl;
	**
	** // finding all matches
	** String::Vector list;
	** if (search.find(list, "mylib"))
	**	std::cerr << "impossible to find something\n";
	** else
	**	std::cout << "found: " << list << std::endl;
	** \endcode
	**
	** \note The cache lookup is disabled by default
	*/
	class YUNI_DECL SearchPath final
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default constructor
		*/
		SearchPath();
		//! Destructor
		~SearchPath();
		//@}


		//! \name Lookup
		//@{
		/*!
		** \brief Try to find the first filename/folder in the search paths which mathes the criteria
		** \return An absolute filename, or empty if not found
		*/
		bool find(String& out, const AnyString& filename) const;

		/*!
		** \brief Find all matching filenames/folders in the search paths which math the criteria
		*/
		bool find(String::Vector& out, const AnyString& filename) const;

		/*!
		** \brief Find all matching filenames/folders in the search paths which math the criteria
		*/
		bool find(String::List& out, const AnyString& filename) const;

		/*!
		** \brief Iterate through all files and folders
		**
		** \code
		** void print(const String& name, const String& filename)
		** {
		**	std::cout << name << " -> " << filename << std::endl;
		** }
		**
		** int main()
		** {
		**	IO::SearchPath searchpath;
		**	searchpath.directories.push_back("/lib");
		**	searchpath.extensions.push_back(".so");
		**	searchpath.each(&print);
		** \endcode
		*/
		void each(const Bind<void (const String&, const String&)>& callback,
			bool recursive = false, bool catchfolder = false) const;
		//@}


		//! \name Cache
		//@{
		//! Get if the cache is used
		bool cacheLookup() const;
		//! Set if the cache should be used
		void cacheLookup(bool enabled);

		/*!
		** \brief Clear the cache
		*/
		void cacheClear();
		//@}


	public:
		//! List of directories where to search of
		String::Vector  directories;
		//! List of extensions
		String::Vector  extensions;
		//! List of prefixes
		String::Vector  prefixes;


	private:
		typedef std::map<String, String> CacheStore;
		//! Should we use the cache ?
		bool pCacheLookup;
		//! Cache store
		CacheStore pCache;
		//! Temporary string used for cache lookup
		// (to reduce memory allocation / deallocation)
		mutable String pCacheQuery;

	}; // class SearchPath






} // namespace IO
} // namespace Yuni

#include "searchpath.hxx"

