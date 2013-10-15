
#include "errors.h"


namespace Yuni
{
namespace Net
{

	const char* ErrorToCString(Error error)
	{
		switch (error)
		{
			case errNone:
				return "no error";
			case errInvalidPort:
				return "invalid port";
			case errInvalidHostAddress:
				return "invalid host address";
			case errDupplicatedAddress:
				return "address dupplicated";
			case errNoTransport:
				return "no transport available";
			case errInvalidTransport:
				return "invalid transport";
			case errStartFailed:
				return "impossible to start the server";
			case errUnknown:
			case errMax:
				break;
		}
		return "<unknown>";
	}



} // namespace Net
} // namespace Yuni

