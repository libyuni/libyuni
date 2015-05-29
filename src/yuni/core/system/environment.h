#pragma once
#include "../../yuni.h"
#include "../string.h"



namespace Yuni
{
namespace System
{
namespace Environment
{

	/*!
	** \brief Get a value from the current environment
	**
	** \param name Name of the variable
	** \return The value of the variable, empty is an error has occured
	*/
	YUNI_DECL String Read(const AnyString& name);


	/*!
	** \brief Get a value from the current environment (Yuni::String version)
	**
	** \param name Name of the variable
	** \param[out] out Variable of type 'string'/'container' where the value will be appened
	** \param emptyBefore True to empty the parameter `out` before
	** \return True if the operation succeeded, false otherwise
	*/
	YUNI_DECL bool Read(const AnyString& name, String& out, bool emptyBefore = true);

	/*!
	** \brief Get a value from the current environment (Yuni::Clob version)
	**
	** \param name Name of the variable
	** \param[out] out Variable of type 'string'/'container' where the value will be appened
	** \param emptyBefore True to empty the parameter `out` before
	** \return True if the operation succeeded, false otherwise
	*/
	YUNI_DECL bool Read(const AnyString& name, Clob& out, bool emptyBefore = true);





} // namespace Environment
} // namespace System
} // namespace Yuni

