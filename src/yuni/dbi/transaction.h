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
#pragma once
#include <yuni/yuni.h>
#include <yuni/core/noncopyable.h>
#include "fwd.h"
#include "cursor.h"
#include "error.h"



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

		//! Perform a query and discard the resultset with one bound parameter
		template<class A1>
		DBI::Error perform(const AnyString& script, const A1& a1);

		//! Perform a query and discard the resultset with 2 parameters
		template<class A1, class A2>
		DBI::Error perform(const AnyString& script, const A1& a1, const A2& a2);

		//! Perform a query and discard the resultset with 3 parameters
		template<class A1, class A2, class A3>
		DBI::Error perform(const AnyString& script, const A1& a1, const A2& a2, const A3& a3);

		//! Perform a query and discard the resultset with 4 parameters
		template<class A1, class A2, class A3, class A4>
		DBI::Error perform(const AnyString& script, const A1& a1, const A2& a2, const A3& a3, const A4& a4);

		//! Iterate over all rows of the resultset of a query (without any parameter)
		template<class CallbackT>
		DBI::Error each(const AnyString& query, const CallbackT& callback);

		//! Iterate over all rows of the resultset of a query with 1 parameter
		template<class CallbackT, class A1>
		DBI::Error each(const AnyString& query, const A1& a1, const CallbackT& callback);

		//! Iterate over all rows of the resultset of a query with 2 parameters
		template<class CallbackT, class A1, class A2>
		DBI::Error each(const AnyString& query, const A1& a1, const A2& a2, const CallbackT& callback);

		//! Iterate over all rows of the resultset of a query with 3 parameters
		template<class CallbackT, class A1, class A2, class A3>
		DBI::Error each(const AnyString& query, const A1& a1, const A2& a2, const A3& a3, const CallbackT& callback);

		//! Iterate over all rows of the resultset of a query with 4 parameters
		template<class CallbackT, class A1, class A2, class A3, class A4>
		DBI::Error each(const AnyString& query, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const CallbackT& callback);
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

