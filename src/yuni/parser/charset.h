#ifndef __YUNI_PARSER_CHARSET_H__
# define __YUNI_PARSER_CHARSET_H__

# include "../yuni.h"
# include "../core/bit/bit.h"
# include "../core/string.h"
# include "../core/dictionary.h"
# include <cassert>
# include "fwd.h"


namespace Yuni
{
namespace Parser
{

	class Charset final
	{
	public:
		//! UTF8 Range
		typedef std::pair<UTF8::Char, UTF8::Char>  UTF8Range;

	public:
		//! Default constructor
		Charset();

		//! Register a new UTF8 char
		void add(UTF8::Char c);

		//! Register a new set of characters
		void add(const AnyString& listofchars);

		//! Add a range
		void addRange(UTF8::Char from, UTF8::Char to);


		//! \name Contains
		//@{
		/*!
		** \brief Get if the charset contains a given utf8 char
		*/
		bool contains(const UTF8::Char& c) const;
		//@}


		//! \name Standards charsets
		//@{
		//! Add whitespaces \r\n\t\v
		void addWhitespaces();

		//! Add lower/upper case letters a-zA-Z
		void addLetters();

		//! Add digits 0-9
		void addNumbers();

		//! Add alpha numeric (letters + numbers)
		void addAlphaNumeric();
		//@}


		//! \name Operators
		//@{
		//! Assign with a list of chars
		Charset& operator = (const AnyString& listofchars);
		//! Assign with an exclusion of a list of chars
		Charset& operator ^= (const AnyString& listofchars);
		//! Add a list of chars
		Charset& operator += (const AnyString& listofchars);
		//! Add a list of chars
		Charset& operator |= (const AnyString& listofchars);
		//@}


	private:
		//! Default constructor
		Charset(Charsets* list, uint index);

	private:
		//! Container
		Charsets* pList;
		//! Internal index of the charset
		uint pIndex;
		// nakama !
		friend class Charsets;

	}; // class Charset






} // namespace Parser
} // namespace Yuni

# include "charsets.h"
# include "charset.hxx"

#endif // __YUNI_PARSER_CHARSET_H__
