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
#ifndef YUNI_NO_THREAD_SAFE
#	define YUNI_NO_THREAD_SAFE // disabling thread-safety
#endif
#include <yuni/yuni.h>
#include <yuni/core/string.h>
#include <yuni/core/getopt.h>
#include "versions.h"



namespace Yuni
{

	class LibConfigProgram
	{
	public:
		LibConfigProgram();

		int execute(int argc, char** argv);

		bool debug() const {return pOptDebug;}

	private:
		void findRootPath(const char* a0);

		bool parseCommandLine(int argc, char** argv);

		void cleanPrefixPaths();

		void initializeSystemPathList();

		void normalizeCompiler();

		void expandModuleNames();

		bool displayInformations();

		void displayInformationAboutYuniVersion();

		void computeDependencies(LibConfig::VersionInfo::Settings& version);

		void printModulesDependencies() const;

		bool isCoreModule(const String& name) const;

		bool checkForDependencies(LibConfig::VersionInfo::Settings& version);

		bool displayInformationAboutYuniVersion(LibConfig::VersionInfo::Settings& version);

		void createArguments(LibConfig::VersionInfo::Settings& version) const;


	private:
		//! Exit status
		int pExitStatus;
		//! The Root path
		String pRootPath;

		//! Flag: Print the version and exit
		bool pOptVersion;
		//! Flag: Print all available versions of libyuni (with info) and exit
		bool pOptList;
		//! Flag: Print all available versions of libyuni and exit
		bool pOptListOnlyVersions;
		//! Flag: Do not use default paths
		bool pOptNoDefaultPath;
		//! Flag: Print all available modules of the selected versions
		bool pOptModuleList;
		//! Flag: Print all default paths and exit
		bool pOptDefaultPathList;
		//! Flag: Print cxx flags
		bool pOptCxxFlags;
		//! Flag: Print lib flags
		bool pOptLibFlags;
		//! Flag: Print the default compiler
		bool pOptPrintCompilerByDefault;
		//! Flag: Verbose
		bool pOptPrintErrors;
		//! Flag: Print all modules and exit
		bool pOptPrintModulesDeps;
		//! Flag: Debug
		bool pOptDebug;

		//! List of required modules
		String::List pOptModules;
		//! List of given prefix
		String::List pOptPrefix;
		//! List of default paths
		String::List pDefaultPathList;
		//! The complete list of known libyuni versions
		LibConfig::VersionInfo::List pVersionList;
		//! The compiler to use
		String pOptCompiler;

	}; // class Options





} // namespace Yuni

#include "program.hxx"
