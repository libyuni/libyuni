#ifndef __YUNI_CORE_PROCESS_PROGRAM_STREAM_H__
# define __YUNI_CORE_PROCESS_PROGRAM_PROGRAM_H__

# include "../../../yuni.h"
# include "../../smartptr.h"




namespace Yuni
{
namespace Process
{

	/*!
	** \brief Process Stream
	**
	** The lifetime of a process stream is guarantee to be at least
	** the whole execution of the underlying process
	*/
	class Stream
	{
	public:
		typedef SmartPtr<Stream> Ptr;

	public:
		Stream() {}
		virtual ~Stream() {}

		//! Some data from the standard output are ready
		virtual void onRead(const AnyString& /*buffer*/) {}

		//! Some data from the error output are ready
		virtual void onErrorRead(const AnyString& /*buffer*/) {}

		//! The execution has finished
		virtual void onStop(bool /*killed*/, int /*exitstatus*/, sint64 /*duration*/) {}
	};





	class CaptureOutput : public Process::Stream
	{
	public:
		typedef SmartPtr<CaptureOutput>  Ptr;

	public:
		CaptureOutput() {}
		virtual ~CaptureOutput() {}

		//! Some data from the standard output are ready
		virtual void onRead(const AnyString& buffer) override
		{
			cout += buffer;
		}

		//! Some data from the error output are ready
		virtual void onErrorRead(const AnyString& buffer) override
		{
			cerr += buffer;
		}


	public:
		Clob cout;
		Clob cerr;

	}; // class CaptureOutput





} // namespace Process
} // namespace Yuni

#endif // __YUNI_CORE_PROCESS_PROGRAM_PROGRAM_H__
