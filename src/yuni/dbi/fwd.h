#ifndef __YUNI_DBI_FWD_H__
# define __YUNI_DBI_FWD_H__

# include "../core/smartptr/smartptr.h"


namespace Yuni
{
namespace DBI
{

	enum
	{
		//! Invalid transaction handle
		nullHandle = 0
	};



	// Forward declarations
	class ConnectorPool;
	class Transaction;
	class Query;
	class AutoCommit;
	class Row;
	class Column;


} // namespace DBI
} // namespace Yuni



namespace Yuni
{
namespace Private
{
namespace DBI
{

	// Forward declarations
	class Channel;
	class ConnectorData;

	//! Connector data ptr
	typedef Yuni::SmartPtr<ConnectorData>  ConnectorDataPtr;
	typedef Yuni::SmartPtr<Channel>  ChannelPtr;


} // namespace DBI
} // namespace Private
} // namespace Yuni


#endif // __YUNI_DBI_FWD_H__
