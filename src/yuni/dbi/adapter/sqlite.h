#ifndef __YUNI_DBI_ADAPTER_SQLITE_H__
# define __YUNI_DBI_ADAPTER_SQLITE_H__

# include "adapter.h"


namespace Yuni
{
namespace DBI
{
namespace Adapter
{

	class SQLite final : public IAdapter
	{
	public:
		SQLite() {}
		virtual ~SQLite() {}

		virtual void retrieveEntries(::yn_dbi_adapter& entries) override;

	}; // class SQLite




} // namespace Adapter
} // namespace DBI
} // namespace Yuni

#endif // __YUNI_DBI_ADAPTER_SQLITE_H__
