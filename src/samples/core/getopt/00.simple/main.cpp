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
** ./getopt_00_simple --text="Lorem Ipsum"
** ./getopt_00_simple --text "Lorem Ipsum"
** ./getopt_00_simple -t "Lorem Ipsum"
** ./getopt_00_simple -t "Lorem Ipsum" --verbose
** ./getopt_00_simple -v
** ./getopt_00_simple /text "Lorem Ipsum"  (Windows only)
** ./getopt_00_simple /t "Lorem Ipsum"     (Windows only)
**
** You can try an unknown option :
** ./getopt_00_simple --unknown-option
*/



int main(int argc, char* argv[])
{
	// The command line options parser
	GetOpt::Parser options;

	// A simple option : --title="<X>" or -t "<X>"
	// We have to provide the variable where the given value on the command line
	// will be written
	String optText = "A default value";
	// Adding the option to the parser :
	// <variable>    : the variable where the user data will be written
	// 't'           : The short name of the option (./getopt_00_simple -t "my value")
	// "text"        : The long name of the option  (./getopt_00_simple --text "my value")
	// <description> : The description of the option, that will be displayed in the help usage
	options.add(optText, 't', "title", "An option with a parameter");

	// A simple flag, enabled when the option is present on the command line
	// Example: ./getopt_00_simple --verbose
	bool optVerbose = false;
	options.addFlag(optVerbose, 'v', "verbose", "A simple flag");


	// Ask to the parser to parse the command line
	if (not options(argc, argv))
	{
		// The program should not continue here
		// The user may have requested the help or an error has happened
		// If an error has happened, the exit status should be different from 0
		if (options.errors())
		{
			std::cout << "Abort due to error" << std::endl;
			return 1;
		}
		return 0;
	}

	// Displaying the value of our variable on the standard output
	std::cout << "Value for `optText` : `" << optText << "`" << std::endl;
	std::cout << "Verbose             : " << optVerbose << std::endl;

	return 0;
}
