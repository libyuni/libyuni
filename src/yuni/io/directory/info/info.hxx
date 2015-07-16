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
#include "info.h"



namespace Yuni
{
namespace IO
{
namespace Directory
{


	inline Info::Info(const AnyString& directory)
		: pDirectory(directory)
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

	inline Info& Info::operator = (const AnyString& rhs)
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

