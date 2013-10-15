#ifndef __YUNI_THREAD_FWD_H__
# define __YUNI_THREAD_FWD_H__

# ifndef YUNI_NO_THREAD_SAFE
#	ifdef YUNI_OS_WINDOWS
#		include "../core/system/windows.hdr.h"
#		define YUNI_THREAD_FNC_RETURN   DWORD WINAPI
#	else
#		define YUNI_THREAD_FNC_RETURN   void*
#	endif
# endif


namespace Yuni
{
namespace Thread
{

	//! Return error status
	enum Error
	{
		//! No error, the operation succeeded
		errNone = 0,
		//! A timeout occured
		errTimeout,
		//! Impossible to create the new thread
		errThreadCreation,
		//! The onStarting handler returned false.
		errAborted,
		//! The operation failed for an unkown reason
		errUnkown
	};


	enum
	{
		//! The default timeout for stopping a thread
		defaultTimeout = 5000, // 5 seconds
	};




} // namespace Thread
} // namespace Yuni





namespace Yuni
{
namespace Job
{

	// forward declaration
	class YUNI_DECL IJob;

} // namespace Job
} // namespace Yuni






# ifndef YUNI_NO_THREAD_SAFE

namespace Yuni
{
namespace Private
{
namespace Thread
{

	/*!
	** \brief This procedure will be run in a separate thread and will run IThread::onExecute()
	*/
	extern "C"  YUNI_THREAD_FNC_RETURN  threadCallbackExecute(void* arg);



} // namespace Yuni
} // namespace Private
} // namespace Thread

# endif // YUNI_NO_THREAD_SAFE

#endif // __YUNI_THREAD_FWD_H__
