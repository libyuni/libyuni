#ifndef __YUNI_CORE_LOGS_DECORATORS_MESSAGE_H__
# define __YUNI_CORE_LOGS_DECORATORS_MESSAGE_H__

# include "../null.h"
# include "../../system/console.h"


namespace Yuni
{
namespace Logs
{


	template<class LeftType = NullDecorator>
	class Message : public LeftType
	{
	public:
		template<class Handler, class VerbosityType, class O>
		void internalDecoratorAddPrefix(O& out, const AnyString& s) const
		{
			// Write the message itself
			out.put(' ');

			// Color
			if (VerbosityType::messageColor != System::Console::none && Handler::colorsAllowed)
				System::Console::TextColor<VerbosityType::messageColor>::Set(out);

			// The message
			out.write(s.c_str(), (uint) s.sizeInBytes());

			// Resetting the color
			if (VerbosityType::messageColor != System::Console::none && Handler::colorsAllowed)
				System::Console::ResetTextColor(out);

			// Transmit the message to the next handler
			LeftType::template internalDecoratorAddPrefix<Handler, VerbosityType,O>(out, s);
		}

	}; // class VerbosityLevel





} // namespace Logs
} // namespace Yuni

#endif // __YUNI_CORE_LOGS_DECORATORS_MESSAGE_H__
