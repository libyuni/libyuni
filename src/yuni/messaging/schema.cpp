
#include "schema.h"


namespace Yuni
{
namespace Messaging
{

	Schema::Defaults& Schema::Defaults::param(const AnyString& name, const AnyString& brief)
	{
		API::Method::Parameter& param = pParams[(pTmp = name)];
		param.name = name;
		param.brief = brief;
		param.defvalue.clear();
		param.hasDefault = false;
		return *this;
	}


	Schema::Defaults& Schema::Defaults::param(const AnyString& name, const AnyString& brief, const AnyString& defvalue)
	{
		API::Method::Parameter& param = pParams[(pTmp = name)];
		param.name = name;
		param.brief = brief;
		param.defvalue = defvalue;
		param.hasDefault = true;
		return *this;
	}






} // namespace Messaging
} // namespace Yuni

