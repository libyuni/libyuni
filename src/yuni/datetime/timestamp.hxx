#ifndef __YUNI_DATETIME_TIMESTAMP_HXX__
# define __YUNI_DATETIME_TIMESTAMP_HXX__

# include <cstdlib>



namespace Yuni
{
namespace Private
{
namespace DateTime
{

	// forward declaration
	char* FormatTimestampToString(const AnyString& format, sint64 timestamp);


} // namespace DateTime
} // namespace Private
} // namespace Yuni



namespace Yuni
{
namespace DateTime
{

	template<class StringT>
	bool TimestampToString(StringT& out, const AnyString& format, Timestamp timestamp, bool emptyBefore)
	{
		if (emptyBefore)
			out.clear();
		if (format.empty())
			return true;

		char* buffer = Yuni::Private::DateTime::FormatTimestampToString(format, timestamp);
		if (buffer)
		{
			out += (const char*) buffer;
			::free(buffer);
			return true;
		}
		return false;
	}




} // namespace DateTime
} // namespace Yuni

#endif // __YUNI_DATETIME_TIMESTAMP_H__
