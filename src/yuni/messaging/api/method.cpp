
#include "method.h"


namespace Yuni
{
namespace Messaging
{
namespace API
{

	void Method::shrinkMemory()
	{
		pTmp.clear();
		pTmp.shrink();

		if (pBrief.empty() and pBrief.capacity() != 0)
			pBrief.shrink();
	}


	Method& Method::param(const AnyString& name, const AnyString& brief)
	{
		Parameter& param = pParams[(pTmp = name)];
		param.name = name;
		param.brief = brief;
		param.defvalue.clear();
		param.hasDefault = false;
		return *this;
	}


	Method& Method::param(const AnyString& name, const AnyString& brief, const AnyString& defvalue)
	{
		Parameter& param = pParams[(pTmp = name)];
		param.name = name;
		param.brief = brief;
		param.defvalue = defvalue;
		param.hasDefault = true;
		return *this;
	}




} // namespace API
} // namespace Messaging
} // namespace Yuni

