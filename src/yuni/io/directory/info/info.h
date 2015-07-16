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
#include "../../../yuni.h"
#include "../../../core/string.h"
#include "../../io.h"
#include "../../directory.h"
#include "../../../core/iterator/iterator.h"
#include "platform.h"



namespace Yuni
{
namespace IO
{
namespace Directory
{

	/*!
	** \brief Convenient tool for creating, moving, and enumerating through directories and subdirectories
	**
	** Example for iterating through a directory :
	** \code
	** #include <yuni/yuni.h>
	** #include <yuni/io/directory/info.h>
	** #include <iostream>
	**
	** using namespace Yuni;
	**
	**
	** int main()
	** {
	**	IO::Directory::Info info{"/tmp"};
	**	for (IO::Directory::Info::iterator i = info.begin(); i != info.end(); ++i)
	**		std::cout << *i << std::endl;
	**
	**	return 0;
	** }
	** \endcode
	*/
	class Info final
	{
	public:
		//! \name Iterators
		//@{
		//! Options for performing an iteration
		enum OptionIterator
		{
			itNone       = 0,
			//! Include files
			itFile       = 1,
			//! Include folders
			itFolder     = 2,
			//! Perform a recursive iteration
			itRecursive  = 4,
			//! All options
			itAll        = itFile | itFolder | itRecursive,
		};

		// implementation
		# include "iterator.inc.hpp"

		// \internal We must specify the full namespace (`Yuni::...`) to avoid conflict
		//   with  Yuni::IO::Directory::IIterator.

		//! The default iterator, for iterating through all files and folders within a directory
		typedef Yuni::IIterator<Model::Iterator<itFile|itFolder>, true>  iterator;
		//! The default iterator, for iterating through all files within a directory
		typedef Yuni::IIterator<Model::Iterator<itFile>, true>    file_iterator;
		//! The default iterator, for iterating through all folders within a directory
		typedef Yuni::IIterator<Model::Iterator<itFolder>, true>  folder_iterator;
		//! The default iterator, for recursively iterating through all files and folders within a directory
		typedef Yuni::IIterator<Model::Iterator<itAll>, true>     recursive_iterator;
		//! The default iterator, for iterating through all files within a directory
		typedef Yuni::IIterator<Model::Iterator<itFile|itRecursive>, true>    recursive_file_iterator;
		//! The default iterator, for iterating through all folders within a directory
		typedef Yuni::IIterator<Model::Iterator<itFolder|itRecursive>, true>  recursive_folder_iterator;
		//! A generic null iterator
		typedef Yuni::IIterator<Model::NullIterator, true>  null_iterator;
		//@}


	public:
		//! \name Constructors & Destructor
		//@{
		/*!
		** \brief Default constructor
		*/
		Info() {}
		/*!
		** \brief Constructor
		** \param directory A directory
		*/
		explicit Info(const AnyString& directory);
		//@}

		/*!
		** \brief Get if the directory exists
		*/
		bool exists() const;

		/*!
		** \brief Normalize the directory path
		** \see Yuni::IO::Normalize()
		*/
		void normalize();

		/*!
		** \brief Normalize the directory path
		**
		** A temporary string is required to normalize the path. This
		** additional parameter allows the user to provide its own
		** temporary buffer to be reused when performing the operation.
		** This should reduce the overhead due to useless memory allocation.
		**
		** \tparam StringT Any container of type string (String, std::string...)
		*/
		template<class StringT> void normalize(StringT& tmp);


		//! \name Common operations
		//@{
		/*!
		** \brief Create the directory recursively
		**
		** \param mode Access permissions (ignored on the MS Windows platforms)
		** \return True if the operation succeeded, false otherwise
		** \see Yuni::IO::Directory::Create()
		*/
		bool create(uint mode = 0755) const;

		/*!
		** \brief Remove the directory and all its content
		**
		** \return True if the operation succeeded False otherwise
		** \see Yuni::IO::Directory::Remove()
		*/
		bool remove() const;

		/*!
		** \brief Clean the directory (remove all its content but keep the top folder)
		*/
		bool clean() const;

		/*!
		** \brief Copy the directory and its content into another location
		**
		** \param destination The destination folder
		** \return True if the operation succeeded False otherwise
		** \see Yuni::IO::Directory::Copy()
		*/
		bool copy(const AnyString& destination) const;

		/*!
		** \brief Set as the current directory
		**
		** \return True if the operation succeeded False otherwise
		** \see Yuni::IO::Directory::Current::Set()
		*/
		bool setAsCurrentDirectory() const;
		//@}


		//! \name Iterators
		//@{
		iterator begin() const;
		null_iterator end() const;

		file_iterator file_begin() const;
		null_iterator file_end() const;

		folder_iterator folder_begin() const;
		null_iterator folder_end() const;

		recursive_iterator recursive_begin() const;
		null_iterator recursive_end() const;

		recursive_file_iterator recursive_file_begin() const;
		null_iterator recursive_file_end() const;

		recursive_folder_iterator recursive_folder_begin() const;
		null_iterator recursive_folder_end() const;
		//@}


		/*!
		** \brief Get a reference to the directory string itself (const)
		*/
		const String& directory() const;

		/*!
		** \brief Get a reference to the directory string itself
		**
		** \note The directory can directly be modified by the user without any
		**   restriction.
		**   This can be useful to reduce memory allocations.
		*/
		String& directory();


		//! \name Operators
		//@{
		//! Assignement
		Info& operator = (const Info& rhs);
		/*!
		** \brief Assignment, for any types convertible into a string
		*/
		Info& operator = (const AnyString& rhs);

		//! Comparison
		bool operator == (const Info& rhs) const;
		//! Comparison
		template<class U> bool operator == (const U& rhs) const;

		template<class U> Info& operator += (const U& rhs);
		template<class U> Info& operator << (const U& rhs);
		//@}


	private:
		//! The directory
		String pDirectory;

	}; // class Info






} // namespace Directory
} // namespace IO
} // namespace Yuni

# include "info.hxx"

