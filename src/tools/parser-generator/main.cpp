
#include <yuni/yuni.h>
#include <yuni/core/string.h>
#include <yuni/io/file.h>
#include <yuni/core/getopt.h>
#include <yuni/core/logs.h>
#include <yuni/parser/peg/grammar.h>
#include <yuni/core/variant.h>

using namespace Yuni;



//! logger
static Yuni::Logs::Logger<>  logs;





class Settings final
{
public:
	enum Format
	{
		sfCPP,
	};

public:
	Settings() :
		format(sfCPP)
	{}

public:
	//! All filenames
	String::Vector filenames;
	//! Namespace
	String namespaceName;

	//! Export format
	Format format;

}; // class Settings







static void ParseWarning(const AnyString& message)
{
	logs.warning() << message;
}


static void ParseError(const AnyString& message)
{
	logs.error() << message;
}


static inline void ParseCommandLine(int argc, char** argv, Settings& settings)
{
	GetOpt::Parser options;
	String::Vector optFilenames;
	ShortString16 format;

	options.add(optFilenames, 'i', "input", "The input grammar");
	options.add(settings.namespaceName, 'n', "namespace", "The target namespace (mandatory)");
	options.add(format, 'f', "format", "Output format [cpp]");
	options.remainingArguments(optFilenames);

	if (not options(argc, argv))
	{
		if (options.errors())
		{
			std::cout << "Abort due to error" << std::endl;
			exit(EXIT_FAILURE);
		}
		exit(0);
	}

	if (optFilenames.empty())
	{
		logs.error() << "please provide a grammar file";
		exit(EXIT_FAILURE);
	}

	settings.namespaceName.trim(" \t\r\n.:");
	if (settings.namespaceName.empty())
	{
		logs.error() << "no namespace provided";
		exit(EXIT_FAILURE);
	}

	format.trim();
	format.toLower();
	if (format == "cpp")
		settings.format = Settings::sfCPP;
	else
	{
		logs.error() << "invalid output format";
		exit(EXIT_FAILURE);
	}

	settings.filenames.resize((uint) optFilenames.size());
	for (uint i = 0; i != (uint) optFilenames.size(); ++i)
		IO::Canonicalize(settings.filenames[i], optFilenames[i]);
}




#include <yuni/uuid/uuid.h>
#include <map>

int main(int argc, char** argv)
{
	// All settings
	Settings settings;

	// arguments parsing
	ParseCommandLine(argc, argv, settings);


	Parser::PEG::Grammar grammar;
	grammar.onWarning.connect(& ParseWarning);
	grammar.onError.connect(& ParseError);

	bool hasError = false;
	String output;

	for (uint i = 0; i != (uint) settings.filenames.size(); ++i)
	{
		const String& url = settings.filenames[i];

		if (not IO::File::Exists(url))
		{
			logs.error() << "io error: \"" << url << "\" not found";
			hasError = true;
			continue;
		}

		if (grammar.loadFromFile(url))
		{
			//std::cout << grammar << std::endl;

			output = url;
			IO::ReplaceExtension(output, ".");

			switch (settings.format)
			{
				case Settings::sfCPP:
				{
					logs.info() << "generating c++ parser from " << url;
					grammar.exportToCPP(output, settings.namespaceName);
					break;
				}
			}
		}
		else
		{
			logs.error() << "impossible to load the grammar " << url;
			hasError = true;
		}
	}

	return (not hasError) ? 0 : EXIT_FAILURE;
}

