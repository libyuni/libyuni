#ifndef __YUNI_PRIVATE_PARSER_CHARSET_H__
# define __YUNI_PRIVATE_PARSER_CHARSET_H__

# include "../../yuni.h"
# include "../../core/bit/bit.h"
# include "../../core/string.h"
# include "../../core/dictionary.h"


namespace Yuni
{
namespace Private
{
namespace Parser
{

	class CharsetInfo final
	{
	public:
		//! UTF8 Range
		typedef std::pair<UTF8::Char, UTF8::Char>  UTF8Range;

	public:
		//! \name Constructor
		//@{
		//! Default constructor
		CharsetInfo();
		//! Copy constructor
		CharsetInfo(const CharsetInfo& rhs);
		//@}


		//! Clear the charset
		void clear();

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
		CharsetInfo& operator = (const CharsetInfo& rhs);
		//@}

	public:
		//! Get if the match is included within the charset or not
		bool excluded;
		//! Ascii enabled
		char ascii[UTF8::Char::asciiLimit / 8];
		//! UTF8 single characters
		Set<UTF8::Char>::Unordered utf8chars;
		//! UTF8 Range
		std::vector<UTF8Range> utf8ranges;
		//! Name of the charsets
		String name;

	}; // class CharsetInfo






} // namespace Parser
} // namespace Private
} // namespace Yuni

#endif // __YUNI_PARSER_CHARSET_H__
