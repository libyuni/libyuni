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
#include "option.h"
#include <iostream>

#define YUNI_GETOPT_HELPUSAGE_30CHAR  "                             "



namespace Yuni
{
namespace Private
{
namespace GetOptImpl
{


	template<bool Decal, int LengthLimit>
	static void PrintLongDescription(std::ostream& out, const String& description)
	{
		uint start = 0;
		uint end = 0;
		uint offset = 0;

		do
		{
			// Jump to the next separator
			offset = description.find_first_of(" .\r\n\t", offset);

			// No separator, aborting
			if (String::npos == offset)
				break;

			if (offset - start < LengthLimit)
			{
				if ('\n' == description.at(offset))
				{
					out.write(description.c_str() + start, (std::streamsize)(offset - start));
					out << '\n';
					if (Decal)
						out.write(YUNI_GETOPT_HELPUSAGE_30CHAR, 30);

					start = offset + 1;
					end = offset + 1;
				}
				else
					end = offset;
			}
			else
			{
				if (0 == end)
					end = offset;

				out.write(description.c_str() + start, (std::streamsize)(end - start));
				out << '\n';

				if (Decal)
					out.write(YUNI_GETOPT_HELPUSAGE_30CHAR, 30);

				start = end + 1;
				end = offset + 1;
			}

			++offset;
		}
		while (true);

		// Display the remaining piece of string
		if (start < description.size())
			out << (description.c_str() + start);
	}







	void DisplayHelpForOption(std::ostream& out, const String::Char shortName, const String& longName,
		const String& description, bool requireParameter)
	{
		// Space
		if ('\0' != shortName && ' ' != shortName)
		{
			out.write("  -", 3);
			out << shortName;

			if (longName.empty())
			{
				if (requireParameter)
					out.write(" VALUE", 6);
			}
			else
				out.write(", ", 2);
		}
		else
			out.write("      ", 6);

		// Long name
		if (longName.empty())
		{
			if (requireParameter)
				out << "              ";
			else
				out << "                    ";
		}
		else
		{
			out.write("--", 2);
			out << longName;
			if (requireParameter)
				out.write("=VALUE", 6);

			if (30 <= longName.size() + 6 /*-o,*/ + 2 /*--*/ + 1 /*space*/ + (requireParameter ? 6 : 0))
				out << "\n                             ";
			else
			{
				for (uint i = 6 + 2 + 1 + static_cast<uint>(longName.size()) + (requireParameter ? 6 : 0); i < 30; ++i)
					out.put(' ');
			}
		}
		// Description
		if (description.size() <= 50 /* 80 - 30 */)
			out << description;
		else
			PrintLongDescription<true, 50>(out, description);

		out << '\n';
	}


	void DisplayTextParagraph(std::ostream& out, const String& text)
	{
		if (text.size() <= 80)
			out << text;
		else
			PrintLongDescription<false, 80>(out, text);

		out << '\n';
	}




} // namespace GetOptImpl
} // namespace Private
} // namespace Yuni

