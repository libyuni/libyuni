#ifndef __YUNI_MESSAGING_API_SCHEMA_HXX__
# define __YUNI_MESSAGING_API_SCHEMA_HXX__


namespace Yuni
{
namespace Messaging
{

	inline void Schema::shrinkMemory()
	{
		defaults.shrinkMemory();
		methods.shrinkMemory();
	}


	inline void Schema::Defaults::shrinkMemory()
	{
		pTmp.clear();
		pTmp.shrink();
	}


	inline const API::Method::Parameter::Hash& Schema::Defaults::params() const
	{
		return pParams;
	}





} // namespace Messaging
} // namespace Yuni

#endif // __YUNI_MESSAGING_API_SCHEMA_HXX__
