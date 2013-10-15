#ifndef __YUNI_CORE_STRING_WSTRING_H__
# define __YUNI_CORE_STRING_WSTRING_H__

# ifdef YUNI_OS_WINDOWS

# include "../../yuni.h"
# include "string.h"
# include "../system/windows.hdr.h"
# include "../noncopyable.h"


namespace Yuni
{
namespace Private
{

	/*!
	** \brief Convert a C-String into a Wide String (Windows Only)
	**
	** \TODO This class should be dedicated to Yuni::IO
	*/
	template<bool UNCPrefix = false, bool AppendSeparatorT = false>
	class WString final : private NonCopyable<WString<UNCPrefix, AppendSeparatorT> >
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Constructor
		*/
		WString(const AnyString& string);
		/*!
		** \brief Constructor with a C-String
		*/
		WString(const char* cstring, uint size);
		//! Destructor
		~WString();
		//@}

		/*!
		** \brief Size of the wide string
		*/
		uint size() const;

		/*!
		** \brief Get if the string is empty
		*/
		bool empty() const;

		/*!
		** \brief Get the wide string
		*/
		const wchar_t* c_str() const;

		/*!
		** \brief Get the wide string
		*/
		wchar_t* c_str();

		/*!
		** \brief Replace all occurences of a single char
		*/
		void replace(char from, char to);

		//! \name Operators
		//@{
		//! Cast to wchar_t*
		operator const wchar_t* () const;
		//@}


	private:
		//! Convert a C-String into a Wide String
		void prepareWString(const char* const cstring, uint size);

	private:
		//! Wide string
		wchar_t* pWString;
		//! Size of the wide string
		uint pSize;

	}; // class WString





} // namespace Private
} // namespace Yuni

# include "wstring.hxx"

# endif // YUNI_OS_WINDOWS
#endif // __YUNI_CORE_STRING_WSTRING_H__
