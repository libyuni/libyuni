#ifndef __YUNI_DATETIME_TIMESTAMP_H__
# define __YUNI_DATETIME_TIMESTAMP_H__

# include "../yuni.h"
# include "../core/string.h"


namespace Yuni
{
namespace DateTime
{

	/*!
	** \brief Unix Timestamp
	**
	** A timestamp is a time in seconds since 0 hours, 0 minutes, 0 seconds,
	** January 1, 1970, Coordinated Universal Time.
	*/
	typedef /*time_t*/ sint64  Timestamp;


	/*!
	** \brief Get the current timestamp
	*/
	Timestamp Now();


	/*!
	** \brief Convert an Unix timestamp to a string
	**
	** \code
	** String s;
	** DateTime::TimestampToString(s, "%D");
	** \endcode
	**
	** \param[out] out The string where the result will be written
	** \param format Format (man strftime). The string must be zero-terminated
	** \param timestamp An Unix Timestamp (0 or negative value will be replaced by the current timestamp)
	** \param emptyBefore True to empty the string before anything else
	** \return True if the operation succeded, false otherwise
	*/
	template<class StringT>
	bool TimestampToString(StringT& out, const AnyString& format, Timestamp timestamp = 0, bool emptyBefore = true);




} // namespace DateTime
} // namespace Yuni

# include "timestamp.hxx"

#endif // __YUNI_DATETIME_TIMESTAMP_H__
