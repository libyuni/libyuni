#ifndef __YUNI_DBI_TRANSACTION_H__
# define __YUNI_DBI_TRANSACTION_H__

# include <yuni/yuni.h>
# include <yuni/core/noncopyable.h>
# include "fwd.h"
# include "cursor.h"
# include "error.h"


namespace Yuni
{
namespace DBI
{

	/*!
	** \brief Transaction
	**
	** \code
	**	auto tx = dbconnexion.begin();
	**	auto query = tx.query("SELECT * FROM data WHERE year >= ? AND year <= ?");
	**	query.map(2010, 2042);
	**	if (DBI::errNone != query.each([] (const DBI::Row& row)
	**	{
	**		return true;
	**	})
	**	{
	**
	**	}
	** \endcode
	*/
	class Transaction final : private NonCopyable<Transaction>
	{
	public:
		//! \name Constructors & Destructor
		//@{
		/*!
		** \brief Move constructor
		*/
		Transaction(Transaction&& other);
		//! Destructor
		~Transaction();
		//@}


		//! \name Actions
		//@{
		/*!
		** \brief Try to commit the current transaction
		*/
		DBI::Error commit();

		/*!
		** \brief Rollback the transaction
		*/
		DBI::Error rollback();

		/*!
		** \brief re-start with a new transaction
		**
		** If a transaction was already taking place, it will be rolled back
		*/
		DBI::Error rebegin();
		//@}


		//! \name SQL queries
		//@{
		/*!
		** \brief Create a new query
		*/
		Cursor prepare(const AnyString& stmt);

		/*!
		** \brief Perform a query and discard the resultset
		*/
		DBI::Error perform(const AnyString& script);
		//@}


		//! \name Convenient routines
		//@{
		/*!
		** \brief Truncate a table
		**
		** The real SQL query is adapter-dependent.
		** This method should be prefered instead of using the
		** SQL query `truncate <tablename>`, which may not exist.
		*/
		DBI::Error truncate(const AnyString& tablename);

		/*!
		** \brief Garbage-collect and optionally analyze a database
		**
		** The real SQL query is adapter-dependent.
		*/
		DBI::Error vacuum();
		//@}


		//! \name Connector
		//@{
		//! Retrieve a connector from the current transaction
		//Connector connector() const;
		//@}


		//! \name Operators
		//@{
		//! operator (), equivalent to query()
		Cursor operator () (const AnyString& stmt);
		//@}


	protected:
		//! Constructor from a connector
		explicit Transaction(Yuni::Private::DBI::ConnectorDataPtr& data);
		//! Constructor from a channel
		explicit Transaction(Yuni::Private::DBI::ChannelPtr& data);

	private:
		//! Communication channel with the remote database
		Yuni::Private::DBI::ChannelPtr pChannel;
		//! Transaction handle
		uint pTxHandle;
		// friends
		friend class ConnectorPool;

	}; // class Transaction





} // namespace DBI
} // namespace Yuni

# include "auto-commit.h"
# include "transaction.hxx"

#endif // __YUNI_DBI_TRANSACTION_H__
