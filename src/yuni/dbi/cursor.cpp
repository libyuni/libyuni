
#include "cursor.h"
#include <cassert>


namespace Yuni
{
namespace DBI
{


	DBI::Error Cursor::execute()
	{
		if (pHandle)
		{
			assert(pAdapter.query_execute and "invalid query_execute");
			return (DBI::Error) pAdapter.query_execute(pHandle);
		}

		return DBI::errNoTransaction;
	}


	DBI::Error Cursor::perform()
	{
		if (pHandle)
		{
			assert(pAdapter.query_perform_and_release and "invalid query_perform_and_release");

			// execute the query
			DBI::Error error = (DBI::Error) pAdapter.query_perform_and_release(pHandle);
			// we must consider that the query has been released
			pHandle = nullptr;
			return error;
		}
		return errNoQuery;
	}





} // namespace DBI
} // namespace Yuni
