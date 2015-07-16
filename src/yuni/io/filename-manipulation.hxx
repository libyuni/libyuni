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
#pragma once
#include "directory.h"



namespace Yuni
{
namespace IO
{
namespace Directory
{

	// Forward declaration
	template<typename C> struct Constant;

} // namespace Directory
} // namespace IO
} // namespace Yuni




namespace Yuni
{
namespace IO
{


	inline bool IsRelative(const AnyString& filename)
	{
		return not IsAbsolute(filename);
	}



	template<class StringT>
	void ExtractFileNameWithoutExtension(StringT& out, const AnyString& path, bool systemDependant)
	{
		AnyString::size_type pos = (systemDependant)
			? path.find_last_of(IO::Constant<char>::Separator)
			: path.find_last_of(IO::Constant<char>::AllSeparators);
		AnyString::size_type n = path.find_last_of('.');

		if (AnyString::npos == n and AnyString::npos == pos)
		{
			out = path;
			return;
		}
		if (n == pos)
		{
			out.clear();
			return;
		}
		if (n == AnyString::npos and n > pos + 1)
		{
			if (AnyString::npos == pos)
			{
				out = path;
				return;
			}
			out.assign(path.c_str() + pos + 1);
			return;
		}
		if (pos == AnyString::npos)
		{
			out.assign(path, n);
			return;
		}
		out.assign(path.c_str() + pos + 1, n - pos - 1);
	}


	template<class StringT>
	bool ExtractExtension(StringT& out, const AnyString& filename, bool dot, bool clear)
	{
		if (clear)
			out.clear();
		// If the string is empty, the buffer may be invalid (NULL)
		if (filename.size())
		{
			uint i = filename.size();
			do
			{
				--i;
				switch (filename[i])
				{
					case '.':
						{
							if (not dot)
							{
								if (++i >= (uint) filename.size())
									return true;
							}
							out.append(filename.c_str() + i, filename.size() - i);
							return true;
						}
					case '/':
					case '\\':
						return false;
				}
			}
			while (i != 0);
		}
		return false;
	}


	template<class StringT>
	void MakeAbsolute(StringT& out, const AnyString& filename, bool clearBefore)
	{
		if (clearBefore)
			out.clear();
		if (IsAbsolute(filename))
		{
			out += filename;
		}
		else
		{
			IO::Directory::Current::Get(out, clearBefore);
			out << IO::Separator << filename;
		}
	}


	template<class StringT>
	void MakeAbsolute(StringT& out, const AnyString& filename, const AnyString& currentPath, bool clearBefore)
	{
		if (clearBefore)
			out.clear();
		if (IsAbsolute(filename))
		{
			out += filename;
		}
		else
		{
			out += currentPath;
			out += IO::Separator;
			out += filename;
		}
	}


	template<class StringT1, class StringT2>
	bool ReplaceExtension(StringT1& filename, const StringT2& newExtension)
	{
		// If the string is empty, the buffer may be invalid (NULL)
		if (filename.size())
		{
			uint i = filename.size();
			do
			{
				--i;
				switch (filename[i])
				{
					case '.':
						{
							filename.resize(i);
							filename += newExtension;
							return true;
						}
					case '/':
					case '\\':
						return false;
				}
			}
			while (i != 0);
		}
		return false;
	}





} // namespace IO
} // namespace Yuni

