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
#include <yuni/yuni.h>
#include <yuni/core/logs.h>
#include <yuni/dbi/dbi.h>
#include <yuni/dbi/adapter/sqlite.h>

using namespace Yuni;

//! Logging facility
Logs::Logger<> logs;



static bool RunExample()
{
	// our connection pool
	DBI::ConnectorPool connectors;

	// database location
	logs.info() << "opening the database file";
	auto error = connectors.open(new DBI::Adapter::SQLite(), "dbi-example.db");
	if (error)
	{
		logs.error() << "impossible to establish a connection to the database";
		return false;
	}

	// begin a new transaction
	auto tx = connectors.begin();

	// create a new table if not already exists
	logs.info() << "creating the table 'example'";
	tx.perform("CREATE TABLE IF NOT EXISTS example (x INTEGER PRIMARY KEY);");

	// truncate the table, if it already exists
	logs.info() << "truncating the table";
	tx.truncate("example");

	// fill it with some values
	logs.info() << "inserting some data";
	tx.perform("INSERT INTO example (x) VALUES (42), (3), (-1), (-10), (-3);");

	// iterating through a result set
	{
		uint rowindex = 0;

		logs.info();
		logs.info() << "iterating through all values";
		auto stmt = tx.prepare("SELECT * FROM example WHERE x > $1 ORDER BY x");
		stmt.bind(0, -5); // x > -5
		stmt.execute();

		// iterate through all values
		stmt.each([&] (DBI::Row& row) -> bool
		{
			sint64 x = row[0].asInt64();
			logs.info() << "  :: resultset: in row " << (++rowindex) << ":  x = " << x;
			return true; // continue
		});
	}

	// commit changes
	logs.info() << "commit";
	if (DBI::errNone != (error = tx.commit()))
	{
		logs.error() << "commit failed";
		return false;
	}

	return true;
}



int main()
{
	// Welcome !
	logs.notice() << "Yuni DBI Example";
	logs.info() << "using the SQLite default adapter";
	logs.info(); // empty line

	return RunExample() ? 0 : EXIT_FAILURE;
}

