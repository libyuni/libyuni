
#include "charsets.h"

// see http://www.regular-expressions.info/posixbrackets.html


namespace Yuni
{
namespace Parser
{


	Charsets::Charsets()
	{
		// symbols here
	}


	void Charsets::addPosixExpressions()
	{
		pCharsets.reserve(pCharsets.size() + 3);

		Charset space = add("[:space:]");
		space += " \t\r\n\v\f";

		Charset alnum = add("[:alnum:]");
		alnum.addLetters();
		alnum.addNumbers();

		Charset alpha = add("[:alpha:]");
		alpha.addLetters();

		Charset digit = add("[:digit:]");
		digit.addNumbers();

		Charset blank = add("[:blank:]");
		blank += " \t";

		Charset punc = add("[:punc:]");
		punc += "!\"#$%&\'()*+,\\-./:;<=>?@[]^_`{|}~";

		Charset upper = add("[:upper:]");
		upper.addRange(UTF8::Char('A'), UTF8::Char('Z'));

		Charset lower = add("[:lower:]");
		lower.addRange(UTF8::Char('a'), UTF8::Char('z'));

		Charset xdigit = add("[:xdigit:]");
		xdigit.addRange(UTF8::Char('a'), UTF8::Char('f'));
		xdigit.addRange(UTF8::Char('A'), UTF8::Char('F'));
		xdigit.addNumbers();
	}




} // namespace Parser
} // namespace Yuni


