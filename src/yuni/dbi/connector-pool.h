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
#ifndef __YUNI_DBI_CONNECTOR_H__
# define __YUNI_DBI_CONNECTOR_H__

# include "error.h"
# include "fwd.h"
# include "settings.h"
# include "adapter.h"
# include "../core/event/event.h"



namespace Yuni
{
/*!
** \brief Database Independent Abstraction Layer
** \ingroup DBI
*/
namespace DBI
{


	/*!
	** \brief Database interface
	** \ingroup DBI
	**
	** All methods of this class are thread-safe. This means that manipulating a
	** connector pool from several threads is safe (thus starting new transactions)
	** However, queries and resultset are NOT thread-safe.
	*/
	class ConnectorPool final
	{
	public:
		//! The most suitable smart pointer for the class
		typedef SmartPtr<ConnectorPool>  Ptr;

	public:
		//! \name Constructor & Destructor
		//@{
		//! Default constructor
		ConnectorPool();
		//! Copy constructor
		ConnectorPool(const ConnectorPool& rhs);
		/*!
		** \brief Destructor
		**
		** The destructor will try to close all connections
		** \see close()
		*/
		~ConnectorPool();
		//@}


		//! \name Channel
		//@{
		/*!
		** \brief Try to open a connection to a remote database
		**
		** A real connexion attempt to the database will be performed in order to validate
		** input parameters.
		*/
		Error open(Adapter::IAdapter* adapter, const Settings& settings);

		/*!
		** \brief Try to open a connection to a remote database with default settings
		**
		** A real connexion attempt to the database will be performed in order to validate
		** input parameters.
		**
		** \param adapter Adapter name (sqlite, psql...)
		** \param host    host name (or filename)
		*/
		Error open(Adapter::IAdapter* adapter, AnyString host);

		/*!
		** \brief Try to open a connection to a remote database with default settings
		**
		** A real connexion attempt to the database will be performed in order to validate
		** input parameters.
		**
		** \param adapter Adapter name (sqlite, psql...)
		** \param host    host name (or filename)
		*/
		Error open(Adapter::IAdapter* adapter, AnyString host, AnyString dbname, AnyString user, AnyString password);

		/*!
		** \brief Close all connections to the remote database
		**
		** It is safe to call this method several times. The pool will try to
		** close all connections (all which are not currently in use)
		*/
		void close();
		//@}


		//! \name Transactions
		//@{
		/*!
		** \brief Start a new transaction
		*/
		Transaction begin();

		/*!
		** \brief Start a new transaction which will be automatically commited
		*/
		AutoCommit autocommit();
		//@}


		//! \name Settings
		//@{
		//! Get a copy of the connector settings
		bool retrieveSettings(Settings& out) const;
		//@}


		//! \name Maintenance
		//@{
		/*!
		** \brief Close all connections which are idle for too long
		**
		** Calling this method should not be necessary since there is worker thread
		** dedicated to maintenance
		**
		** \param[out] remainingCount the number of connections which remain active
		** \param[out] closedCount The number of connection which have been closed
		*/
		void closeIdleConnections(uint* remainingCount = nullptr, uint* closedCount = nullptr);

		/*!
		** \brief Close all connections which are idle for too long
		**
		** Calling this method should not be necessary since there is worker thread
		** dedicated to maintenance
		**
		** \param idleTime The maximum idle time in seconds (override the value provided by the connection settings)
		** \param[out] remainingCount the number of connections which remain active
		** \param[out] closedCount The number of connection which have been closed
		*/
		void closeIdleConnections(uint idleTime, uint* remainingCount = nullptr, uint* closedCount = nullptr);
		//@}


		//! \name Operators
		//@{
		//! Operator =
		ConnectorPool& operator = (const ConnectorPool& rhs);
		//@}


	public:
		//! Event trigered when a SQL error occurs
		// \important NOT IMPLEMENTED YET !!!
		Event<void ()> onSQLError;


	private:
		//! Specific connector data
		Yuni::Private::DBI::ConnectorDataPtr pData;

		friend class Transaction;

	}; // class ConnectorPool





} // namespace DBI
} // namespace Yuni

# include "transaction.h"
# include "connector-pool.hxx"

#endif // __YUNI_DBI_CONNECTOR_H__
