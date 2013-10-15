#ifndef __YUNI_MESSAGING_SERVICE_HXX__
# define __YUNI_MESSAGING_SERVICE_HXX__


namespace Yuni
{
namespace Messaging
{

	inline Net::Error Service::Transports::add(const Net::Port& port, Transport::ITransport::Ptr transport)
	{
		return add("*", port, transport);
	}


	inline bool Service::HeavyTasks::enabled() const
	{
		return pEnabled;
	}


	inline void Service::HeavyTasks::enabled(bool on)
	{
		pEnabled = (on ? 1 : 0);
	}


	inline Service::HeavyTasks& Service::HeavyTasks::operator += (Job::IJob* job)
	{
		queue += job;
		return *this;
	}


	inline Service::HeavyTasks& Service::HeavyTasks::operator += (Job::IJob::Ptr& job)
	{
		queue += job;
		return *this;
	}





} // namespace Messaging
} // namespace Yuni

#endif // __YUNI_MESSAGING_SERVICE_HXX__
