#pragma once
// file included by program.cpp

#ifdef YUNI_OS_WINDOWS

// http://msdn.microsoft.com/en-us/library/ms682499%28v=vs.85%29.aspx
// http://blogs.msdn.com/b/oldnewthing/archive/2011/12/16/10248328.aspx
// http://stackoverflow.com/questions/53208/how-do-i-automatically-destroy-child-processes-in-windows

# include "../../system/windows.hdr.h"
# include <WinNT.h>
# include "../../string/wstring.h"
# if _WIN32_WINNT >= 0x0600
#	include <processthreadsapi.h>
# endif
# include <iostream>
# include <io.h>


namespace Yuni
{
namespace Process
{

	namespace // anonymous
	{

		static inline bool pipe(HANDLE (&fd)[2], SECURITY_ATTRIBUTES& attr, const char* const pipeName)
		{
			// Create a pipe for the child process's STDOUT.
			if (!::CreatePipe(fd, fd + 1, &attr, 0))
			{
				std::cerr << "pipe failed : failed to create pipe for "
						  << pipeName << '\n';
				return false;
			}

			// Ensure the read handle to the pipe for STDOUT is not inherited.
			if (!::SetHandleInformation(fd[0], HANDLE_FLAG_INHERIT, 0))
			{
				std::cerr << "pipe failed : failed to set handle information on "
						  << pipeName << " pipe\n";
				return false;
			}

			return true;
		}


#if _WIN32_WINNT >= 0x0600
		static inline bool fillProcAttributeList(HANDLE (&handles)[3], LPPROC_THREAD_ATTRIBUTE_LIST& attrList)
		{
			SIZE_T size = 0;
			if (!::InitializeProcThreadAttributeList(nullptr, 1, 0, &size))
				return false;
			if ((attrList = reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(
				::HeapAlloc(::GetProcessHeap(), 0, size))) != nullptr)
				return false;
			if (!::InitializeProcThreadAttributeList(attrList, 1, 0, &size))
				return false;
			return ::UpdateProcThreadAttribute(attrList, 0, PROC_THREAD_ATTRIBUTE_HANDLE_LIST,
				handles, 3 * sizeof(HANDLE), nullptr, nullptr);
		}
#endif


		bool connectToNewClient(HANDLE hPipe, LPOVERLAPPED lpo)
		{
			bool connected = false;
			bool pendingIO = false;

			// Start an overlapped connection for this pipe instance.
			fConnected = ::ConnectNamedPipe(hPipe, lpo);

			// Overlapped ConnectNamedPipe should return zero.
			if (fConnected)
			{
				std::cerr << "ConnectNamedPipe failed with " << ::GetLastError() << '\n';
				return false;
			}

			switch (::GetLastError())
			{
				// The overlapped connection in progress.
				case ERROR_IO_PENDING:
					pendingIO = TRUE;
					break;

					// Client is already connected, so signal an event.
				case ERROR_PIPE_CONNECTED:
					if (::SetEvent(lpo->hEvent))
						break;

					// If an error occurs during the connect operation...
				default:
					{
						std::cerr << "ConnectNamedPipe failed with " << ::GetLastError() << '\n';
						return 0;
					}
			}

			return pendingIO;
		}


	} // namespace anonymous




	bool Program::ThreadMonitor::spawnProcess()
	{
		SECURITY_ATTRIBUTES saAttr;

		// Set the bInheritHandle flag so pipe handles are inherited.
		saAttr.nLength = (uint32) sizeof(SECURITY_ATTRIBUTES);
		saAttr.bInheritHandle = TRUE;
		saAttr.lpSecurityDescriptor = nullptr;

		if (!pipe(channels.outfd, saAttr, "stdout"))
			return false;

		if (!pipe(channels.infd, saAttr, "stdin"))
			return false;

		if (!pipe(channels.errd, saAttr, "stderr"))
			return false;


#if _WIN32_WINNT >= 0x0600
		// Prepare inherited handle list for STARTUPINFOEX
		LPPROC_THREAD_ATTRIBUTE_LIST attrList = nullptr;
		HANDLE inheritedHandles[3] = {
			channels.outfd[0],
			channels.infd[1],
			channels.errd[1],
		};
		fillProcAttributeList(inheritedHandles, attrList);

		// Set up members of the STARTUPINFOEX structure.
		// This structure specifies the handles for redirection.
		// It is necessary to use the EX version, otherwise _all_ inheritable
		// handles in the program are inherited by the child.
		STARTUPINFOEX startInfo;
		::ZeroMemory(&startInfo, sizeof(STARTUPINFOEX));
		startInfo.StartupInfo.cb = (uint32) sizeof(STARTUPINFOEX);
		startInfo.lpAttributeList = attrList;
		startInfo.StartupInfo.dwFlags |= STARTF_USESTDHANDLES;
		startInfo.StartupInfo.hStdOutput = channels.outfd[0];
		startInfo.StartupInfo.hStdInput = channels.infd[1];
		startInfo.StartupInfo.hStdError = channels.errd[1];
#else
		STARTUPINFO startInfo;
		::ZeroMemory(&startInfo, sizeof(STARTUPINFO));
		startInfo.cb = (uint32) sizeof(STARTUPINFO);
		startInfo.dwFlags |= STARTF_USESTDHANDLES;
		startInfo.hStdOutput = channels.outfd[0];
		startInfo.hStdInput = channels.infd[1];
		startInfo.hStdError = channels.errd[1];
#endif // _WIN32_WINNT < 0x0600

		// Set up members of the PROCESS_INFORMATION structure.
		PROCESS_INFORMATION winProcInfo;
		::ZeroMemory(&winProcInfo, sizeof(PROCESS_INFORMATION));

		Yuni::Private::WString<> cmdLine(procinfo.executable);

		// Getting the start time of execution
		pStartTime = currentTime();

		// Create the child process.
		// ** FORK **
		bool success = 0 != ::CreateProcessW(nullptr,
			cmdLine.c_str(),	// command line
			nullptr,			// process security attributes
			nullptr,			// primary thread security attributes
			true,				// handles are inherited (necessary for STARTF_USESTDHANDLES)
			0,					// creation flags
			nullptr,			// use parent's environment
			nullptr,			// use parent's current directory
			&startInfo,			// STARTUPINFO pointer
			&winProcInfo);		// receives PROCESS_INFORMATION

		// If an error occurs, give up
		if (!success)
		{
			//DWORD error = ::GetLastError();
			//std::wcerr << L"Fork failed with:\n";
			//if (error)
			//{
			//	LPVOID lpMsgBuf = nullptr;
			//	DWORD bufLen = ::FormatMessageW(
			//		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			//		nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			//		(LPTSTR) &lpMsgBuf, 1024, nullptr);
			//	if (bufLen)
			//	{
			//		LPCTSTR lpMsgStr = (LPCTSTR)lpMsgBuf;
			//		std::wcerr << TEXT("  ") << lpMsgStr << std::endl;
			//		::HeapFree(::GetProcessHeap(), 0, lpMsgBuf);
			//		return false;
			//	}
			//}
			//std::wcerr << "  Unknown error !" << std::endl;
			return false;
		}

		// Close streams passed to the child process
		::CloseHandle(channels.outfd[0]);
		::CloseHandle(channels.infd[1]);
		::CloseHandle(channels.errd[1]);

		processHandle = winProcInfo.hProcess;
		threadHandle = winProcInfo.hThread;
		procinfo.processID = (int)winProcInfo.dwProcessId;
		procinfo.processInput = _open_osfhandle(reinterpret_cast<intptr_t>(channels.infd[0]), 0);

		// timeout for the sub process
		procinfo.createThreadForTimeoutWL();

		return true;
	}


	void Program::ThreadMonitor::waitForSubProcess()
	{
		// a 4K buffer seems the most efficient size
		enum { bufferSize = 4096 };
		enum { nbHandles = 3 };

		OVERLAPPED outOverlap;
		HANDLE outEvent = ::CreateEvent(nullptr, false, false, nullptr);
		outOverlap.hEvent = outEvent;
		connectToNewClient(channels.infd[0], &outOverlap);
		OVERLAPPED errOverlap;
		HANDLE errEvent = ::CreateEvent(nullptr, false, false, nullptr);
		errOverlap.hEvent = errEvent;
		connectToNewClient(channels.errd[0], &errOverlap);

		// Wait for all these handles
		HANDLE handleList[nbHandles] = {
			processHandle,
			outEvent,
			errEvent
		};

		bool finished = false;
		do
		{
			uint32 waitStatus = ::WaitForMultipleObjectsEx(nbHandles, handleList, false, INFINITE, true);
			if (/*waitStatus >= WAIT_OBJECT_0 and*/ waitStatus < WAIT_OBJECT_0 + nbHandles)
			{
				HANDLE signalled = handleList[waitStatus - WAIT_OBJECT_0];
				DWORD exitCode;
				// If the process was signalled
				if (signalled == processHandle && ::GetExitCodeProcess(processHandle, &exitCode))
				{
					// It is forbidden to use STILL_ACTIVE as an exit code
					// However, if it happens in real life, let it happen
					assert(exitCode != STILL_ACTIVE and "Child process uses STILL_ACTIVE as exit status !");
					pExitStatus = (int)exitCode;
					pEndTime = currentTime();
					finished = true;
					// TODO : How to know if the process was signalled or exited ?
					// pKilled = true;
				}
				else // If the child wrote to stdout or stderr
				{
					DWORD readBytes;
					DWORD writtenBytes;
					DWORD totalAvailBytes;
					DWORD bytesLeft;
					char buffer[bufferSize];
					bool cout = waitStatus - WAIT_OBJECT_0 == 1; // otherwise, cerr
					HANDLE writeTo = ::GetStdHandle(cout ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE);
					HANDLE readFrom = cout ? channels.infd[0] : channels.errd[0];

					while (true)
					{/*
						bool success = ::PeekNamedPipe(readFrom, nullptr, bufferSize, &readBytes,
							&totalAvailBytes, &bytesLeft);
						std::cout << "Peeking : " << readBytes << " bytes to read" << std::endl;
						if (not success || readBytes == 0)
						break;*/
						std::cout << "Ready to read !" << std::endl;
						success = ::ReadFile(readFrom, buffer, bufferSize, &readBytes, nullptr);
						std::cout << "Read " << readBytes << " characters" << std::endl;
						if (not success || readBytes == 0)
							break;
						success = ::WriteFile(writeTo, buffer, readBytes, &writtenBytes, nullptr);
						if (not success)
							break;
					}
				}
			}
			else // Abnormal completion
			{
				switch (waitStatus)
				{
					// Async I/O event
					case WAIT_IO_COMPLETION:
						// TODO Manage I/O for IPC
						break;
					case WAIT_FAILED: // if WaitFSO failed, give up looping
						pEndTime = currentTime();
						finished = true;
						break;
					case WAIT_TIMEOUT: // Normally not possible with INFINITE
						assert(false and "Program::ThreadMonitor::waitForSubProcess : Timeout on infinite !");
						// Keep looping
						break;
					default: // WAIT_ABANDONED_X is only for mutexes, this should never occur
						assert(false and "Program::ThreadMonitor::waitForSubProcess : Unmanaged WaitForSingleObject return code !");
						break;
				}
			}
		}
		while (not finished);

		// end of the chrono
		assert(pEndTime != 0 and "pEndTime is not properly initialized");
	}


	void Program::ThreadMonitor::cleanupAfterChildTermination()
	{
		// stop the thread dedicated to handle the timeout
		if (procinfo.timeoutThread)
		{
			procinfo.timeoutThread->stop();
			delete procinfo.timeoutThread;
			procinfo.timeoutThread = nullptr;
		}

		::CloseHandle(channels.outfd[1]);
		::CloseHandle(channels.infd[0]);
		::CloseHandle(channels.errd[0]);
		::CloseHandle(processHandle);
		::CloseHandle(threadHandle);
	}


	void Program::ThreadMonitor::onKill()
	{
		// the thread has been killed - killing the sub process if any
		bool killed = false;

		// try to kill the attached child process if any
		{
			// killing the sub-process, until it is really dead
			::TerminateProcess(processHandle, (uint)-127);
			uint32 waitStatus = ::WaitForSingleObject(processHandle, INFINITE);
			// getting the current time as soon as possible
			pEndTime = currentTime();
			switch (waitStatus)
			{
				case WAIT_FAILED:
					// the process was probably already dead
					break;
				default:
					// the process existed and has been killed
					killed = true;
					pExitStatus = -127;
					break;
			}
			MutexLocker locker(procinfo.mutex);
			procinfo.processID = 0;

			cleanupAfterChildTermination();
		}

		theProcessHasStopped(killed, -127);
	}




} // namespace Process
} // namespace Yuni

#endif // YUNI_OS_WINDOWS
