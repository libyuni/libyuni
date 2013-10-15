#ifndef __YUNI_NET_HOST_ADDRESS_PORT_HXX__
# define __YUNI_NET_HOST_ADDRESS_PORT_HXX__


namespace Yuni
{
namespace Net
{


	inline void HostAddressPort::reset()
	{
		address.clear();
		port = nullptr;
	}


	inline bool HostAddressPort::operator < (const HostAddressPort& rhs) const
	{
		return (port < rhs.port) and (address < rhs.address);
	}




} // namespace Net
} // namespace Yuni

#endif // __YUNI_NET_HOST_ADDRESS_PORT_H__
