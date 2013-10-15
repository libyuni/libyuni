#ifndef __YUNI_MESSAGING_TRANSPORT_TRANSPORT_HXX__
# define __YUNI_MESSAGING_TRANSPORT_TRANSPORT_HXX__


namespace Yuni
{
namespace Messaging
{
namespace Transport
{


	inline ITransport::ITransport(Mode m) :
		mode(m),
		pAttachedThread(nullptr),
		pService(nullptr)
	{
		// do nothing
	}


	inline ITransport::~ITransport()
	{
		// do nothing
	}


	inline Thread::IThread*  ITransport::attachedThread()
	{
		return pAttachedThread;
	}


	inline const Thread::IThread*  ITransport::attachedThread() const
	{
		return pAttachedThread;
	}


	inline void ITransport::attachedThread(Thread::IThread* thread)
	{
		pAttachedThread = thread;
	}






} // namespace Transport
} // namespace Messaging
} // namespace Yuni

#endif // __YUNI_MESSAGING_TRANSPORT_TRANSPORT_HXX__
