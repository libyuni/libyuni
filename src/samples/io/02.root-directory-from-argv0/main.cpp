
#include <yuni/yuni.h>
#include <yuni/io/file.h>
#include <iostream>

using namespace Yuni;



int main(int, char** argv)
{
	String root;
	IO::ExtractAbsoluteFilePath(root, argv[0]);
	std::cout << "executable root folder : " << root << std::endl;
	return 0;
}

