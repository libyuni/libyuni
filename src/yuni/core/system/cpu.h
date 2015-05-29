#pragma once
#include "../../yuni.h"



namespace Yuni
{
namespace System
{
namespace CPU
{

	/*!
	** \brief Get the number of system CPU
	**
	** \return The number of CPU (logic or not), 1 when the value is unknown
	*/
	YUNI_DECL uint Count();



} // namespace CPU
} // namespace System
} // namespace Yuni

