#pragma once
#include "program.h"



namespace Yuni
{
namespace Process
{

	template<class MainLoopT>
	inline bool Execute(MainLoopT& mainloop, const AnyString& commandLine, uint timeout)
	{
		Program program;
		program.commandLine(commandLine);
		return (program.execute(mainloop, timeout)) ? (0 == program.wait()) : false;
	}

	inline bool Execute(const AnyString& commandLine, uint timeout)
	{
		Program program;
		program.commandLine(commandLine);
		return (program.execute(timeout)) ? (0 == program.wait()) : false;
	}




	template<class MainLoopT>
	inline bool Program::execute(MainLoopT& mainloop, uint timeout)
	{
		Bind<void (const Callback&)> dispatcher;
		dispatcher.bind<MainLoopT>(&mainloop, & MainLoopT::dispatch);
		return dispatchExecution(dispatcher, timeout);
	}


	inline Stream::Ptr  Program::stream() const
	{
		return pStream;
	}


	inline void Program::stream(Stream::Ptr newstream)
	{
		pStream = newstream;
	}





} // namespace Process
} // namespace Yuni

