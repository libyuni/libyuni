#ifndef __YUNI_PRIVATE_DBI_CONNECTOR_DATA_HXX__
# define __YUNI_PRIVATE_DBI_CONNECTOR_DATA_HXX__

# include "../../thread/id.h"


namespace Yuni
{
namespace Private
{
namespace DBI
{

	inline ConnectorData::ConnectorData(const Yuni::DBI::Settings& settings, Yuni::DBI::Adapter::IAdapter* instance) :
		settings(settings),
		instance(instance)
	{
		assert(instance != NULL);

		// reset adapter
		(void)::memset(&adapter, '\0', sizeof(adapter));
		instance->retrieveEntries(adapter);
		assert(adapter.dbh == NULL and "the variable dbh must not be set by adapter");
	}


	inline ChannelPtr ConnectorData::openChannel()
	{
		// current thread id
		auto threadid = Thread::ID();

		// locker
		Yuni::MutexLocker locker(mutex);

		// checking if a channel does not already exists
		Channel::Table::iterator i = channels.find(threadid);
		return (i != channels.end())
			? i->second
			: createNewChannelWL(threadid);
	}





} // namespace DBI
} // namespace Private
} // namespace Yuni

#endif // __YUNI_PRIVATE_DBI_CONNECTOR_DATA_HXX__
