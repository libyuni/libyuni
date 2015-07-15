#pragma once
#include "../../yuni.h"
#include "string.h"


namespace Yuni
{

	/*!
	** \brief Escapes a string
	*/
	template<class StringT>
	YUNI_DECL void AppendEscapedString(StringT& out, const AnyString& string, char quote = '"');



} // namespace Yuni

#include "escape.hxx"
