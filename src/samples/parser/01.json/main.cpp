
#include <yuni/yuni.h>
#include <yuni/core/system/console.h>
#include "json.h"
#include <iostream>



int main()
{
	Demo::JSON::Parser parser;

	if (parser.loadFromFile("example.json"))
	{
		// Colors only when the attached console is a TTY
		bool withColors = Yuni::System::Console::IsStdoutTTY();

		Yuni::Clob output;
		Demo::JSON::Node::Export(output, *parser.root, withColors);
		std::cout << output << std::endl;
		return EXIT_SUCCESS;
	}

	std::cerr << "failed to parse\n";
	return EXIT_FAILURE;
}
