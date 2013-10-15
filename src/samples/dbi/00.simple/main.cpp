
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

