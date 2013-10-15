#ifndef __YUNI_DBI_ROW_HXX__
# define __YUNI_DBI_ROW_HXX__



namespace Yuni
{
namespace DBI
{

	inline Row::Row(::yn_dbi_adapter& adapter, void* handle) :
		pAdapter(adapter),
		pHandle(handle)
	{}

	inline Row::Row(const Row& rhs) :
		pAdapter(rhs.pAdapter),
		pHandle(rhs.pHandle)
	{}


	inline Column Row::column(uint nth)
	{
		return Column(pAdapter, pHandle, nth);
	}


	inline Column Row::operator [] (uint nth)
	{
		return Column(pAdapter, pHandle, nth);
	}





} // namespace DBI
} // namespace Yuni

#endif // __YUNI_DBI_ROW_HXX__
