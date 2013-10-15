#ifndef __YUNI_MESSAGE_API_PROTOCOL_HXX__
# define __YUNI_MESSAGE_API_PROTOCOL_HXX__


namespace Yuni
{
namespace Messaging
{
namespace API
{

	inline void Methods::clear()
	{
		pMethods.clear();
	}


	inline const Method::Hash& Methods::all() const
	{
		return pMethods;
	}


	inline void Methods::shrinkMemory()
	{
	}




} // namespace API
} // namespace Messaging
} // namespace Yuni

#endif // __YUNI_MESSAGE_API_PROTOCOL_HXX__
