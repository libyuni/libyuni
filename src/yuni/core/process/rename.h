#ifndef __YUNI_CORE_PROCESS_RENAME_H__
# define __YUNI_CORE_PROCESS_RENAME_H__

# include "../../yuni.h"
# include "../string.h"


namespace Yuni
{

	/*!
	** \brief Try to rename the process name
	**
	** This routine does nothing on Windows
	** This routine is an implementation of setproctitle
	*/
	char** RenameProcess(int argc, char** argv, const AnyString& newprocessname);



} // namespace Yuni

#endif // __YUNI_CORE_PROCESS_RENAME_H__
