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
#include "../null.h"
#include "../../../io/file.h"
#include <cassert>



namespace Yuni
{
namespace Logs
{

	/*!
	** \brief Log Handler: Single Log file
	*/
	template<class NextHandler = NullHandler>
	class YUNI_DECL File : public NextHandler
	{
	public:
		enum Settings
		{
			// Colors are not allowed in a file
			colorsAllowed = 0,
		};

	public:
		/*!
		** \brief Try to (re)open a target log file
		**
		** You should use an absolute filename to be able to safely reopen it.
		** If a log file was already opened, it will be closed before anything else.
		** If the given filename is empty, true will be returned.
		**
		** \param filename A relative or absolute filename
		** \return True if the operation succeeded, false otherwise
		*/
		bool logfile(const AnyString& filename);

		/*!
		** \brief Get the last opened log file
		** \see outputFilename(filename)
		*/
		String logfile() const;

		/*!
		** \brief Reopen the log file
		**
		** It is safe to call several times this routine.
		** True will be returned if the log filename is empty.
		*/
		bool reopenLogfile();

		/*!
		** \brief Close the log file
		**
		** It is safe to call several times this routine.
		*/
		void closeLogfile();

		/*!
		** \brief Get if a log file is opened
		*/
		bool logfileIsOpened() const;


	public:
		template<class LoggerT, class VerbosityType>
		void internalDecoratorWriteWL(LoggerT& logger, const AnyString& s) const;

	private:
		//! The originale filename
		String pOutputFilename;
		//! File
		mutable IO::File::Stream pFile;

	}; // class File






} // namespace Logs
} // namespace Yuni

#include "file.hxx"

