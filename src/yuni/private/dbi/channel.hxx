#ifndef __YUNI_PRIVATE_DBI_CHANNEL_HXX__
# define __YUNI_PRIVATE_DBI_CHANNEL_HXX__



namespace Yuni
{
namespace Private
{
namespace DBI
{

	enum
	{
		//! Null handle
		nullHandle = 0 // Yuni::DBI::Transaction::nullHandle,
	};


	inline void Channel::open()
	{
		// The inner mutex must be locked here

		if (YUNI_UNLIKELY(not adapter.open or not adapter.close))
		{
			adapter.dbh = nullptr;
			status = Yuni::DBI::errInvalidAdapter;
		}
		else
		{
			if (adapter.dbh)
			{
				// a communication channel is already opened. Closing it.
				adapter.close(adapter.dbh);
			}

			// try to open a new communication channel
			auto error = (Error) adapter.open(&adapter.dbh,
				settings.host.c_str(),      // host
				settings.port,              // port
				settings.username.c_str(),  // username
				settings.password.c_str(),  // password
				settings.database.c_str()   // database name
			);

			if (error != Yuni::DBI::errNone)
			{
				adapter.dbh = nullptr;
				status = error;
				return;
			}

			// ok, good to go
			status = Yuni::DBI::errNone;
		}
	}


	inline Error Channel::begin(uint& handle)
	{
		using namespace Yuni::DBI;

		if (status != errNone)
		{
			// no connection to the remote database - retrying to connect
			open();

			if (status != errNone)
			{
				handle = nullHandle;
				return status;
			}
		}

		if (YUNI_LIKELY(0 == nestedTransactionCount++))
		{
			// No transaction available - creating a new one
			auto error = (Error) adapter.begin(adapter.dbh);
			if (error != errNone)
			{
				--nestedTransactionCount;
				handle = nullHandle;
				return error;
			}
		}
		else
		{
			// A transaction is already present - starting a savepoint
			ShortString64 spname("__yn__sp_");
			spname <<  nestedTransactionCount;
			auto error = (Error) adapter.savepoint(adapter.dbh, spname.c_str(), spname.size());
			if (error != errNone)
			{
				--nestedTransactionCount;
				handle = nullHandle;
				return error;
			}
		}

		// The transaction handle will be the number of transaction
		handle = nestedTransactionCount;
		// no error
		return errNone;
	}


	inline Error Channel::rollback(uint handle)
	{
		assert(handle != 0 and "invalid handle");

		if (YUNI_UNLIKELY(handle != nestedTransactionCount))
		{
			assert(false and "invalid transaction lifetime rollback");
			return Yuni::DBI::errInvalidNestedTransaction;
		}

		if (YUNI_LIKELY(0 == --nestedTransactionCount))
		{
			// rollback the transaction
			return (status = (Error) adapter.rollback(adapter.dbh));
		}
		else
		{
			// rollbacking a savepoint
			ShortString64 spname("__yn__sp_");
			spname <<  (nestedTransactionCount + 1);
			return (status = (Error) adapter.rollback_savepoint(adapter.dbh, spname.c_str(), spname.size()));
		}
	}


	inline Error Channel::commit(uint handle)
	{
		assert(handle != 0 and "invalid handle");

		if (YUNI_UNLIKELY(handle != nestedTransactionCount))
		{
			std::cout << "\nhandle : " << handle << " / " << nestedTransactionCount << std::endl;
			std::cerr << "\ninvalid transaction lifetime rollback" << std::endl;

			std::cerr << "\ninvalid transaction lifetime commit" << std::endl;
			assert(false and "invalid transaction lifetime commit");
			return Yuni::DBI::errInvalidNestedTransaction;
		}

		if (YUNI_LIKELY(0 == --nestedTransactionCount))
		{
			// committing the transaction
			return (status = (Error) adapter.commit(adapter.dbh));
		}
		else
		{
			// committing a savepoint
			ShortString64 spname("__yn__sp_");
			spname <<  (nestedTransactionCount + 1);
			return (status = (Error) adapter.commit_savepoint(adapter.dbh, spname.c_str(), spname.size()));
		}
	}






} // namespace DBI
} // namespace Private
} // namespace Yuni

#endif // __YUNI_PRIVATE_DBI_CHANNEL_HXX__
