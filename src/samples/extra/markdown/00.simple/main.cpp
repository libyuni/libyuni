/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** github: https://github.com/libyuni/libyuni/
** gitlab: https://gitlab.com/libyuni/libyuni/ (mirror)
*/
#include <yuni/yuni.h>
#include <yuni/extra/markdown/reader.h>
#include <yuni/extra/markdown/renderer/html.h>
#include <yuni/core/io/file.h>


int main(int argc, char** argv)
{
	if (argc < 2)
		return 0;

	using namespace Yuni;

	Markdown::Reader  reader;
	Markdown::Renderer::Html html;
	CString<4096> buffer;

	for (int i = 1; i < argc; ++i)
	{
		// filename
		const char* const filename = argv[i];

		Core::IO::File::Stream file;
		// opening out file
		if (file.open(filename))
		{
			reader.beginDocument(argv[i]);
			// A buffer. The given capacity will be the maximum length for a single line
			while (file.readline(buffer))
				reader += buffer;
		
			reader.endDocument();

			html.render(reader);
		}
	}

	return 0;
}


