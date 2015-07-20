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
#include <yuni/core/getopt.h>
#include <iostream>


using namespace Yuni;


/*
** How to try the example from the command line :
**
** To Display the help :
** ./getopt_00_simple -h
** ./getopt_00_simple --help
** ./getopt_00_simple /?      (Windows only)
** ./getopt_00_simple /help   (Windows only)
**
** To test the example of an option :
** ./getopt_00_simple --width=1024 --height=768
** ./getopt_00_simple --width=1024 --height=768 --fullscreen=yes
** ./getopt_00_simple --width=1024 --height=768 --fullscreen=true
** ./getopt_00_simple --width=1024 --height=768 --fullscreen=true --language=ru
** ./getopt_00_simple -l fr
** ./getopt_00_simple -lf fr
**
** You can try an unknown option :
** ./getopt_00_simple --unknown-option
*/



int main(int argc, char* argv[])
{
	// The command line options parser
	GetOpt::Parser options;

	options.addParagraph("  This sample shows you how to add custom text paragraph where you want. "
		"In any cases, like for the options, the text will be formated to 80 columns.\n"
		"\n"
		"A long explanation :\n  Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer nec odio. Praesent "
		"libero. Sed cursus ante dapibus diam. Sed nisi. Nulla quis sem at nibh elementum imperdiet. "
		"Duis sagittis ipsum. Praesent mauris. Fusce nec tellus sed augue semper porta. Mauris massa. "
		"Vestibulum lacinia arcu eget nulla.\n");

	// Screen Settings
	options.addParagraph("Screen Settings:");
	// --width
	unsigned int optWidth = 800;
	options.add(optWidth, ' ', "width",  "Width of the screen");
	// --height
	unsigned int optHeight = 600;
	options.add(optHeight, ' ', "height", "Height of the screen");
	// --depth
	unsigned int optDepth = 32;
	options.add(optDepth, ' ', "depth", "Depth");
	// --fullscreen
	bool optFullscreen = false;
	options.addFlag(optFullscreen, 'f', "fullscreen", "Toggle the fullscreen mode");

	// Languages
	options.addParagraph("\nLanguages:");
	// --language
	String optLang = "en";
	options.add(optLang, 'l', "language", "Set the language to VALUE");
	// --language-list
	bool optLangList = false;
	options.addFlag(optLangList, ' ', "language-list", "Print the list of all languages and exit");

	// Help
	options.addParagraph("\nHelp");
	// Actually, when the option `help` is not overridden (means you did not add an
	// option with a long name `help`), the description of the --help option
	// is automatically added to the help usage.
	// If you want to make your own help usage, see the sample `04.customhelpusage`


	// Ask the parser to parse the command line
	if (!options(argc, argv))
	{
		// The program should not continue here
		return (options.errors() ? 1 /* Error */ : 0);
	}

	if (optLangList)
	{
		// As written in the help usage, display the list of languages
		// and exit
		std::cout << "Example: All languages : en, fr, ru, jp" << std::endl;
		return 0;
	}

	// Displaying the value of our variable on the standard output
	std::cout << "Width      : `" << optWidth << "`" << std::endl;
	std::cout << "Height     : `" << optHeight << "`" << std::endl;
	std::cout << "Depth      : `" << optDepth << "`" << std::endl;
	std::cout << "Fullscren  : `" << optFullscreen << "`" << std::endl;
	std::cout << "Language   : `" << optLang << "`" << std::endl;

	return 0;
}

