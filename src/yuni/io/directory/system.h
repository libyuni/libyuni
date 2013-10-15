#ifndef __YUNI_IO_DIRECTORY_SYSTEM_H__
# define __YUNI_IO_DIRECTORY_SYSTEM_H__

# include "../directory.h"


namespace Yuni
{
namespace IO
{
namespace Directory
{
/*!
** \brief Special folders
*/
namespace System
{

	/*!
	** \brief Get the temporary folder
	**
	** \param[out] out Variable of type 'string'/'container' where the value will be appened
	** \param emptyBefore True to empty the variable \p out before anything else
	** \return True if the operation succeeded, false otherwise
	*/
	template<class StringT>
	bool Temporary(StringT& out, bool emptyBefore = true);


	/*!
	** \brief Get the user's home folder
	**
	** \param[out] out Variable of type 'string'/'container' where the value will be appened
	** \param emptyBefore True to clear the given variable \p out before
	** \return True if the operation succeeded, false otherwise
	*/
	template<class StringT>
	bool UserHome(StringT& out, bool emptyBefore = true);





} // namespace System
} // namespace Directory
} // namespace IO
} // namespace Yuni

# include "system.hxx"

#endif // __YUNI_IO_DIRECTORY_SYSTEM_H__
