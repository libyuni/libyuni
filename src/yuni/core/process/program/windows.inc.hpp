#pragma once
// file included by program.cpp

#ifdef YUNI_OS_WINDOWS

// http://msdn.microsoft.com/en-us/library/ms682499%28v=vs.85%29.aspx
// http://stackoverflow.com/questions/53208/how-do-i-automatically-destroy-child-processes-in-windows
// http://stackoverflow.com/questions/985281/what-is-the-closest-thing-windows-has-to-fork/985525#985525

//# include "../../system/windows.hdr.h"
# include <windows.h>
# include <WinNT.h>
# include <setjmp.h>


namespace Yuni
{
namespace Process
{

	namespace // anonymous
	{

#define NtCurrentProcess() ((HANDLE)-1)
#define NtCurrentThread() ((HANDLE) -2)

		typedef struct _SYSTEM_HANDLE_INFORMATION
		{
			ULONG ProcessId;
			UCHAR ObjectTypeNumber;
			UCHAR Flags;
			USHORT Handle;
			PVOID Object;
			ACCESS_MASK GrantedAccess;
		} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

		typedef struct _OBJECT_ATTRIBUTES
		{
			ULONG Length;
			HANDLE RootDirectory;
			PVOID /* really PUNICODE_STRING */  ObjectName;
			ULONG Attributes;
			PVOID SecurityDescriptor;       /* type SECURITY_DESCRIPTOR */
			PVOID SecurityQualityOfService; /* type SECURITY_QUALITY_OF_SERVICE */
		} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

		typedef enum _MEMORY_INFORMATION_
		{
			MemoryBasicInformation,
			MemoryWorkingSetList,
			MemorySectionName,
			MemoryBasicVlmInformation
		} MEMORY_INFORMATION_CLASS;

		typedef struct _CLIENT_ID
		{
			HANDLE UniqueProcess;
			HANDLE UniqueThread;
		} CLIENT_ID, *PCLIENT_ID;

		typedef struct _USER_STACK
		{
			PVOID FixedStackBase;
			PVOID FixedStackLimit;
			PVOID ExpandableStackBase;
			PVOID ExpandableStackLimit;
			PVOID ExpandableStackBottom;
		} USER_STACK, *PUSER_STACK;

		typedef LONG KPRIORITY;
		typedef ULONG_PTR KAFFINITY;
		typedef KAFFINITY *PKAFFINITY;

		typedef struct _THREAD_BASIC_INFORMATION
		{
			LONG ExitStatus;
			PVOID TebBaseAddress;
			CLIENT_ID ClientId;
			KAFFINITY AffinityMask;
			KPRIORITY Priority;
			KPRIORITY BasePriority;
		} THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION;

		typedef enum _THREAD_INFORMATION_CLASS
		{
			ThreadBasicInformation,
			ThreadTimes,
			ThreadPriority,
			ThreadBasePriority,
			ThreadAffinityMask,
			ThreadImpersonationToken,
			ThreadDescriptorTableEntry,
			ThreadEnableAlignmentFaultFixup,
			ThreadEventPair,
			ThreadQuerySetWin32StartAddress,
			ThreadZeroTlsCell,
			ThreadPerformanceCount,
			ThreadAmILastThread,
			ThreadIdealProcessor,
			ThreadPriorityBoost,
			ThreadSetTlsArrayAddress,
			ThreadIsIoPending,
			ThreadHideFromDebugger
		} THREAD_INFORMATION_CLASS, *PTHREAD_INFORMATION_CLASS;

		typedef enum _SYSTEM_INFORMATION_CLASS { SystemHandleInformation = 0x10 } SYSTEM_INFORMATION_CLASS;

		typedef LONG (NTAPI *ZwWriteVirtualMemory_t)(
			IN HANDLE ProcessHandle,
			IN PVOID BaseAddress,
			IN PVOID Buffer,
			IN ULONG NumberOfBytesToWrite,
			OUT PULONG NumberOfBytesWritten OPTIONAL);

		typedef LONG (NTAPI *ZwCreateProcess_t)(
			OUT PHANDLE ProcessHandle,
			IN ACCESS_MASK DesiredAccess,
			IN POBJECT_ATTRIBUTES ObjectAttributes,
			IN HANDLE InheriteFromProcessHandle,
			IN BOOLEAN InheritHandles,
			IN HANDLE SectionHandle OPTIONAL,
			IN HANDLE DebugPort OPTIONAL,
			IN HANDLE ExceptionPort OPTIONAL);

		typedef LONG (WINAPI *ZwQuerySystemInformation_t)(
			SYSTEM_INFORMATION_CLASS SystemInformationClass,
			PVOID SystemInformation,
			ULONG SystemInformationLength,
			PULONG ReturnLength);

		typedef LONG (NTAPI *ZwQueryVirtualMemory_t)(
			IN HANDLE ProcessHandle,
			IN PVOID BaseAddress,
			IN MEMORY_INFORMATION_CLASS MemoryInformationClass,
			OUT PVOID MemoryInformation,
			IN ULONG MemoryInformationLength,
			OUT PULONG ReturnLength OPTIONAL);

		typedef LONG (NTAPI *ZwGetContextThread_t)(
			IN HANDLE ThreadHandle,
			OUT PCONTEXT Context);

		typedef LONG (NTAPI *ZwCreateThread_t)(
			OUT PHANDLE ThreadHandle,
			IN ACCESS_MASK DesiredAccess,
			IN POBJECT_ATTRIBUTES ObjectAttributes,
			IN HANDLE ProcessHandle,
			OUT PCLIENT_ID ClientId,
			IN PCONTEXT ThreadContext,
			IN PUSER_STACK UserStack,
			IN BOOLEAN CreateSuspended);

		typedef LONG (NTAPI *ZwResumeThread_t)(
			IN HANDLE ThreadHandle,
			OUT PULONG SuspendCount OPTIONAL);

		typedef LONG (NTAPI *ZwClose_t)(IN HANDLE ObjectHandle);

		typedef LONG (NTAPI *ZwQueryInformationThread_t)(
			IN HANDLE ThreadHandle,
			IN THREAD_INFORMATION_CLASS ThreadInformationClass,
			OUT PVOID ThreadInformation,
			IN ULONG ThreadInformationLength,
			OUT PULONG ReturnLength OPTIONAL);


		static ZwCreateProcess_t ZwCreateProcess = nullptr;
		static ZwQuerySystemInformation_t ZwQuerySystemInformation = nullptr;
		static ZwQueryVirtualMemory_t ZwQueryVirtualMemory = nullptr;
		static ZwCreateThread_t ZwCreateThread = nullptr;
		static ZwGetContextThread_t ZwGetContextThread = nullptr;
		static ZwResumeThread_t ZwResumeThread = nullptr;
		static ZwClose_t ZwClose = nullptr;
		static ZwQueryInformationThread_t ZwQueryInformationThread = nullptr;
		static ZwWriteVirtualMemory_t ZwWriteVirtualMemory = nullptr;

		static jmp_buf jenv;

		static int ChildEntry()
		{
			::longjmp(jenv, 1);
			return 0;
		}


		static bool HaveLoadedFunctionsForFork()
		{
			auto ntdll = ::GetModuleHandle(L"ntdll");
			if (ntdll == nullptr)
				return false;

			if (ZwCreateProcess && ZwQuerySystemInformation && ZwQueryVirtualMemory &&
				ZwCreateThread && ZwGetContextThread && ZwResumeThread &&
				ZwQueryInformationThread && ZwWriteVirtualMemory && ZwClose)
			{
				return true;
			}

			ZwCreateProcess = (ZwCreateProcess_t)::GetProcAddress(ntdll, TEXT("ZwCreateProcess"));
			ZwQuerySystemInformation = (ZwQuerySystemInformation_t)::GetProcAddress(ntdll, TEXT("ZwQuerySystemInformation"));
			ZwQueryVirtualMemory = (ZwQueryVirtualMemory_t)::GetProcAddress(ntdll, TEXT("ZwQueryVirtualMemory"));
			ZwCreateThread = (ZwCreateThread_t)::GetProcAddress(ntdll, TEXT("ZwCreateThread"));
			ZwGetContextThread = (ZwGetContextThread_t)::GetProcAddress(ntdll, TEXT("ZwGetContextThread"));
			ZwResumeThread = (ZwResumeThread_t)::GetProcAddress(ntdll, TEXT("ZwResumeThread"));
			ZwQueryInformationThread = (ZwQueryInformationThread_t)::GetProcAddress(ntdll, TEXT("ZwQueryInformationThread"));
			ZwWriteVirtualMemory = (ZwWriteVirtualMemory_t)::GetProcAddress(ntdll, TEXT("ZwWriteVirtualMemory"));
			ZwClose = (ZwClose_t)::GetProcAddress(ntdll, TEXT("ZwClose"));

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
			#endif
			return false;
		}


		static intptr_t Fork()
		{
			#if 0
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
			ZwCreateProcess(&hProcess, PROCESS_ALL_ACCESS, &oa,
				NtCurrentProcess(), true, 0, 0, 0);

			// set the Eip for the child process to our child function
			ZwGetContextThread(NtCurrentThread(), &context);

			// In x64 the Eip and Esp are not present,
			// their x64 counterparts are Rip and Rsp respectively.
			#if _WIN64
			context.Rip = (uint64)ChildEntry;
			#else
			context.Eip = (uint32)ChildEntry;
			#endif

			#if _WIN64
			ZwQueryVirtualMemory(NtCurrentProcess(), (PVOID)context.Rsp,
								 MemoryBasicInformation, &mbi, sizeof mbi, 0);
			#else
			ZwQueryVirtualMemory(NtCurrentProcess(), (PVOID)context.Esp,
								 MemoryBasicInformation, &mbi, sizeof mbi, 0);
			#endif

			stack.FixedStackBase = 0;
			stack.FixedStackLimit = 0;
			stack.ExpandableStackBase = (PCHAR)mbi.BaseAddress + mbi.RegionSize;
			stack.ExpandableStackLimit = mbi.BaseAddress;
			stack.ExpandableStackBottom = mbi.AllocationBase;

			// create thread using the modified context and stack
			if (FAILED(ZwCreateThread(&hThread, THREAD_ALL_ACCESS, &oa, hProcess,
				&cid, &context, &stack, TRUE)))
				return -1;

			// copy exception table
			ZwQueryInformationThread(NtCurrentThread(), ThreadBasicInformation,
									 &tbi, sizeof tbi, 0);
			tib = (PNT_TIB)tbi.TebBaseAddress;
			ZwQueryInformationThread(hThread, ThreadBasicInformation,
				&tbi, sizeof tbi, 0);
			ZwWriteVirtualMemory(hProcess, tbi.TebBaseAddress,
				&tib->ExceptionList, sizeof tib->ExceptionList, 0);

			// start (resume really) the child
			ZwResumeThread(hThread, 0);

			// clean up
			ZwClose(hThread);
			ZwClose(hProcess);

			// exit with child's pid
			return (intptr_t)cid.UniqueProcess;
			#endif
			return -1;
		}


	} // namespace anonymous




	bool Program::ThreadMonitor::spawnProcess()
	{
		#ifndef YUNI_OS_MSVC
		#warning not implemented on windows
		#endif
		assert(false and "not implemented on Windows");

		#if 0
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
		#endif
		return false;
	}


	void Program::ThreadMonitor::waitForSubProcess()
	{
		#ifndef YUNI_OS_MSVC
		#warning not implemented on windows
		#endif
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
