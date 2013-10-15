#ifndef __YUNI_CORE_PROCESS_PROCESS_HXX__
# define __YUNI_CORE_PROCESS_PROCESS_HXX__


namespace Yuni
{


	template<class MainLoopT>
	bool Process::execute(MainLoopT& mainloop, uint timeout)
	{
		Bind<void (const Callback&)> dispatcher;
		dispatcher.bind<MainLoopT>(&mainloop, & MainLoopT::dispatch);
		return dispatchExecution(dispatcher, timeout);
	}


	inline Process::Stream::Ptr  Process::stream() const
	{
		return pStream;
	}


	inline void Process::stream(Stream::Ptr newstream)
	{
		pStream = newstream;
	}




} // namespace Yuni

#endif // __YUNI_CORE_PROCESS_PROCESS_HXX__
