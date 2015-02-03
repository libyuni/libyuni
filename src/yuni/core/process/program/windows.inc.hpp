
// file included by program.cpp

#ifdef YUNI_OS_WINDOWS

// http://msdn.microsoft.com/en-us/library/ms682499%28v=vs.85%29.aspx
// http://stackoverflow.com/questions/53208/how-do-i-automatically-destroy-child-processes-in-windows
// http://stackoverflow.com/questions/985281/what-is-the-closest-thing-windows-has-to-fork/985525#985525



namespace Yuni
{
namespace Process
{

	namespace // anonymous
	{


		static bool HaveLoadedFunctionsForFork(void)
		{
			HANDLE ntdll = ::GetModuleHandle("ntdll");
			if (ntdll == nullptr)
				return false;

			if (ZwCreateProcess && ZwQuerySystemInformation && ZwQueryVirtualMemory &&
				ZwCreateThread && ZwGetContextThread && ZwResumeThread &&
				ZwQueryInformationThread && ZwWriteVirtualMemory && ZwClose)
			{
				return true;
			}

			ZwCreateProcess = (ZwCreateProcess_t)::GetProcAddress(ntdll, "ZwCreateProcess");
			ZwQuerySystemInformation = (ZwQuerySystemInformation_t)::GetProcAddress(ntdll, "ZwQuerySystemInformation");
			ZwQueryVirtualMemory = (ZwQueryVirtualMemory_t)::GetProcAddress(ntdll, "ZwQueryVirtualMemory");
			ZwCreateThread = (ZwCreateThread_t)::GetProcAddress(ntdll, "ZwCreateThread");
			ZwGetContextThread = (ZwGetContextThread_t)::GetProcAddress(ntdll, "ZwGetContextThread");
			ZwResumeThread = (ZwResumeThread_t)::GetProcAddress(ntdll, "ZwResumeThread");
			ZwQueryInformationThread = (ZwQueryInformationThread_t)::GetProcAddress(ntdll, "ZwQueryInformationThread");
			ZwWriteVirtualMemory = (ZwWriteVirtualMemory_t)::GetProcAddress(ntdll, "ZwWriteVirtualMemory");
			ZwClose = (ZwClose_t)::GetProcAddress(ntdll, "ZwClose");

			if (ZwCreateProcess && ZwQuerySystemInformation && ZwQueryVirtualMemory &&
				ZwCreateThread && ZwGetContextThread && ZwResumeThread &&
				ZwQueryInformationThread && ZwWriteVirtualMemory && ZwClose)
				return true;

			ZwCreateProcess = nullptr;
			ZwQuerySystemInformation = nullptr;
			ZwQueryVirtualMemory = nullptr;
			ZwCreateThread = nullptr;
			ZwGetContextThread = nullptr;
			ZwResumeThread = nullptr;
			ZwQueryInformationThread = nullptr;
			ZwWriteVirtualMemory = nullptr;
			ZwClose = nullptr;
			return false;
		}


		static int Fork()
		{
			HANDLE hProcess = 0;
			HANDLE hThread = 0;
			OBJECT_ATTRIBUTES oa = { sizeof(oa) };
			MEMORY_BASIC_INFORMATION mbi;
			CLIENT_ID cid;
			USER_STACK stack;
			PNT_TIB tib;
			THREAD_BASIC_INFORMATION tbi;

			CONTEXT context = {
				CONTEXT_FULL |
				CONTEXT_DEBUG_REGISTERS |
				CONTEXT_FLOATING_POINT
			};

			if (::setjmp(jenv) != 0)
				// return as a child
				return 0;

			// check whether the entry points are
			// initialized and get them if necessary
			if (!ZwCreateProcess && !HaveLoadedFunctionsForFork())
				return -1;

			// create forked process
			::ZwCreateProcess(&hProcess, PROCESS_ALL_ACCESS, &oa,
				::NtCurrentProcess(), true, 0, 0, 0);

			// set the Eip for the child process to our child function
			::ZwGetContextThread(::NtCurrentThread(), &context);

			// In x64 the Eip and Esp are not present,
			// their x64 counterparts are Rip and Rsp respectively.
#if _WIN64
			context.Rip = (ULONG)child_entry;
#else
			context.Eip = (ULONG)child_entry;
#endif

#if _WIN64
			::ZwQueryVirtualMemory(NtCurrentProcess(), (PVOID)context.Rsp,
								 MemoryBasicInformation, &mbi, sizeof mbi, 0);
#else
			::ZwQueryVirtualMemory(NtCurrentProcess(), (PVOID)context.Esp,
								 MemoryBasicInformation, &mbi, sizeof mbi, 0);
#endif

			stack.FixedStackBase = 0;
			stack.FixedStackLimit = 0;
			stack.ExpandableStackBase = (PCHAR)mbi.BaseAddress + mbi.RegionSize;
			stack.ExpandableStackLimit = mbi.BaseAddress;
			stack.ExpandableStackBottom = mbi.AllocationBase;

			// create thread using the modified context and stack
			if (FAILED(::ZwCreateThread(&hThread, THREAD_ALL_ACCESS, &oa, hProcess,
				&cid, &context, &stack, TRUE))
				return -1;

			// copy exception table
			::ZwQueryInformationThread(NtCurrentThread(), ThreadBasicInformation,
									 &tbi, sizeof tbi, 0);
			tib = (PNT_TIB)tbi.TebBaseAddress;
			::ZwQueryInformationThread(hThread, ThreadBasicInformation,
				&tbi, sizeof tbi, 0);
			::ZwWriteVirtualMemory(hProcess, tbi.TebBaseAddress,
				&tib->ExceptionList, sizeof tib->ExceptionList, 0);

			// start (resume really) the child
			::ZwResumeThread(hThread, 0);

			// clean up
			::ZwClose(hThread);
			::ZwClose(hProcess);

			// exit with child's pid
			return (int)cid.UniqueProcess;
		}


	} // namespace anonymous




	bool Program::ThreadMonitor::spawnProcess()
	{
		#warning not implemented on windows
		assert(false and "not implemented on Windows");

		SECURITY_ATTRIBUTES saAttr;

		// Set the bInheritHandle flag so pipe handles are inherited.
		saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
		saAttr.bInheritHandle = TRUE;
		saAttr.lpSecurityDescriptor = nullptr;

		HANDLE readOut = nullptr;
		HANDLE writeOut = nullptr;
		// Create a pipe for the child process's STDOUT.
		if (!::CreatePipe(&readOut, &writeOut, &saAttr, 0))
		{
			std::cerr << "pipe failed : failed to create pipe for stdout\n";
			return false;
		}

		// Ensure the read handle to the pipe for STDOUT is not inherited.
		if (!::SetHandleInformation(readOut, HANDLE_FLAG_INHERIT, 0))
		{
			std::cerr << "pipe failed : failed to set handle information on stdout pipe\n";
			return false;
		}

		HANDLE readIn = nullptr;
		HANDLE writeIn = nullptr;
		// Create a pipe for the child process's STDIN.
		if (!::CreatePipe(&readIn, &writeIn, &saAttr, 0))
		{
			std::cerr << "pipe failed : failed to create pipe for stdin\n";
			return false;
		}

		// Ensure the write handle to the pipe for STDIN is not inherited.
		if (!::SetHandleInformation(writeIn, HANDLE_FLAG_INHERIT, 0))
		{
			std::cerr << "pipe failed : failed to set handle information on stdin pipe\n";
			return false;
		}

		// Set up members of the PROCESS_INFORMATION structure.
		PROCESS_INFORMATION procInfo;
		::ZeroMemory(&procInfo, sizeof(PROCESS_INFORMATION));

		// Set up members of the STARTUPINFO structure.
		// This structure specifies the STDIN and STDOUT handles for redirection.
		STARTUPINFO startInfo;
		::ZeroMemory(&startInfo, sizeof(STARTUPINFO));
		startInfo.cb = sizeof(STARTUPINFO);
		startInfo.hStdError = writeOut;
		startInfo.hStdOutput = writeOut;
		startInfo.hStdInput = readIn;
		startInfo.dwFlags |= STARTF_USESTDHANDLES;

		TCHAR cmdline[] = TEXT("child");

		// Getting the start time of execution
		pStartTime = currentTime();

		// Create the child process.
		// ** FORK **
		bool success = ::CreateProcess(nullptr,
			cmdline,	// command line
			nullptr,	// process security attributes
			nullptr,	// primary thread security attributes
			true,		// handles are inherited
			0,			// creation flags
			nullptr,	// use parent's environment
			nullptr,	// use parent's current directory
			&startInfo, // STARTUPINFO pointer
			&procInfo); // receives PROCESS_INFORMATION

		// If an error occurs, exit the application.
		if (!success)
		{
			std::cerr << "fork failed\n";
			return false;
		}

		// Close handles to the child process and its primary thread.
		// Some applications might keep these handles to monitor the status
		// of the child process, for example.
		::CloseHandle(procInfo.hProcess);
		::CloseHandle(procInfo.hThread);

		return true;
	}


	void Program::ThreadMonitor::waitForSubProcess()
	{
		#warning not implemented on windows
		pEndTime = currentTime();
	}


	void Program::ThreadMonitor::cleanupAfterChildTermination()
	{
	}


	void Program::ThreadMonitor::onKill()
	{
		// the thread has been killed - killing the sub process if any
		bool killed = false;

		// try to kill the attached child process if any
		{
			# warning not implemented
			// getting the current time as soon as possible
			pEndTime = currentTime();
		}

		theProcessHasStopped(killed, -127);
	}




} // namespace Process
} // namespace Yuni

#endif
