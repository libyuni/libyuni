/*
** YUNI's default license is the GNU Lesser Public License (LGPL), with some
** exclusions (see below). This basically means that you can get the full source
** code for nothing, so long as you adhere to a few rules.
**
** Under the LGPL you may use YUNI for any purpose you wish, and modify it if you
** require, as long as you:
**
** Pass on the (modified) YUNI source code with your software, with original
** copyrights intact :
**  * If you distribute electronically, the source can be a separate download
**    (either from your own site if you modified YUNI, or to the official YUNI
**    website if you used an unmodified version) – just include a link in your
**    documentation
**  * If you distribute physical media, the YUNI source that you used to build
**    your application should be included on that media
** Make it clear where you have customised it.
**
** In addition to the LGPL license text, the following exceptions / clarifications
** to the LGPL conditions apply to YUNI:
**
**  * Making modifications to YUNI configuration files, build scripts and
**    configuration headers such as yuni/platform.h in order to create a
**    customised build setup of YUNI with the otherwise unmodified source code,
**    does not constitute a derived work
**  * Building against YUNI headers which have inlined code does not constitute a
**    derived work
**  * Code which subclasses YUNI classes outside of the YUNI libraries does not
**    form a derived work
**  * Statically linking the YUNI libraries into a user application does not make
**    the user application a derived work.
**  * Using source code obsfucation on the YUNI source code when distributing it
**    is not permitted.
** As per the terms of the LGPL, a "derived work" is one for which you have to
** distribute source code for, so when the clauses above define something as not
** a derived work, it means you don't have to distribute source code for it.
** However, the original YUNI source code with all modifications must always be
** made available.
*/
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





static bool hasError = false;


static void ParseWarning(const AnyString& message)
{
	logs.warning() << message;
}


static void ParseError(const AnyString& message)
{
	logs.error() << message;
	hasError = true;
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
	{
		settings.format = Settings::sfCPP;
	}
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

	String output;

	for (uint i = 0; i != (uint) settings.filenames.size(); ++i)
	{
		const String& url = settings.filenames[i];

		if (not IO::File::Exists(url))
		{
			logs.error() << "error: \"" << url << "\" file not found";
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

