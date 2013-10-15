#ifndef __YUNI_MESSAGING_PROTOCOL_HXX__
# define __YUNI_MESSAGING_PROTOCOL_HXX__


namespace Yuni
{
namespace Messaging
{

	inline Schema& Protocol::schema()
	{
		pTmp.clear();
		return pSchemas[pTmp];
	}


	inline Schema& Protocol::schema(const AnyString& name)
	{
		return pSchemas[(pTmp = name)];
	}


	inline const Schema::Hash& Protocol::allSchemas() const
	{
		return pSchemas;
	}




} // namespace Messaging
} // namespace Yuni

#endif // __YUNI_MESSAGING_PROTOCOL_HXX__
