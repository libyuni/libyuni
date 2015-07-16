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
#ifdef YUNI_OS_WINDOWS

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
	class YUNI_DECL WString final
		: private NonCopyable<WString<UNCPrefix, AppendSeparatorT> >
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

#endif // YUNI_OS_WINDOWS
