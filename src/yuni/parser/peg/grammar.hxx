#ifndef __YUNI_PARSER_GENERATOR_GRAMMAR_HXX__
# define __YUNI_PARSER_GENERATOR_GRAMMAR_HXX__


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

#endif // __YUNI_PARSER_GENERATOR_GRAMMAR_H__
