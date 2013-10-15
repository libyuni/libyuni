#ifndef __YUNI_NET_HTTP_STATUS_CODE_H__
# define __YUNI_NET_HTTP_STATUS_CODE_H__

# include "../yuni.h"


namespace Yuni
{
namespace Net
{

	/*!
	** \brief Convert an http status code into a human-readable diagnostic information
	**
	** \code
	** std::cout << Net::HttpStatusCodeToCString(500) << std::endl; // Internal error
	** \endcode
	*/
	const char* HttpStatusCodeToCString(uint code);




} // namespace Net
} // namespace Yuni

#endif // __YUNI_NET_HTTP_STATUS_CODE_H__
