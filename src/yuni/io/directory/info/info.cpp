
#include "info.h"
#include "../../file.h"


namespace Yuni
{
namespace IO
{
namespace Directory
{


	static void NormalizeTinyDirectoryPath(String& path)
	{
		CString<1024, false> tmp = path;
		IO::Normalize(path, tmp);
	}



	void Info::normalize()
	{
		// We will use a tiny optimization here
		// When possible, we will use a static buffer to avoid as much as possible
		// malloc and free.
		if (pDirectory.size() < 1024)
			NormalizeTinyDirectoryPath(pDirectory);
		else
		{
			String tmp = pDirectory;
			IO::Normalize(pDirectory, tmp);
		}
	}


	bool Info::clean() const
	{
		bool result = true;
		iterator i(pDirectory);
		for (; i.valid(); ++i)
		{
			if (i.isFile())
			{
				// removing the file
				result = (IO::errNone == IO::File::Delete(i.filename())) and result;
			}
			else
			{
				if (i.isFolder())
				{
					// Removing the folder
					result = IO::Directory::Remove(i.filename()) and result;
				}
			}
		}
		return result;
	}





} // namespace Directory
} // namespace IO
} // namespace Yuni

