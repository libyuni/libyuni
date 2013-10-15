#ifndef __YUNI_DBI_UTILS_H__
# define __YUNI_DBI_UTILS_H__

# include "../yuni.h"
# include "../core/string.h"


namespace Yuni
{
namespace DBI
{

	/*!
	** \brief Check whether a string is an identifier or not
	*/
	bool IsValidIdentifier(AnyString text);




} // namespace DBI
} // namespace Yuni

#endif // __YUNI_DBI_UTILS_H__
