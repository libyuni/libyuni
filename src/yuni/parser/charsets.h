#ifndef __YUNI_PARSER_CHARSETS_H__
# define __YUNI_PARSER_CHARSETS_H__

# include "../yuni.h"
# include "../private/parser/info-charset.h"
# include "fwd.h"


namespace Yuni
{
namespace Parser
{

	class Charsets final
	{
	public:
		//! \name Constructor
		//@{
		//! Default constructor
		Charsets();
		//@}


		//! Create a new charset
		Charset add(const AnyString& name);

		/*!
		** \brief Add standard POSIX expressions
		**
		** [:alnum:]   [a-zA-Z0-9]
		** [:alpha:]   [a-zA-Z]
		** [:digit:]   [0-9]
		** [:blank:]   [ \t]
		** [:punc:]    [!"#$%&'()*+,\-./:;<=>?@[\\\]^_`{|}~]
		** [:space:]   [ \t\r\n\v\f]
		** [:upper:]   [A-Z]
		** [:lower:]   [a-z]
		** [:xdigit:]  [A-Fa-f0-9]
		*/
		void addPosixExpressions();

		uint size() const;

	private:
		//! List of all charsets
		std::vector<Yuni::Private::Parser::CharsetInfo> pCharsets;
		//! Reverse
		// The key is stored in \p pCharsets
		Dictionary<AnyString, uint>::Unordered pByName;
		// nakama
		friend class Charset;

	}; // class Charset






} // namespace Parser
} // namespace Yuni

# include "charset.h"
# include "charsets.hxx"

#endif // __YUNI_PARSER_CHARSETS_H__
