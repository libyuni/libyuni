#ifndef __YUNI_CORE_LOGS_DECORATORS_VERBOSITY_LEVEL_H__
# define __YUNI_CORE_LOGS_DECORATORS_VERBOSITY_LEVEL_H__

# include "../null.h"
# include "../../system/console.h"


namespace Yuni
{
namespace Logs
{

	template<class LeftType = NullDecorator>
	class VerbosityLevel : public LeftType
	{
	public:
		template<class Handler, class VerbosityType, class O>
		void internalDecoratorAddPrefix(O& out, const AnyString& s) const
		{
			// Write the verbosity to the output
			if (VerbosityType::hasName)
			{
				// Set Color
				if (Handler::colorsAllowed && VerbosityType::color != System::Console::none)
					System::Console::TextColor<VerbosityType::color>::Set(out);
				// The verbosity
				VerbosityType::AppendName(out);
				// Reset Color
				if (Handler::colorsAllowed && VerbosityType::color != System::Console::none)
					System::Console::ResetTextColor(out);
			}
			// Transmit the message to the next decorator
			LeftType::template internalDecoratorAddPrefix<Handler, VerbosityType,O>(out, s);
		}

	}; // class VerbosityLevel





} // namespace Logs
} // namespace Yuni

#endif // __YUNI_CORE_LOGS_DECORATORS_VERBOSITY_LEVEL_H__
