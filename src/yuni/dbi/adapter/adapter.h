#ifndef __YUNI_DBI_ADAPTER_ADAPTER_H__
# define __YUNI_DBI_ADAPTER_ADAPTER_H__

# include "entries.h"


namespace Yuni
{
namespace DBI
{
namespace Adapter
{

	class IAdapter
	{
	public:
		//! \name Default constructor
		//@[
		//! Default constructor
		IAdapter() {}
		//! Destructor
		virtual ~IAdapter() {}
		//@}

		//! \name Entries
		//@{
		//! Retrieve the entry table for the adapter
		virtual void retrieveEntries(::yn_dbi_adapter& entries) = 0;
		//@}

	}; // class IAdapter




} // namespace Adapter
} // namespace DBI
} // namespace Yuni

#endif // __YUNI_DBI_ADAPTER_ADAPTER_H__
