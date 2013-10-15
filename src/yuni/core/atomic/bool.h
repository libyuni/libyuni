#ifndef __YUNI_CORE_ATOMIC_BOOL_H__
# define __YUNI_CORE_ATOMIC_BOOL_H__

# include "../../yuni.h"
# include "int.h"



namespace Yuni
{
namespace Atomic
{

	/*!
	** \brief An atomic boolean type
	*/
	typedef Atomic::Int<1>  Bool;




} // namespace Atomic
} // namespace Yuni

#endif // __YUNI_CORE_ATOMIC_BOOL_H__
