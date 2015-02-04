#pragma once
#include "../../yuni.h"
#include "../string.h"


namespace Yuni
{
namespace Process
{

	/*!
	** \brief Try to rename the process name
	**
	** This routine does nothing on Windows
	** This routine is an implementation of setproctitle
	*/
	YUNI_DECL char** Rename(int argc, char** argv, const AnyString& newprocessname);





} // namespace Process
} // namespace Yuni

