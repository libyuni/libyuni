#ifndef __YUNI_DBI_CONNECTOR_HXX__
# define __YUNI_DBI_CONNECTOR_HXX__



namespace Yuni
{
namespace DBI
{

	inline Transaction  ConnectorPool::begin()
	{
		return Transaction(pData);
	}


	inline Error ConnectorPool::open(Adapter::IAdapter* adapter, AnyString host)
	{
		Settings settings;
		settings.host    = host;
		return open(adapter, settings);
	}


	inline Error ConnectorPool::open(Adapter::IAdapter* adapter, AnyString host, AnyString dbname, AnyString user, AnyString password)
	{
		Settings settings;
		settings.host     = host;
		settings.database = dbname;
		settings.username = user;
		settings.password = password;
		return open(adapter, settings);
	}


	inline AutoCommit ConnectorPool::autocommit()
	{
		return AutoCommit(*this);
	}





} // namespace DBI
} // namespace Yuni

#endif // __YUNI_DBI_CONNECTOR_HXX__
