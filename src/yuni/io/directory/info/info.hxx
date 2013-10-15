#ifndef __YUNI_IO_DIRECTORY_INFO_INFO_HXX__
# define __YUNI_IO_DIRECTORY_INFO_INFO_HXX__


namespace Yuni
{
namespace IO
{
namespace Directory
{


	inline Info::Info()
	{}


	template<class StringT>
	inline Info::Info(const StringT& directory)
		:pDirectory(directory)
	{}


	inline Info::Info(const Info& rhs)
		:pDirectory(rhs.pDirectory)
	{}


	inline bool Info::exists() const
	{
		return IO::Directory::Exists(pDirectory);
	}


	template<class StringT>
	inline void Info::normalize(StringT& tmp)
	{
		tmp = pDirectory;
		IO::Normalize(pDirectory, tmp);
	}

	inline Info::iterator Info::begin() const
	{
		return iterator(pDirectory);
	}


	inline Info::file_iterator Info::file_begin() const
	{
		return file_iterator(pDirectory);
	}

	inline Info::folder_iterator Info::folder_begin() const
	{
		return folder_iterator(pDirectory);
	}

	inline Info::recursive_iterator Info::recursive_begin() const
	{
		return recursive_iterator(pDirectory);
	}


	inline Info::recursive_file_iterator Info::recursive_file_begin() const
	{
		return recursive_file_iterator(pDirectory);
	}

	inline Info::recursive_folder_iterator Info::recursive_folder_begin() const
	{
		return recursive_folder_iterator(pDirectory);
	}




	inline Info::null_iterator Info::end() const
	{
		return null_iterator();
	}


	inline Info::null_iterator Info::recursive_end() const
	{
		return null_iterator();
	}


	inline Info::null_iterator Info::file_end() const
	{
		return null_iterator();
	}


	inline Info::null_iterator Info::folder_end() const
	{
		return null_iterator();
	}


	inline Info::null_iterator Info::recursive_file_end() const
	{
		return null_iterator();
	}


	inline Info::null_iterator Info::recursive_folder_end() const
	{
		return null_iterator();
	}


	inline String& Info::directory()
	{
		return pDirectory;
	}

	inline const String& Info::directory() const
	{
		return pDirectory;
	}


	inline bool Info::create(uint mode) const
	{
		return IO::Directory::Create(pDirectory, mode);
	}


	inline bool Info::remove() const
	{
		return IO::Directory::Remove(pDirectory);
	}


	inline bool Info::copy(const AnyString& destination) const
	{
		return IO::Directory::Copy(pDirectory, destination);
	}


	inline bool Info::setAsCurrentDirectory() const
	{
		return IO::Directory::Current::Set(pDirectory);
	}


	inline Info& Info::operator = (const Info& rhs)
	{
		pDirectory = rhs.pDirectory;
		return *this;
	}

	template<class U>
	inline Info& Info::operator = (const U& rhs)
	{
		pDirectory = rhs;
		return *this;
	}


	inline bool Info::operator == (const Info& rhs) const
	{
		return pDirectory == rhs.pDirectory;
	}


	template<class U>
	inline bool Info::operator == (const U& rhs) const
	{
		return pDirectory == rhs;
	}


	template<class U>
	inline Info& Info::operator += (const U& rhs)
	{
		pDirectory += rhs;
		return *this;
	}


	template<class U>
	inline Info& Info::operator << (const U& rhs)
	{
		pDirectory << rhs;
		return *this;
	}





} // namespace Directory
} // namespace IO
} // namespace Yuni

#endif // __YUNI_IO_DIRECTORY_INFO_INFO_HXX__
