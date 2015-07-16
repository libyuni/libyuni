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
#include "../../yuni.h"
#include "../../core/dictionary.h"
#include "../../datetime/timestamp.h"
#include "../../core/noncopyable.h"
#include "../../dbi/settings.h"
#include "../../dbi/error.h"
#include "../../dbi/adapter/entries.h"

#ifdef YUNI_OS_32
#	define YUNI_PRIVATE_DBI_ATOMIC_INT  32
#else
#	define YUNI_PRIVATE_DBI_ATOMIC_INT  64
#endif



namespace Yuni
{
namespace Private
{
namespace DBI
{

	//! Error
	typedef Yuni::DBI::Error  Error;




	/*!
	** \brief Single connection to a remote database
	*/
	class Channel final : private Yuni::NonCopyable<Channel>
	{
	public:
		//! The most suitable smart pointer
		typedef Yuni::SmartPtr<Channel> Ptr;
		//! Channel table, per thread-pseudo id
		typedef Dictionary<uint64, Ptr>::Hash  Table;


	public:
		//! \name Constructor & Destructor
		//@{
		Channel(const Yuni::DBI::Settings& settings, const ::yn_dbi_adapter& adapter);
		//! Destructor
		~Channel();
		//@}


		//! \name Connection
		//@{
		/*!
		** \brief Open the connection
		** \see status
		*/
		void open();
		//@}


		//! \name Transactions
		//@{
		//! Start a new transaction
		Error begin(uint& handle);

		//! Try to commit the transaction
		Error commit(uint handle);

		//! Rollback a transaction
		Error rollback(uint handle);
		//@}


	public:
		//! Mutex, locked/released by transaction
		Yuni::Mutex mutex;
		//! Status of the connection
		Error status;
		//! Adapter
		::yn_dbi_adapter adapter;
		//! The total number of nested transactions
		uint nestedTransactionCount;
		//! Channel settings
		Yuni::DBI::Settings settings;

		//! Timestamp when the channel was last used
		Atomic::Int<YUNI_PRIVATE_DBI_ATOMIC_INT> lastUsed;

	}; // class Channel





} // namespace DBI
} // namespace Private
} // namespace Yuni

#include "channel.hxx"
