
#include "calculator-grammar.h"


int main()
{
	Demo::Calculator::Parser parser;
	parser.load("4+3*(9.42-2)");
	return 0;
}
