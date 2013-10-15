#ifndef __YUNI_CORE_LOGS_HANDLERS_CALLBACK_H__
# define __YUNI_CORE_LOGS_HANDLERS_CALLBACK_H__

# include "../null.h"
# include "../../event/event.h"


namespace Yuni
{
namespace Logs
{

	/*!
	** \brief Log Handler: The standard output (cout & cerr)
	*/
	template<class NextHandler = NullHandler>
	class Callback : public NextHandler
	{
	public:
		enum Settings
		{
			// Colors are not allowed in a file
			unixColorsAllowed = 0,
		};

	public:
		template<class LoggerT, class VerbosityType>
		void internalDecoratorWriteWL(LoggerT& logger, const AnyString& s) const
		{
			if ((uint) VerbosityType::level != (uint) Verbosity::Debug::level)
			{
				if (not callback.empty() && not s.empty())
				{
					// A mutex is already locked
					pDispatchedMessage = s;
					callback(VerbosityType::level, pDispatchedMessage);
				}
			}

			// Transmit the message to the next handler
			NextHandler::template internalDecoratorWriteWL<LoggerT, VerbosityType>(logger, s);
		}


	public:
		mutable Yuni::Event<void (int, const String&)> callback;
		mutable String pDispatchedMessage;

	}; // class Callback





} // namespace Logs
} // namespace Yuni

#endif // __YUNI_CORE_LOGS_HANDLERS_CALLBACK_H__
