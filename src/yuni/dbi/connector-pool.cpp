
#include "connector-pool.h"
#include "../private/dbi/connector-data.h"



namespace Yuni
{
namespace DBI
{

	ConnectorPool::ConnectorPool(const ConnectorPool& rhs) :
		pData(rhs.pData)
	{}


	ConnectorPool& ConnectorPool::operator = (const ConnectorPool& rhs)
	{
		pData = rhs.pData;
		return *this;
	}


	ConnectorPool::ConnectorPool()
	{
		// does nothing
	}


	ConnectorPool::~ConnectorPool()
	{
		close();
	}


	Error ConnectorPool::open(Adapter::IAdapter* adapter, const Settings& settings)
	{
		// close all existing connectionss if possible
		close();

		// the adapter must be set
		if (not adapter)
			return errInvalidAdapter;

		// creating the new connector data
		auto* data = new Yuni::Private::DBI::ConnectorData(settings, adapter);

		// attempt to open a new channel to the remote database
		// from the current thread in order to check the connection settings
		auto channel = data->openChannel();

		// should never happen, but we never know
		assert(!(!channel) and "invalid channel");

		// checking the channel status
		MutexLocker locker(channel->mutex);
		if (errNone == channel->status)
		{
			// the channel has been successfully opened

			// retrieving all events
			// we may have an inconsistency here in some heavily multithreaded applications
			// but it should not matter
			// (event is thread-safe of course)
			data->onSQLError = onSQLError;

			// installing the new connector data
			pData = data;
			return errNone;
		}
		else
		{
			// the channel could not be opened, aborting
			delete data;
			return channel->status;
		}
	}


	void ConnectorPool::close()
	{
		// switching the connector data
		Yuni::Private::DBI::ConnectorDataPtr data;
		data.swap(pData);

		if (!(!data))
		{
			// The connector data may be shared by several instance of
			// the class `ConnectorPool`. We should close the channels
			// only if this is the last one
			if (data.unique())
			{
				// try to close as many connections as possible.
				// with a null idle time, the only remaining connections will be
				// still in use, probably by another thread
				uint dummy;
				data->closeTooOldChannels(/*idletime:*/0, dummy);
			}
		}
	}


	bool ConnectorPool::retrieveSettings(Settings& out) const
	{
		Yuni::Private::DBI::ConnectorDataPtr data = pData;
		if (!(!data))
		{
			out = data->settings;
			return true;
		}
		else
		{
			out.clear();
			return false;
		}
	}


	void ConnectorPool::closeIdleConnections(uint* remainingCount, uint* closedCount)
	{
		Yuni::Private::DBI::ConnectorDataPtr data = pData;
		if (!(!data))
		{
			// idle time, from the settings
			uint idletime = data->settings.idleTime;

			// stats
			uint statsRemain = 0;
			uint statsClosed;

			// closing idle connections
			statsClosed = data->closeTooOldChannels(idletime, statsRemain);

			// reporting
			if (remainingCount)
				*remainingCount = statsRemain;
			if (closedCount)
				*closedCount = statsClosed;
		}
		else
		{
			// no connector data, no connections
			if (remainingCount)
				*remainingCount = 0;
			if (closedCount)
				*closedCount = 0;
		}
	}


	void ConnectorPool::closeIdleConnections(uint idleTime, uint* remainingCount, uint* closedCount)
	{
		Yuni::Private::DBI::ConnectorDataPtr data = pData;
		if (!(!data))
		{
			// stats
			uint statsRemain = 0;
			uint statsClosed;

			// closing idle connections
			statsClosed = data->closeTooOldChannels(idleTime, statsRemain);

			// reporting
			if (remainingCount)
				*remainingCount = statsRemain;
			if (closedCount)
				*closedCount = statsClosed;
		}
		else
		{
			// no connector data, no connections
			if (remainingCount)
				*remainingCount = 0;
			if (closedCount)
				*closedCount = 0;
		}
	}





} // namespace DBI
} // namespace Yuni

