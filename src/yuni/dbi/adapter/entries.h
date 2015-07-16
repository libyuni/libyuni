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
#ifndef __YUNI_DBI_ADAPTER_ENTRIES_H__
# define __YUNI_DBI_ADAPTER_ENTRIES_H__

# include "../../yuni.h"
# include "../error.h"


/*!
** \brief Adapter entries table
**
** \note All routines must be reentrant
*/
struct yn_dbi_adapter
{
	//! Database handle, provided for convenient uses
	void* dbh;

	//! Start a new transaction
	yn_dbierr (*begin)(void* dbh);
	//! Commit the current transaction
	yn_dbierr (*commit)(void* dbh);
	//! Create a new savepoint in the current transaction
	yn_dbierr (*savepoint)(void* dbh, const char* name, uint length);
	//! Create a new savepoint in the current transaction
	yn_dbierr (*commit_savepoint)(void* dbh, const char* name, uint length);
	//! Rollback the current transaction
	yn_dbierr (*rollback)(void* dbh);
	//! Rollback a savepoint
	yn_dbierr (*rollback_savepoint)(void* dbh, const char* name, uint length);

	//! Execute a simple query
	yn_dbierr (*query_exec)(void* dbh, const char* stmt, uint length);
	//! Start a new query (and acquire it)
	yn_dbierr (*query_new)(void** qh, void* dbh, const char* stmt, uint length);
	//! Acquire a query pointer
	void (*query_ref_acquire)(void* qh);
	//! Release a query
	void (*query_ref_release)(void* qh);
	//! Bind a string
	yn_dbierr (*bind_str)(void* qh, uint index, const char* str, uint length);
	//! Bind a bool
	yn_dbierr (*bind_bool)(void* qh, uint index, int value);
	//! Bind a sint32
	yn_dbierr (*bind_int32)(void* qh, uint index, yint32 value);
	//! Bind a sint64
	yn_dbierr (*bind_int64)(void* qh, uint index, yint64 value);
	//! Bind a double
	yn_dbierr (*bind_double)(void* qh, uint index, double value);
	//! Bind a null value
	yn_dbierr (*bind_null)(void* qh, uint index);

	//! Execute a query
	yn_dbierr (*query_execute)(void* qh);
	//! Execute a query, and release the handler
	yn_dbierr (*query_perform_and_release)(void* qh);

	//! Go to the next row
	yn_dbierr (*cursor_go_to_next)(void* qh);
	//! Go to the previous row
	yn_dbierr (*cursor_go_to_previous)(void* qh);
	//! Go to a specific row
	yn_dbierr (*cursor_go_to)(void* qh, yuint64 rowindex);

	//! Get the value as an int32 of a specific column for the current row
	yint32 (*column_to_int32)(void* qh, uint colindex);
	//! Get the value as an int64 of a specific column for the current row
	yint64 (*column_to_int64)(void* qh, uint colindex);
	//! Get the value as a double of a specific column for the current row
	double (*column_to_double)(void* qh, uint colindex);
	//! Get the value as a string of a specific column for the current row
	const char* (*column_to_cstring)(void* qh, uint colindex, uint* length);
	//! Get whether a column is null or not
	int (*column_is_null)(void* qh, uint colindex);

	//! garbage-collect and optionally analyze a database
	yn_dbierr (*vacuum)(void* dbh);
	//! truncate a table
	yn_dbierr (*truncate)(void* dbh, const char* tablename, uint length);

	//! Open a connection to the remote database
	yn_dbierr (*open) (void** dbh, const char* host, uint port, const char* user, const char* pass, const char* dbname);
	//! Open a schema
	yn_dbierr (*open_schema) (void* dbh, const char* name, uint length);
	//! Close the connection
	void (*close) (void* dbh);

}; // class Adapter




#endif /* __YUNI_DBI_ADAPTER_ENTRIES_H__ */
