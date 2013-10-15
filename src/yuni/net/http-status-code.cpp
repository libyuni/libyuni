
#include "http-status-code.h"


namespace Yuni
{
namespace Net
{

	const char* HttpStatusCodeToCString(uint code)
	{
		switch (code)
		{
			// 2xx
			case 200: return "OK";
			case 201: return "CREATED";
			case 202: return "Accepted";
			case 203: return "Partial information";
			case 204: return "No response";
			// 4xx
			case 400: return "Bad request";
			case 401: return "Unauthorized";
			case 402: return "PaymentRequired";
			case 403: return "Forbidden";
			case 404: return "Not found";
			// 5xx
			case 500: return "Internal Error";
		}
		return nullptr;
	}





} // namespace Net
} // namespace Yuni

