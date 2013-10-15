
#include "threadcontext.h"
#include "../net/http-status-code.h"


namespace Yuni
{
namespace Messaging
{

	Context::Context(Service& service) :
		userdata(NULL),
		service(service),
		httpStatusCode()
	{
		HttpStatusCode& codes = const_cast<HttpStatusCode&>(httpStatusCode);

		for (uint i = 200; i <= codes.max2xx; ++i)
			codes.header2xx[i - 200] << "HTTP/1.1 " << i << ' ' << Net::HttpStatusCodeToCString(i) << "\r\n";

		for (uint i = 400; i <= codes.max4xx; ++i)
		{
			codes.header4xx[i - 400] << "HTTP/1.1 " << i << ' '
				<< Net::HttpStatusCodeToCString(i) << "\r\n";

			codes.response4xx[i - 400] << codes.header4xx[i - 400]
				<< "\r\nCache: no-cache\r\n\r\n"
				<< Net::HttpStatusCodeToCString(i) << "\r\n";
		}
		for (uint i = 500; i <= codes.max5xx; ++i)
		{
			codes.header5xx[i - 500] << "HTTP/1.1 " << i << ' '
				<< Net::HttpStatusCodeToCString(i) << "\r\n";

			codes.response5xx[i - 500] << codes.header5xx[i - 500]
				<< "\r\nCache: no-cache\r\n\r\n"
				<< Net::HttpStatusCodeToCString(i) << "\r\n";
		}
	}





} // namespace Messaging
} // namespace Yuni

