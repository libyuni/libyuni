
#include <yuni/yuni.h>
#include <yuni/parser/peg/grammar.h>
#include <yuni/core/getopt.h>
#include <yuni/core/logs.h>
#include <yuni/io/file.h>

using namespace Yuni;

Yuni::Logs::Logger<>  logs;


void ParseWarning(const AnyString& message)
{
	logs.warning() << message;
}

void ParseError(const AnyString& message)
{
	logs.error() << message;
}




int main(int argc, char** argv)
{
	// The grammar to open
	String filename;

	// arguments parsing
	{
		GetOpt::Parser options;
		String optFilename;
		options.add(optFilename, 'i', "input", "The input grammar");
		options.remainingArguments(optFilename);

		if (not options(argc, argv))
		{
			if (options.errors())
			{
				std::cout << "Abort due to error" << std::endl;
				return EXIT_FAILURE;
			}
			return 0;
		}

		if (optFilename.empty())
		{
			logs.error() << "please provide a grammar file";
			return EXIT_FAILURE;
		}
		if (not IO::File::Exists(optFilename))
		{
			logs.error() << "i/o error: not found: " << optFilename;
			return EXIT_FAILURE;
		}

		IO::Canonicalize(filename, optFilename);
	}

	Parser::PEG::Grammar grammar;
	grammar.onWarning.connect(& ParseWarning);
	grammar.onError.connect(& ParseError);

	if (not grammar.loadFromFile(filename))
	{
		logs.error() << "impossible to load " << filename;
		return EXIT_FAILURE;
	}

	std::cout << grammar << std::endl;

	//Clob dot;
	//grammar.exportToDOT(dot);
	//std::cout << dot << std::endl;

	Clob h, hxx, cpp;
	grammar.exportToCPP("./calculator-grammar.", "Demo::Calculator");

	return 0;
}

