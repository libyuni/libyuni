
#include <yuni/yuni.h>
#include <yuni/core/system/console.h>
#include "calculator.h"
#include <iostream>



int main()
{
	Demo::Calculator::Parser parser;

	if (parser.load("4+3*(9.42-2)"))
	{
		// Colors only when the attached console is a TTY
		bool withColors = Yuni::System::Console::IsStdoutTTY();

		Yuni::Clob output;
		Demo::Calculator::Node::Export(output, *parser.root, withColors);
		std::cout << output << std::endl;
		return EXIT_SUCCESS;
	}


	std::cerr << "failed to parse\n";
	return EXIT_FAILURE;
}
