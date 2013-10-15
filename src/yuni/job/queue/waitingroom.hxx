#ifndef __YUNI_JOB_WAITING_ROOM_HXX__
# define __YUNI_JOB_WAITING_ROOM_HXX__


namespace Yuni
{
namespace Private
{
namespace QueueService
{

	inline bool WaitingRoom::empty() const
	{
		return (!pJobCount);
	}


	inline uint WaitingRoom::size() const
	{
		return (uint) pJobCount;
	}




} // namespace QueueService
} // namespace Private
} // namespace Yuni

#endif // __YUNI_JOB_WAITING_ROOM_HXX__


