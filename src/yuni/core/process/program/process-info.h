/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** github: https://github.com/libyuni/libyuni/
** gitlab: https://gitlab.com/libyuni/libyuni/ (mirror)
*/
#pragma once
#include "program.h"
#include "../../noncopyable.h"
#include "../../../thread/thread.h"
#include <vector>
#include <memory>




namespace Yuni
{
namespace Process
{

	/*!
	** \brief Helper class for storing shared information on the program which is currently
	** launched by Process::Program
	**
	** \note This class may be shared by several threads
	*/
	struct Program::ProcessSharedInfo final
		: public Yuni::NonCopyable<Program::ProcessSharedInfo>
	{
		~ProcessSharedInfo();

		/*!
		** \return True if the signal has been delivered
		*/
		bool sendSignal(bool withLock, int value);

		/*!
		** \brief Create a thread dedicated to handle the execution timeout
		*/
		void createThreadForTimeoutWL();


	public:
		String executable;
		//! The command
		std::vector<String> arguments;
		//! The working directory
		String workingDirectory;
		//! Flag to know if the process is running
		bool running = false;
		//! PID
		int processID = -1;
		//! input file descriptors
		int processInput = -1;
		//! Thread
		Yuni::Process::Program::ThreadMonitor* thread = nullptr;
		//! Duration in seconds
		sint64 duration = 0;
		//! Duration precision
		DurationPrecision durationPrecision = dpSeconds;
		//! Timeout
		uint timeout = 0;
		//! Exit status
		int exitstatus = -1;
		//! Console
		bool redirectToConsole = false;
		//! Mutex
		mutable Mutex mutex;
		//! Optional thread for timeout
		std::unique_ptr<Yuni::Thread::IThread> timeoutThread;

	}; // class Program::ProcessSharedInfo


} // namespace Process
} // namespace Yuni
