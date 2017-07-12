/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** gitlab: https://gitlab.com/libyuni/libyuni/
** github: https://github.com/libyuni/libyuni/ {mirror}
*/
#include <yuni/yuni.h>
#include <yuni/core/string.h>
#include <yuni/io/file.h>
#include <yuni/core/getopt.h>
#include <yuni/core/logs.h>
#include <yuni/parser/peg/grammar.h>

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
	String filename;
	//! Namespace
	String namespaceName;
	//! Output folder
	String outputFolder;

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
	String optFilename;
	ShortString16 format;
	String outputFolder;

	options.add(optFilename, 'i', "input", "The input grammar");
	options.add(settings.namespaceName, 'n', "namespace", "The target namespace (required)");
	options.add(format, 'f', "format", "Output format [cpp]");
	options.add(outputFolder, 'o', "output", "Output folder (required)");

	if (not options(argc, argv))
	{
		if (options.errors())
		{
			std::cout << "Abort due to error" << std::endl;
			exit(EXIT_FAILURE);
		}
		exit(0);
	}

	if (not IO::File::Exists(optFilename))
	{
		logs.error() << "input grammar file not found: '" << optFilename << "'";
		exit(EXIT_FAILURE);
	}

	if (outputFolder.empty())
	{
		logs.error() << "an output folder is required";
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

	IO::Canonicalize(settings.filename, optFilename);
	IO::Canonicalize(settings.outputFolder, outputFolder);
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

	// generate C++ classes from grammar file
	if (grammar.loadFromFile(settings.filename))
	{
		String barname;
		IO::ExtractFileName(barname, settings.filename);

		String output;
		output << settings.outputFolder << IO::Separator << barname;
		IO::ReplaceExtension(output, "."); // remove extension

		switch (settings.format)
		{
			case Settings::sfCPP:
			{
				logs.info() << "generating c++ parser from " << settings.filename;
				logs.info() << "c++ classes output: " << output;
				grammar.exportToCPP(output, settings.namespaceName);
				break;
			}
		}
	}
	else
	{
		logs.error() << "impossible to load the grammar " << settings.filename;
		return EXIT_FAILURE;
	}
	return 0;
}

