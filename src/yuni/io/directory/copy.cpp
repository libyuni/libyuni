
#include "../io.h"
#include "../directory.h"
#include "../../core/slist.h"
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
		typedef LinkedList<InfoItem> List;

	} // anonymous namespace



	bool Copy(const AnyString& src, const AnyString& dst, bool recursive,
		bool overwrite, const IO::Directory::CopyOnUpdateBind& onUpdate)
	{
		// normalize paths
		String fsrc;
		IO::Normalize(fsrc, src);
		if (fsrc.empty())
			return false;

		String fdst;
		IO::Normalize(fdst, dst);

		// The list of files to copy
		List list;
		uint64 totalSize = 0;

		// Adding the target folder, to create it if required
		if (not onUpdate(cpsGatheringInformation, fdst, fdst, 0, 1))
			return false;
		IO::Directory::Create(fdst);

		{
			IO::Directory::Info info(fsrc);
			if (recursive)
			{
				const IO::Directory::Info::recursive_iterator& end = info.recursive_end();
				for (IO::Directory::Info::recursive_iterator i = info.recursive_begin(); i != end; ++i)
				{
					list.push_back();
					InfoItem& info = list.back();
					info.filename = i.filename();
					info.isFile   = i.isFile();
					totalSize += i.size();
					if (!onUpdate(cpsGatheringInformation, *i, *i, 0, list.size()))
						return false;
				}
			}
			else
			{
				const IO::Directory::Info::recursive_iterator& end = info.recursive_end();
				for (IO::Directory::Info::recursive_iterator i = info.recursive_begin(); i != end; ++i)
				{
					list.push_back();
					InfoItem& info = list.back();
					info.filename = i.filename();
					info.isFile   = i.isFile();
					totalSize += i.size();

					if (!onUpdate(cpsGatheringInformation, i.filename(), i.filename(), 0, list.size()))
						return false;
				}
			}
		}

		uint64 current = 0;
		// A temporary string
		String tmp;

		// Streams : in the worst scenario, the last file to copy will be closed
		// at the end of this routine
		// Stream on the source file
		IO::File::Stream fromFile;
		// Stream on the target file
		IO::File::Stream toFile;

		// A temporary buffer for copying files' contents
		// 16k seems to be a good choice (better than smaller block size when used
		// in Virtual Machines)
		enum { bufferSize = 8192 };
		char* buffer = new char[bufferSize];

		// reduce overhead brought by `onUpdate`
		uint skip = 8;

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
				// The target file is actually a folder
				// We have to create it before copying its content
				if (!onUpdate(cpsCopying, info.filename, tmp, current, totalSize)
					|| !IO::Directory::Create(tmp))
				{
					delete[] buffer;
					return false;
				}
			}
			else
			{
				// The target file is a real file (and not a folder)
				// Checking first for overwritting
				if (not overwrite && IO::Exists(tmp))
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
						while ((numRead = fromFile.read(buffer, bufferSize)) != 0)
						{
							// progression
							current += numRead;

							// Trying to copy the block which has just been read
							if (numRead != toFile.write((const char*)buffer, numRead))
							{
								delete[] buffer;
								return false;
							}

							// Notify the user from time to time about the progression
							if (!--skip)
							{
								if (!onUpdate(cpsCopying, info.filename, tmp, current, totalSize))
								{
									delete[] buffer;
									return false;
								}
								skip = 8;
							}
						} // read
					}
				}
			}
		}

		delete[] buffer;

		return true;
	}





} // namespace Directory
} // namespace IO
} // namespace Yuni

