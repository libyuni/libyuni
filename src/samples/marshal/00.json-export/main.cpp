
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

