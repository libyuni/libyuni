#include <yuni/yuni.h>
#include <yuni/string.h>
#include <yuni/io/file.h>
#include <iostream>

using namespace Yuni;



class Reader final
{
public:
	void operator () (const String& line) const
	{
		s = line;
		size_t length = line.size();
		s.replace("\\", "\\\\");
		s.replace("\"", "\\\"");
		s.replace("\r", "\\r");
		s.replace("\n", "\\n");
		s.replace("\t", "\\t");

		content << "\t\tout.append(\"" << s << "\\n\", " << (1 + length) << ");\n";
	}

public:
	mutable Clob content;
	mutable Clob s;
};



int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cerr << "USAGE: <template-file> <destination-file>\n";
		return 1;
	}

	AnyString tmplfile(argv[1]);
	String tmplcontent;

	Reader reader;
	reader.content  = "\n\n// GENERATED\n";
	reader.content += "\n\ntemplate<class StreamT>\nstatic inline void PrepareCPPInclude(StreamT& out)\n{\n";

	bool success = IO::File::ReadLineByLine(argv[1], reader);

	reader.content << "}\n";

	/*
	if (IO::errNone != IO::File::LoadFromFile(tmplcontent, tmplfile) or tmplcontent.empty())
	{
		std::cerr << "failed to load '" << tmplfile << "'\n";
		return 1;
	}


	tmplcontent.replace("\\", "\\\\");
	tmplcontent.replace("\"", "\\\"");
	tmplcontent.replace("\n", "\\n\";\n\tout << \"");

	tmplcontent.prepend("\ntemplate<class StreamT>\nstatic inline void PrepareCPPInclude(StreamT& out)\n{\n\tout << \"");
	tmplcontent << "\";\n}\n";
	tmplcontent.replace("<< \"\\n\";", "<< '\\n';");

	tmplcontent.replace("out << ", "out.append(");
	tmplcontent.replace(";\n", ");\n");*/

	return (success and IO::File::SetContent(argv[2], reader.content)) ? 0 : 1;
}
