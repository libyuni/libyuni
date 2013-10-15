#ifndef __YUNI_PRIVATE_DBI_CONNECTOR_DATA_H__
# define __YUNI_PRIVATE_DBI_CONNECTOR_DATA_H__

# include "../../dbi/settings.h"
# include "../../dbi/fwd.h"
# include "../../dbi/adapter.h"
# include "channel.h"
# include "../../core/event/event.h"



namespace Yuni
{
namespace Private
{
namespace DBI
{


	class ConnectorData final
	{
	public:
		//! Default constructor
		ConnectorData(const Yuni::DBI::Settings& settings, Yuni::DBI::Adapter::IAdapter* adapter);
		//! Destructor
		~ConnectorData();

		/*!
		** \brief Open a communication channel to the remote database (per thread)
		*/
		ChannelPtr openChannel();

		/*!
		** \brief Close all old channels
		**
		** \param idletime Idle time (seconds)
		** \param[out] remainingCount The number of channels currently opened (after cleanup)
		** \return The number of channels which have been closed
		*/
		uint closeTooOldChannels(uint idletime, uint& remainingCount);


	public:
		//! Settings used to connect to the database
		Yuni::DBI::Settings settings;
		//! Adapter Entries
		::yn_dbi_adapter adapter;
		//! Mutex
		Mutex mutex;

		//! All channels, ordered by a thread id
		Channel::Table channels;

		// delete the instance
		Yuni::DBI::Adapter::IAdapter* instance;

		//! Event trigered when a SQL error occurs
		Event<void ()> onSQLError;


	private:
		//! Instantiate a new channel
		ChannelPtr createNewChannelWL(uint64 threadid);

	}; // class ConnectorData





} // namespace DBI
} // namespace Private
} // namespace Yuni

# include "connector-data.hxx"

#endif // __YUNI_PRIVATE_DBI_CONNECTOR_DATA_H__
