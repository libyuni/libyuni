#ifndef __YUNI_PARSER_CHARSETS_HXX__
# define __YUNI_PARSER_CHARSETS_HXX__


namespace Yuni
{
namespace Parser
{

	inline uint Charsets::size() const
	{
		return (uint) pCharsets.size();
	}


	inline Charset Charsets::add(const AnyString& name)
	{
		uint index = (uint) pCharsets.size();
		pCharsets.resize(index + 1);
		pCharsets[index].name = name;
		pByName[name] = index;
		return Charset(this, index);
	}




} // namespace Parser
} // namespace Yuni

#endif // __YUNI_PARSER_CHARSET_HXX__
