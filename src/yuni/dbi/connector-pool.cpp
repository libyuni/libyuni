/*
** YUNI's default license is the GNU Lesser Public License (LGPL), with some
** exclusions (see below). This basically means that you can get the full source
** code for nothing, so long as you adhere to a few rules.
**
** Under the LGPL you may use YUNI for any purpose you wish, and modify it if you
** require, as long as you:
**
** Pass on the (modified) YUNI source code with your software, with original
** copyrights intact :
**  * If you distribute electronically, the source can be a separate download
**    (either from your own site if you modified YUNI, or to the official YUNI
**    website if you used an unmodified version) – just include a link in your
**    documentation
**  * If you distribute physical media, the YUNI source that you used to build
**    your application should be included on that media
** Make it clear where you have customised it.
**
** In addition to the LGPL license text, the following exceptions / clarifications
** to the LGPL conditions apply to YUNI:
**
**  * Making modifications to YUNI configuration files, build scripts and
**    configuration headers such as yuni/platform.h in order to create a
**    customised build setup of YUNI with the otherwise unmodified source code,
**    does not constitute a derived work
**  * Building against YUNI headers which have inlined code does not constitute a
**    derived work
**  * Code which subclasses YUNI classes outside of the YUNI libraries does not
**    form a derived work
**  * Statically linking the YUNI libraries into a user application does not make
**    the user application a derived work.
**  * Using source code obsfucation on the YUNI source code when distributing it
**    is not permitted.
** As per the terms of the LGPL, a "derived work" is one for which you have to
** distribute source code for, so when the clauses above define something as not
** a derived work, it means you don't have to distribute source code for it.
** However, the original YUNI source code with all modifications must always be
** made available.
*/
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
			// we may have a minor inconsistency here in some heavily multithreaded
			// applications with the value of onSQLError, but it should not matter
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
		// acquiring / swapping the data, to destroy it in the same time
		Yuni::Private::DBI::ConnectorDataPtr data;
		data.swap(pData);

		if (!(!data))
		{
			// try to close as many connections as possible.
			// with a null idle time, the only remaining connections will be
			// still in use, probably by another thread
			uint dummy;
			data->closeTooOldChannels(/*idletime:*/ 0, dummy);
		}
	}


	bool ConnectorPool::retrieveSettings(Settings& out) const
	{
		// acquiring the pointer to avoid race conditions
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
		// acquiring the pointer to avoid race conditions
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
		// acquiring the pointer to avoid race conditions
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

