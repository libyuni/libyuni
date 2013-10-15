#ifndef __YUNI_DBI_ROW_H__
# define __YUNI_DBI_ROW_H__

# include "column.h"


namespace Yuni
{
namespace DBI
{

	class Row final
	{
	public:
		//! \name Constructors
		//@{
		//! Default constructor
		explicit Row(::yn_dbi_adapter& adapter, void* handle);
		//! Copy constructor
		Row(const Row& rhs);
		//@}


		//! \name Columns
		//@{
		//! Get the Nth column
		Column column(uint nth);
		//@}


		//! \name Operators
		//@{
		//! Get the Nth column
		Column operator [] (uint nth);
		//@}


	private:
		//! Alias to the current channel
		// \internal This reference can be null and must never be called if pHandle is null
		::yn_dbi_adapter& pAdapter;
		//! Opaque pointer to the current query
		void* pHandle;

	}; // class Row





} // namespace DBI
} // namespace Yuni

# include "row.hxx"

#endif // __YUNI_DBI_ROW_H__
