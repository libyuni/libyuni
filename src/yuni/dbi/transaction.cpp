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
#include "connector-pool.h" // must be included instead of transaction.h for AutoCommit
#include "connector-pool.h"
#include "../private/dbi/connector-data.h"
#include "../private/dbi/channel.h"
#include <cassert>
#include "utils.h"



namespace Yuni
{
namespace DBI
{

	Transaction::Transaction(Transaction&& other) :
		pTxHandle(other.pTxHandle)
	{
		pChannel.swap(other.pChannel);
		other.pTxHandle = nullHandle;
	}


	Transaction::Transaction(Yuni::Private::DBI::ConnectorDataPtr& data) :
		pTxHandle(nullHandle)
	{
		// retrieving or opening a channel to the remote database
		pChannel = data->openChannel();
		assert(!(!pChannel));
		// avoid concurrent access to this channel
		pChannel->mutex.lock();
	}


	Transaction::Transaction(Yuni::Private::DBI::ChannelPtr& channel) :
		pChannel(channel),
		pTxHandle(nullHandle)
	{
		assert(!(!pChannel) and "null pointer to channel");
		pChannel->mutex.lock();
	}


	Transaction::~Transaction()
	{
		// the channel may have been reset by the move constructor
		if (!(!pChannel))
		{
			// automatic rollback, if not already commited
			if (pTxHandle != nullHandle)
				pChannel->rollback(pTxHandle);

			// allow concurrent access to the channel
			pChannel->mutex.unlock();
		}
	}


	DBI::Error Transaction::rebegin()
	{
		assert(!(!pChannel));

		// rollback if any transaction were already taking place
		if (pTxHandle != nullHandle)
		{
			assert(pChannel and "Invalid channel but valid tx handle");
			pChannel->rollback(pTxHandle);
		}

		// Starting a new transaction
		return pChannel->begin(pTxHandle);
	}


	DBI::Error Transaction::commit()
	{
		if (pTxHandle != nullHandle)
		{
			assert(pChannel and "Invalid channel but valid tx handle");
			Error err = pChannel->commit(pTxHandle);
			pTxHandle = nullHandle;
			return err;
		}
		return errNone;
	}


	DBI::Error Transaction::rollback()
	{
		if (pTxHandle != nullHandle)
		{
			assert(pChannel and "Invalid channel but valid tx handle");
			Error err = pChannel->rollback(pTxHandle);
			// reset
			pTxHandle = nullHandle;
			return err;
		}
		return errNone;
	}


	Cursor Transaction::prepare(const AnyString& stmt)
	{
		assert(!(!pChannel));

		// the adapter
		::yn_dbi_adapter& adapter = pChannel->adapter;

		if (YUNI_UNLIKELY(nullHandle == pTxHandle))
		{
			if (errNone != pChannel->begin(pTxHandle))
				return Cursor(adapter, nullptr);
		}

		// query handle
		void* handle = nullptr;

		if (YUNI_LIKELY(not stmt.empty() and adapter.dbh))
		{
			assert(adapter.query_new != NULL  and "invalid adapter query_new");
			assert(adapter.query_ref_acquire != NULL and "invalid adapter query_ref_acquire");
			assert(adapter.query_ref_release != NULL and "invalid adapter query_ref_release");

			adapter.query_new(&handle, adapter.dbh, stmt.c_str(), stmt.size());
		}

		return Cursor(adapter, handle);
	}


	DBI::Error Transaction::perform(const AnyString& script)
	{
		assert(!(!pChannel));

		// the adapter
		::yn_dbi_adapter& adapter = pChannel->adapter;
		assert(adapter.query_exec != NULL);

		if (YUNI_LIKELY(nullHandle != pTxHandle))
		{
			return (DBI::Error) adapter.query_exec(adapter.dbh, script.c_str(), script.size());
		}
		else
		{
			// start a new transaction
			DBI::Error error = pChannel->begin(pTxHandle);

			if (YUNI_LIKELY(error == errNone))
				error = (DBI::Error) adapter.query_exec(adapter.dbh, script.c_str(), script.size());

			return error;
		}
	}


	DBI::Error Transaction::truncate(const AnyString& tablename)
	{
		if (YUNI_UNLIKELY(not IsValidIdentifier(tablename)))
			return errInvalidIdentifier;

		assert(!(!pChannel));

		// the adapter
		::yn_dbi_adapter& adapter = pChannel->adapter;

		// The DBI interface should provide the most appropriate way for
		// truncating a table (autoincrement / cascade...)
		if (YUNI_LIKELY(adapter.truncate))
		{
			return (DBI::Error) adapter.truncate(adapter.dbh, tablename.c_str(), tablename.size());
		}
		else
		{
			// Fallback to a failsafe method
			// -- stmt << "TRUNCATE " << tablename << ';';
			// The SQL command Truncate is not supported by all databases. `DELETE FROM`
			// is not  the most efficient way for truncating a table
			// but it should work almost everywhere
			String stmt;
			stmt << "DELETE FROM " << tablename << ';';
			return perform(stmt);
		}
	}


	DBI::Error Transaction::vacuum()
	{
		assert(!(!pChannel));
		// the adapter
		::yn_dbi_adapter& adapter = pChannel->adapter;

		if (YUNI_LIKELY(adapter.vacuum))
		{
			return (DBI::Error) adapter.vacuum(adapter.dbh);
		}
		else
		{
			// Fallback to the standard SQL command
			return perform("VACUUM");
		}
	}





} // namespace DBI
} // namespace Yuni

