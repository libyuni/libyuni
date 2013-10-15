#ifndef __YUNI_CORE_STRING_ESCAPE_H__
# define __YUNI_CORE_STRING_ESCAPE_H__

# include "../../yuni.h"
# include "string.h"


namespace Yuni
{

	/*!
	** \brief Escapes a string
	*/
	template<class StringT>
	void AppendEscapedString(StringT& out, const AnyString& string, char quote = '"');



} // namespace Yuni

# include "escape.hxx"

#endif // __YUNI_CORE_STRING_ESCAPE_H__
