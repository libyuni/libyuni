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
#include "../io.h"
#include "../directory.h"
#include "info.h"
#include "../file.h"



namespace Yuni
{
namespace IO
{
namespace Directory
{


	namespace // anonymous
	{

		struct InfoItem
		{
			bool isFile;
			uint64  size;
			String filename;
		};
		typedef std::vector<InfoItem> List;

	} // anonymous namespace



	bool Copy(const AnyString& src, const AnyString& dst, bool recursive, bool overwrite,
		const IO::Directory::CopyOnUpdateBind& onUpdate)
	{
		// normalize paths
		String fsrc;
		IO::Normalize(fsrc, src);
		if (fsrc.empty())
			return false;

		String fdst;
		IO::Normalize(fdst, dst);

		// Adding the target folder, to create it if required
		if (not onUpdate(cpsGatheringInformation, fdst, fdst, 0, 1))
			return false;

		if (not IO::Directory::Create(fdst))
			return false;

		// The list of files to copy
		List list;
		list.reserve(512);
		// the total number of bytes to copy
		uint64 totalSize = 0;


		// get the complete list of all files to copy and all folders to create
		{
			IO::Directory::Info info(fsrc);
			if (recursive)
			{
				const IO::Directory::Info::recursive_iterator& end = info.recursive_end();
				for (IO::Directory::Info::recursive_iterator i = info.recursive_begin(); i != end; ++i)
				{
					list.resize(list.size() + 1);
					InfoItem& info = list.back();
					info.filename = i.filename();
					info.isFile   = i.isFile();
					totalSize += i.size();
					if (not onUpdate(cpsGatheringInformation, *i, *i, 0, list.size()))
						return false;
				}
			}
			else
			{
				const IO::Directory::Info::iterator& end = info.end();
				for (IO::Directory::Info::iterator i = info.begin(); i != end; ++i)
				{
					list.resize(list.size() + 1);
					InfoItem& info = list.back();
					info.filename = i.filename();
					info.isFile   = i.isFile();
					totalSize += i.size();

					if (not onUpdate(cpsGatheringInformation, i.filename(), i.filename(), 0, list.size()))
						return false;
				}
			}
		}

		if (list.empty())
			return true;


		// A temporary buffer for copying files' contents
		// 16k seems to be a good choice (better than smaller block size when used
		// in Virtual Machines)
		const uint bufferSize = 16384;
		char* const buffer = new (std::nothrow) char[bufferSize];
		if (YUNI_UNLIKELY(nullptr == buffer))
			return false;

		uint64 current = 0;
		// A temporary string
		String tmp;
		tmp.reserve(1024);

		// Streams : in the worst scenario, the last file to copy will be closed
		// at the end of this routine
		// Stream on the source file
		IO::File::Stream fromFile;
		// Stream on the target file
		IO::File::Stream toFile;


		// reduce overhead brought by `onUpdate`
		enum { maxSkip = 6 };
		uint skip = (uint) maxSkip;
		// result
		bool success = true;

		const List::const_iterator end = list.end();
		for (List::const_iterator i = list.begin(); i != end; ++i)
		{
			// alias to the current information block
			const InfoItem& info = *i;

			// Address of the target file
			tmp = fdst; // without any OS-dependant separator
			if (fsrc.size() < info.filename.size())
				tmp.append(info.filename.c_str() + fsrc.size(), info.filename.size() - fsrc.size());

			if (not info.isFile)
			{
				// The target file is actually a folder - must be created before copying its content
				if (not onUpdate(cpsCopying, info.filename, tmp, current, totalSize) or not IO::Directory::Create(tmp))
				{
					success = false;
					break;
				}
			}
			else
			{
				// The target file is a real file (and not a folder)
				// Checking first for overwritting
				if (not overwrite and IO::Exists(tmp))
					continue;

				// Try to open the source file
				// The previous opened source file will be closed here
				if (fromFile.open(info.filename, IO::OpenMode::read))
				{
					// Try to open for writing the target file
					// The previous opened target file will be closed here
					if (toFile.open(tmp, IO::OpenMode::write | IO::OpenMode::truncate))
					{
						// reading the whole source file
						uint64 numRead;
						while ((numRead = fromFile.read(buffer, bufferSize)) > 0)
						{
							// progression
							current += numRead;

							// Trying to copy the block which has just been read
							if (numRead != toFile.write((const char*)buffer, numRead))
							{
								success = false;
								break;
							}

							// Notify the user from time to time about the progression
							if (0 == --skip)
							{
								if (not onUpdate(cpsCopying, info.filename, tmp, current, totalSize))
								{
									success = false;
									break;
								}
								skip = (uint) maxSkip;
							}
						} // read
					}
					else
					{
						success = false;
						break;
					}
				}
				else
				{
					success = false;
					break;
				}
			}
		}

		delete[] buffer;

		return success;
	}





} // namespace Directory
} // namespace IO
} // namespace Yuni

