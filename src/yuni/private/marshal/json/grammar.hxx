#ifndef __CPP_PARSER_JSON_GRAMMAR_HXX__
# define __CPP_PARSER_JSON_GRAMMAR_HXX__


namespace Grammar
{
namespace JSON
{


	inline Token::Token() :
		production((uint) -1),
		symbol(symUnknown),
		special(false)
	{}


	inline Token::~Token()
	{
	}


	inline void Token::reset()
	{
		text.clear();
		tokens.clear();
		symbol     = symUnknown;
		special    = false;
		production = (uint) -1;
	}


	inline Cursor::Cursor() :
		line((uint) -1),
		column((uint) -1),
		bytes((uint) -1)
	{}




} // namespace JSON
} // namespace Grammar

#endif // __CPP_PARSER_JSON_GRAMMAR_HXX__
