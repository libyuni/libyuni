
// file included by program.cpp

#ifdef YUNI_OS_WINDOWS

// http://msdn.microsoft.com/en-us/library/ms682499%28v=vs.85%29.aspx
// http://stackoverflow.com/questions/53208/how-do-i-automatically-destroy-child-processes-in-windows



namespace Yuni
{
namespace Process
{


	bool Program::ThreadMonitor::spawnProcess()
	{
		#warning not implemented on windows
		assert(false and "not implemented on Windows");
		env.mutex.unlock();
		pStartTime = currentTime();
		return false;
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
