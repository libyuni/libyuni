#ifndef __YUNI_JOB_QUEUE_EVENT_H__
# define __YUNI_JOB_QUEUE_EVENT_H__


namespace Yuni
{

	enum QServiceEvent
	{
		//! Event: the queue service has been stopped
		qseStop,
		//! Event: the queue service is idle
		qseIdle
	};




} // namespace Yuni

#endif // __YUNI_JOB_QUEUE_EVENT_H__
