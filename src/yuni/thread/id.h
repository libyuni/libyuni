#ifndef __YUNI_THREAD_ID_H__
# define __YUNI_THREAD_ID_H__

# include "../yuni.h"


namespace Yuni
{
namespace Thread
{

	/*!
	** \brief Get the Unique ID of the current thread
	**
	** \warning This value should be considered as an arbitrary value
	**  (see implementation of pthread_self / GetCurrentThreadId)
	*/
	YUNI_DECL uint64 ID();



} // namespace Thread
} // namespace Yuni

#endif // __YUNI_THREAD_ID_H__
