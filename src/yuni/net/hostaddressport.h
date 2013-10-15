#ifndef __YUNI_NET_HOST_ADDRESS_PORT_H__
# define __YUNI_NET_HOST_ADDRESS_PORT_H__

# include "../yuni.h"
# include "port.h"
# include "../core/string.h"
# include <vector>


namespace Yuni
{
namespace Net
{

	//! Raw Address for a host (IP/DNS)
	typedef ShortString256  HostAddress;



	class HostAddressPort
	{
	public:
		//! Vector
		typedef std::vector<HostAddressPort>  Vector;


	public:
		//! Reset the host address and port
		void reset();

	public:
		//! Raw host address (IP/DNS)
		HostAddress  address;
		//! Socket port number
		Port port;

		//! operator <
		bool operator < (const HostAddressPort& rhs) const;

	}; // class HostAddressPort





} // namespace Net
} // namespace Yuni

# include "hostaddressport.hxx"

#endif // __YUNI_NET_HOST_ADDRESS_PORT_H__
