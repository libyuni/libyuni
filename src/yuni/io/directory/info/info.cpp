
#include "info.h"


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
			// Removing the folder
			result = IO::Directory::Remove(*i) and result;
		}
		return result;
	}





} // namespace Directory
} // namespace IO
} // namespace Yuni

