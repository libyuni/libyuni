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
#include "../yuni.h"
#include "checkpoint.h"
#include <iostream>
#include <string.h>
#include <time.h>
#ifndef YUNI_OS_WINDOWS
#	include <sys/time.h>
#endif
#include "../core/system/gettimeofday.h"


#define RESULT_TO_STR(X) ((X) ? "ok" : "failed")


namespace Yuni
{
namespace Test
{

	namespace
	{

		uint GetCurrentTime()
		{
			timeval tv;
			gettimeofday(&tv, 0);
			return (uint)tv.tv_sec * 1000  + (uint)tv.tv_usec / 1000;
		}

	} // anonymous namespace



	Checkpoint::Checkpoint(const char* id)
		:pResult(true)
	{
		// ID
		# ifdef YUNI_OS_MSVC
		strncpy_s(pID, sizeof(pID), id, YUNI_TEST_ID_MAXLENGTH - 1);
		# else
		::strncpy(pID, id, YUNI_TEST_ID_MAXLENGTH - 1);
		# endif
		// Ensure it is zero-terminated (should not be necessary however)
		pID[YUNI_TEST_ID_MAXLENGTH - 1] = '\0';
		// Tag the outputs
		std::cout << "[test:cout] <begin> " << pID << "\n";
		std::cerr << "[test:cerr] <begin> " << pID << "\n";
	}


	Checkpoint::~Checkpoint()
	{
		// \internal At the end of each checkpoint we force the
		// flush for stdout and stderr
		std::cout << "[test:cout] <end," << RESULT_TO_STR(pResult) << "> " << pID << std::endl;
		std::cerr << "[test:cerr] <end," << RESULT_TO_STR(pResult) << "> " << pID << std::endl;
	}


	bool Checkpoint::operator () (bool b)
	{
		if (!b)
			pResult = false;
		std::cout << "[test:cout] <check," << RESULT_TO_STR(b) << ">\n";
		std::cerr << "[test:cerr] <check," << RESULT_TO_STR(b) << ">\n";
		return b;
	}


	bool Checkpoint::operator () (bool b, const char* msg)
	{
		if (!b)
			pResult = false;
		std::cout << "[test:cout] <check," << RESULT_TO_STR(b) << ">" << msg << "\n";
		std::cerr << "[test:cerr] <check," << RESULT_TO_STR(b) << ">" << msg << "\n";
		return b;
	}



	TimedCheckpoint::TimedCheckpoint(const char* id)
		:pResult(true)
	{
		// ID
		# ifdef YUNI_OS_MSVC
		strncpy_s(pID, sizeof(pID), id, YUNI_TEST_ID_MAXLENGTH - 1);
		# else
		::strncpy(pID, id, YUNI_TEST_ID_MAXLENGTH - 1);
		# endif
		// Ensure it is zero-terminated (should not be necessary however)
		pID[YUNI_TEST_ID_MAXLENGTH - 1] = '\0';
		// Tag the outputs
		std::cout << "[test:cout] <begin> " << pID << "\n";
		std::cerr << "[test:cerr] <begin> " << pID << "\n";

		// Start time
		pStartTime = GetCurrentTime();
	}


	TimedCheckpoint::~TimedCheckpoint()
	{
		// Duration
		uint duration = GetCurrentTime() - pStartTime;
		// \internal At the end of each checkpoint we force the
		// flush for stdout and stderr
		std::cout << "[test:cout] <end," << RESULT_TO_STR(pResult) << "," << duration << "ms> " << pID << std::endl;
		std::cerr << "[test:cerr] <end," << RESULT_TO_STR(pResult) << "," << duration << "ms> " << pID << std::endl;
	}


	bool TimedCheckpoint::operator () (bool b)
	{
		if (!b)
			pResult = false;
		std::cout << "[test:cout] <check," << RESULT_TO_STR(b) << ">\n";
		std::cerr << "[test:cerr] <check," << RESULT_TO_STR(b) << ">\n";
		return b;
	}


	bool TimedCheckpoint::operator () (bool b, const char* msg)
	{
		if (!b)
			pResult = false;
		std::cout << "[test:cout] <check," << RESULT_TO_STR(b) << ">" << msg << "\n";
		std::cerr << "[test:cerr] <check," << RESULT_TO_STR(b) << ">" << msg << "\n";
		return b;
	}




} // namespace Test
} // namespace Yuni
