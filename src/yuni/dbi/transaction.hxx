#ifndef __YUNI_DBI_TRANSACTION_HiXX__
# define __YUNI_DBI_TRANSACTION_HXX__


namespace Yuni
{
namespace DBI
{


	inline Cursor Transaction::operator () (const AnyString& stmt)
	{
		return prepare(stmt);
	}






} // namespace DBI
} // namespace Yuni

#endif // __YUNI_DBI_TRANSACTION_HXX__
