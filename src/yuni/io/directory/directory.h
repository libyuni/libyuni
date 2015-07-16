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



namespace Yuni
{
namespace IO
{

/*
** \brief Directory manipulation functions
*/
namespace Directory
{

	//! \name Exists
	//@{
	/*!
	** \brief Test if a node exists and is actually a directory
	**
	** To test if a node exists whatever its nature, you should use
	** IO::Exists() instead.
	**
	** \param path The directory to test
	** \return True if it exists, false otherwise
	*/
	bool Exists(const AnyString& path);
	//@}


	/*!
	** \brief Routines about current directories
	*/
	namespace Current
	{
		/*!
		** \brief Get the current directory
		**
		** \param[out] out The variable where the current directory will be written
		** \param clearBefore True to clean \p out before
		** \return True if the operation succeeded. False otherwise
		*/
		bool Get(String& out, bool clearBefore = true);

		/*!
		** \brief Get the current directory (clob)
		**
		** \param[out] out The variable where the current directory will be written
		** \param clearBefore True to clean \p out before
		** \return True if the operation succeeded. False otherwise
		*/
		bool Get(Clob& out, bool clearBefore = true);

		/*!
		** \brief Get the current directory
		**
		** Performance Tip: Prefer Get(String&) which avoids several string copies
		** \return The current directory
		*/
		String Get();

		/*!
		** \brief Set the current directory
		**
		** \param path The new current directory
		** \return True if the operation succeeded
		*/
		bool Set(const AnyString& path);

	} // namespace Current




	//! \name Create a directory
	//@{
	/*!
	** \brief Create a directory recursively
	**
	** \param path The path to create if it does not exist
	** \param mode Access permissions (ignored on the MS Windows platforms)
	** \return True if the operation succeeded, false otherwise
	*/
	bool Create(const AnyString& path, unsigned int mode = 0755);
	//@}


	//! \name Remove a directory
	//@{
	/*!
	** \brief Recursively delete a directory and its content
	**
	** \param path The path to delete
	** \return True if the operation succeeded False otherwise
	*/
	bool Remove(const AnyString& path);
	//@}



	//! \name Copy a directory
	//@{
	enum CopyState
	{
		cpsGatheringInformation,
		cpsCopying
	};
	typedef Yuni::Bind<bool (CopyState, const String&, const String&, uint64, uint64)>  CopyOnUpdateBind;


	/*!
	** \brief Copy a directory
	**
	** \param source The source folder
	** \param destination The destination folder
	** \param recursive True to copy recursively
	** \param overwrite True to overwrite the files even if they already exist
	** \return True if the operation succeeded, false otherwise
	*/
	bool Copy(const AnyString& source, const AnyString& destination, bool recursive = true, bool overwrite = true);

	/*!
	** \brief Copy a directory
	**
	** \param source The source folder
	** \param destination The destination folder
	** \param onUpdate Event
	** \return True if the operation succeeded, false otherwise
	*/
	bool Copy(const AnyString& source, const AnyString& destination, const CopyOnUpdateBind& onUpdate);

	/*!
	** \brief Copy a directory
	**
	** \param source The source folder
	** \param destination The destination folder
	** \param recursive True to copy recursively
	** \param overwrite True to overwrite the files even if they already exist
	** \param onUpdate Event
	** \return True if the operation succeeded, false otherwise
	*/
	bool Copy(const AnyString& source, const AnyString& destination, bool recursive,
		bool overwrite, const CopyOnUpdateBind& onUpdate);
	//@}




} // namespace Directory
} // namespace IO
} // namespace Yuni

# include "../io.h"
# include "directory.hxx"

