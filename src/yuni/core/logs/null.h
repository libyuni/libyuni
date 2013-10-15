#ifndef __YUNI_CORE_LOGS_NULL_H__
# define __YUNI_CORE_LOGS_NULL_H__

# include "../../yuni.h"
# include "../string.h"


namespace Yuni
{
namespace Logs
{

	/*!
	** \brief Log Handler: The Null device
	**
	** This handler does produce nothing and skip all messages
	*/
	class NullHandler
	{
	public:
		template<class LoggerT, class VerbosityType>
		static void internalDecoratorWriteWL(LoggerT&, const AnyString&)
		{
			// Do nothing
		}
	};


	/*!
	** \brief Log Handler: The Null device
	**
	** This handler does produce nothing and skip all messages
	*/
	class NullDecorator
	{
	public:
		template<class Handler, class VerbosityType, class O>
		static void internalDecoratorAddPrefix(O&, const AnyString&)
		{
			// Do nothing
		}

	};




} // namespace Logs
} // namespace Yuni

#endif // __YUNI_CORE_LOGS_NULL_H__
