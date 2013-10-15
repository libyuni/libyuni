#ifndef __YUNI_CORE_SYSTEM_ENVIRONMENT_H__
# define __YUNI_CORE_SYSTEM_ENVIRONMENT_H__

# include "../../yuni.h"
# include "../string.h"


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
	String Read(const AnyString& name);


	/*!
	** \brief Get a value from the current environment
	**
	** \param name Name of the variable
	** \param[out] out Variable of type 'string'/'container' where the value will be appened
	** \param emptyBefore True to empty the parameter `out` before
	** \return True if the operation succeeded, false otherwise
	*/
	template<class StringT>
	bool Read(const AnyString& name, StringT& out, bool emptyBefore = true);





} // namespace Environment
} // namespace System
} // namespace Yuni

# include "environment.hxx"

#endif // __YUNI_CORE_SYSTEM_ENVIRONMENT_H__
