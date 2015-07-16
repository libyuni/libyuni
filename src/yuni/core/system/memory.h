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



namespace Yuni
{
namespace System
{
namespace Memory
{

	/*!
	** \brief Get the amount of total physical memory
	**
	** When information about the memory usage can not be retrieved, a default
	** value will be used. (see defaultAvailable)
	** \return A value in bytes
	*/
	YUNI_DECL uint64 Total();


	/*!
	** \brief Get the amount of available physical memory
	**
	** When information about the memory usage can not be retrieved, a default
	** value will be used. (see defaultAvailable)
	** \return A value in bytes
	*/
	YUNI_DECL uint64 Available();



	/*!
	** \brief Information about the current memory usage
	**
	** When information about the current memory usage could not be
	** retrieved, some default values are used (see defaultTotal and defaultAvailable).
	** This is the case for example when the implementation is missing for the
	** current operating system. This method ensures that the values are always valid.
	**
	** Performance tip: If you're interrested in a single value, you should use
	**   the corresponding global routine instead, which will be at least as
	**   fast as this this class, sometimes better.
	*/
	YUNI_DECL class Usage final
	{
	public:
		//! \name Constructor
		//@{
		/*!
		** \brief Default constructor
		*/
		Usage();
		/*!
		** \brief Copy constructor
		**
		** The values will remain untouched.
		*/
		Usage(const Usage& copy);
		//@}

		/*!
		** \brief Update information about the current memory usage
		**
		** This routine is automatically called by the constructor.
		*/
		bool update();

	public:
		//! The amount of the available physical memory (in bytes)
		uint64 available;
		//! The amount of the total physical memory (in bytes)
		uint64 total;

	}; // class Usage






	/*!
	** \brief Constants to use when information about the memory usage could not
	**   be retrieved
	*/
	enum
	{
		//! The default amount of available physical memory
		defaultAvailable = 1024 * 1024 * 512,  // 512Mo
		//! The default amount of total physical memory
		defaultTotal     = 1024 * 1024 * 1024, // 1Go
	};




} // namespace Memory
} // namespace System
} // namespace Yuni

# include "memory.hxx"

