#ifndef __YUNI_NET_ERRORS_H__
# define __YUNI_NET_ERRORS_H__

# include "../yuni.h"
# include "../core/string.h"


namespace Yuni
{
namespace Net
{

	/*!
	** \brief Errors that can be returned by routines in the \p 'Yuni::Net' module
	*/
	enum Error
	{
		//! No error
		errNone = 0,
		//! Unknown error
		errUnknown,
		//! The port is invalid (range [1..65535])
		errInvalidPort,
		//! The address is invalid
		errInvalidHostAddress,
		//! The given address already exists in the list
		errDupplicatedAddress,
		//! No transport layer
		errNoTransport,
		//! Invalid transport layer
		errInvalidTransport,
		//! Impossible to start web server
		errStartFailed,
		//! The maximum number of errors
		errMax
	};



	/*!
	** \brief Convert an error to a human readable string
	**
	** For standard uses, you would prefer using Yuni::String
	*/
	const char* ErrorToCString(Error error);


} // namespace Net
} // namespace Yuni




namespace Yuni
{
namespace Extension
{
namespace CString
{

	template<class CStringT>
	class Append<CStringT, Yuni::Net::Error>
	{
	public:
		static inline void Perform(CStringT& string, Yuni::Net::Error error)
		{
			string += Yuni::Net::ErrorToCString(error);
		}
	};



} // namespace CString
} // namespace Extension
} // namespace Yuni


#endif // __YUNI_NET_ERRORS_H__
