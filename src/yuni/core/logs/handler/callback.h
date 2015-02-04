#pragma once
#include "../null.h"
#include "../../event/event.h"


namespace Yuni
{
namespace Logs
{

	/*!
	** \brief Log Handler: The standard output (cout & cerr)
	*/
	template<class NextHandler = NullHandler>
	class YUNI_DECL Callback : public NextHandler
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
				if (not callback.empty() and not s.empty())
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

