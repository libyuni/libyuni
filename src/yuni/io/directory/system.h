#pragma once
#include "../directory.h"



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
	** \brief Get the temporary folder [string]
	**
	** \param[out] out Variable of type 'string'/'container' where the value will be appended
	** \param emptyBefore True to empty the variable \p out before anything else
	** \return True if the operation succeeded, false otherwise
	*/
	bool Temporary(String& out, bool emptyBefore = true);
	/*!
	** \brief Get the temporary folder [clob]
	**
	** \param[out] out Variable of type 'string'/'container' where the value will be appended
	** \param emptyBefore True to empty the variable \p out before anything else
	** \return True if the operation succeeded, false otherwise
	*/
	bool Temporary(Clob& out, bool emptyBefore = true);




	/*!
	** \brief Get the user's home folder [string]
	**
	** \param[out] out Variable of type 'string'/'container' where the value will be appended
	** \param emptyBefore True to clear the given variable \p out before
	** \return True if the operation succeeded, false otherwise
	*/
	bool UserHome(String& out, bool emptyBefore = true);
	/*!
	** \brief Get the user's home folder [clob]
	**
	** \param[out] out Variable of type 'string'/'container' where the value will be appended
	** \param emptyBefore True to clear the given variable \p out before
	** \return True if the operation succeeded, false otherwise
	*/
	bool UserHome(Clob& out, bool emptyBefore = true);




	/*!
	** \brief Get the system fonts folder [string]
	**
	** \param[out] out Variable of type 'string'/'container' where the value will be appended
	** \param emptyBefore True to clear the given variable \p out before
	** \return True if the operation succeeded, false otherwise
	*/
	bool Fonts(String& out, bool emptyBefore = true);
	/*!
	** \brief Get the system fonts folder [clob]
	**
	** \param[out] out Variable of type 'string'/'container' where the value will be appended
	** \param emptyBefore True to clear the given variable \p out before
	** \return True if the operation succeeded, false otherwise
	*/
	bool Fonts(Clob& out, bool emptyBefore = true);





} // namespace System
} // namespace Directory
} // namespace IO
} // namespace Yuni
