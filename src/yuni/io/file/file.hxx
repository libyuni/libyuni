#ifndef __YUNI_IO_FILE_HXX__
# define __YUNI_IO_FILE_HXX__

# include "../../core/traits/cstring.h"
# include "../../core/traits/length.h"
# include "../../core/static/remove.h"



namespace Yuni
{
namespace IO
{
namespace File
{


	inline uint64 Size(const AnyString& filename)
	{
		uint64 size;
		return (Size(filename, size)) ? size : 0;
	}


	inline bool Exists(const AnyString& filename)
	{
		return ((Yuni::IO::typeFile & Yuni::IO::TypeOf(filename)) != 0);
	}


	template<class U>
	bool SetContent(const AnyString& filename, const U& content)
	{
		IO::File::Stream file(filename, OpenMode::write | OpenMode::truncate);
		if (file.opened())
		{
			file += content;
			return true;
		}
		return false;
	}


	template<class U>
	bool AppendContent(const AnyString& filename, const U& content)
	{
		IO::File::Stream file(filename, OpenMode::write | OpenMode::append);
		if (file.opened())
		{
			file += content;
			return true;
		}
		return false;
	}



	template<class StringT, class U>
	bool SaveToFile(const StringT& filename, const U& content)
	{
		IO::File::Stream file(filename, OpenMode::write | OpenMode::truncate);
		if (file.opened())
		{
			file += content;
			return true;
		}
		return false;
	}



	template<class StringT, class PredicateT>
	bool
	ReadLineByLine(const StringT& filename, const PredicateT& predicate)
	{
		IO::File::Stream file;
		if (file.open(filename))
		{
			String line;
			while (file.readline<4096u, String>(line))
				predicate(line);

			return true;
		}
		return false;
	}






} // namespace File
} // namespace IO
} // namespace Yuni

#endif // __YUNI_IO_FILE_H__
