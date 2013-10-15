
#include <yuni/yuni.h>
#include <yuni/parser/grammar.h>

using namespace Yuni;




int main(int, char**)
{
	Parser::Grammar grammar;

	// charsets
	grammar.charsets.addPosixExpressions();

	return 0;
}

