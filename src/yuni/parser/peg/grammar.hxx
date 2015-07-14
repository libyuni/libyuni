#pragma once
#include "grammar.h"


namespace Yuni
{
namespace Parser
{
namespace PEG
{


	inline bool Grammar::loadFromString(const AnyString& content)
	{
		return loadFromData(content, "<memory>");
	}




} // namespace PEG
} // namespace Parser
} // namespace Yuni
