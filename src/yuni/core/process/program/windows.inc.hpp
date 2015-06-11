#pragma once
// file included by program.cpp

#ifdef YUNI_OS_WINDOWS

// http://msdn.microsoft.com/en-us/library/ms682499%28v=vs.85%29.aspx
// http://stackoverflow.com/questions/53208/how-do-i-automatically-destroy-child-processes-in-windows
// http://stackoverflow.com/questions/985281/what-is-the-closest-thing-windows-has-to-fork/985525#985525

# include "../../system/windows.hdr.h"
//# include <windows.h>
# include <WinNT.h>
//# include <setjmp.h>
# include "../../string/wstring.h"


namespace Yuni
{
namespace Process
{



	bool Program::ThreadMonitor::spawnProcess()
	{
		SECURITY_ATTRIBUTES saAttr;

		// Set the bInheritHandle flag so pipe handles are inherited.
		saAttr.nLength = (uint32) sizeof(SECURITY_ATTRIBUTES);
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
		PROCESS_INFORMATION winProcInfo;
		::ZeroMemory(&winProcInfo, sizeof(PROCESS_INFORMATION));

		// Set up members of the STARTUPINFO structure.
		// This structure specifies the STDIN and STDOUT handles for redirection.
		STARTUPINFO startInfo;
		::ZeroMemory(&startInfo, sizeof(STARTUPINFO));
		startInfo.cb = (uint32) sizeof(STARTUPINFO);
		startInfo.hStdError = writeOut;
		startInfo.hStdOutput = writeOut;
		startInfo.hStdInput = readIn;
		startInfo.dwFlags |= STARTF_USESTDHANDLES;

		Yuni::Private::WString<> cmdLine(procinfo.executable);

		// Getting the start time of execution
		pStartTime = currentTime();

		// Create the child process.
		// ** FORK **
		bool success = ::CreateProcessW(nullptr,
			cmdLine.c_str(),	// command line
			nullptr,			// process security attributes
			nullptr,			// primary thread security attributes
			false,				// handles are not inherited
			0,					// creation flags
			nullptr,			// use parent's environment
			nullptr,			// use parent's current directory
			&startInfo,			// STARTUPINFO pointer
			&winProcInfo);		// receives PROCESS_INFORMATION

		// If an error occurs, exit the application.
		if (!success)
		{
			//DWORD error = ::GetLastError();
			//std::wcerr << L"Fork failed with:\n";
			//if (error)
			//{
			//	LPVOID lpMsgBuf = nullptr;
			//	DWORD bufLen = ::FormatMessageW(
			//		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			//		NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			//		(LPTSTR) &lpMsgBuf, 1024, NULL);
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

		processHandle = winProcInfo.hProcess;
		threadHandle = winProcInfo.hThread;
		procinfo.processID = (int)winProcInfo.dwProcessId;
		procinfo.processInput = _open_osfhandle(reinterpret_cast<intptr_t>(readIn), 0);

		// Close streams passed to the child process
		::CloseHandle(writeOut);
		return true;
	}


	void Program::ThreadMonitor::waitForSubProcess()
	{
		bool finished = false;
		do
		{
			// TODO Manage I/O for IPC

			switch (::WaitForSingleObject(processHandle, 100))
			{
				// Signaled
				case WAIT_OBJECT_0:
					DWORD exitCode;
					if (::GetExitCodeProcess(processHandle, &exitCode))
					{
						pExitStatus = (int)exitCode;
						assert(pExitStatus != STILL_ACTIVE);
						pEndTime = currentTime();
						finished = true;
						// TODO : How to know if the process was signaled or exited ?
					}
					break;
				case WAIT_FAILED: // if WaitFSO failed, give up looping
					finished = true;
					break;
				case WAIT_TIMEOUT:
					// Keep looping
					break;
				default: // WAIT_ABANDONED leads here, it should never occur
					assert(false and "Unmanaged WaitForSingleObject return code !");
			}
		}
		while (not finished);

		// end of the chrono
		assert(pEndTime != 0 and "pEndTime is not properly initialized");
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
			#ifndef YUNI_OS_MSVC
			# warning not implemented
			#endif
			// getting the current time as soon as possible
			pEndTime = currentTime();
		}

		theProcessHasStopped(killed, -127);
	}




} // namespace Process
} // namespace Yuni

#endif // YUNI_OS_WINDOWS
