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
#include <yuni/marshal/object.h>

using namespace Yuni;


static void FillObjectWithSomeMovies(Marshal::Object& root)
{
	// Pixar
	Marshal::Object& pixar = root["movies"]["Pixar"];
	pixar["Cars"] = "http://www.imdb.com/title/tt0317219/";
	pixar["Cars 2"] = "http://www.imdb.com/title/tt1216475/";

	// Dreamworks
	Marshal::Object& dreamworks = root["movies"]["Dreamworks"];
	dreamworks["Dragons: Riders of Berk"] = "http://www.imdb.com/title/tt2325846/";
	dreamworks["Rise of the Guardians"] = "http://www.imdb.fr/title/tt1446192/";

	Marshal::Object& disney = root["movies"]["Disney"];
	disney["Wreck-It Ralph"] = "http://www.imdb.com/title/tt1772341/";
	disney["Brave"] = "http://www.imdb.com/title/tt1217209/";
}


int main(int, char**)
{
	// our root object
	Marshal::Object root;
	// putting some data into it
	FillObjectWithSomeMovies(root);

	// Exporting to JSON
	Clob out;
	root.toJSON(out);

	// The output should look like the following :
	// {
	//    "movies": {
	//        "Disney": {
	//            "Brave": "http://www.imdb.com/title/tt1217209/",
	//            "Wreck-It Ralph": "http://www.imdb.com/title/tt1772341/"
	//        },
	//        "Dreamworks": {
	//            "Dragons: Riders of Berk": "http://www.imdb.com/title/tt2325846/",
	//            "Rise of the Guardians": "http://www.imdb.fr/title/tt1446192/"
	//        },
	//        "Pixar": {
	//            "Cars": "http://www.imdb.com/title/tt0317219/",
	//            "Cars 2": "http://www.imdb.com/title/tt1216475/"
	//        }
	//    }
	// }
	std::cout << out << std::endl;
	return 0;
}

