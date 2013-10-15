#ifndef __YUNI_IO_IO_H__
# define __YUNI_IO_IO_H__

# include "../yuni.h"
# include "../core/string.h"
# include "error.h"
# include "constants.h"



namespace Yuni
{
/*
** \brief Low-level routines for file and directory support
*/
namespace IO
{


	/*!
	** \brief Test if a node exists (whatever its type, a folder or a file)
	**
	** \param filename The file/directory to test
	** \return True if it exists, false otherwise
	*/
	bool Exists(const AnyString& filename);

	/*!
	** \brief Get the type of a node
	**
	** \param filename The file/directory to test
	** \return True if it exists, false otherwise
	*/
	NodeType TypeOf(const AnyString& filename);





} // namespace IO
} // namespace Yuni

# include "filename-manipulation.h"
# include "directory.h"
# include "io.hxx"

#endif // __YUNI_IO_IO_H__
