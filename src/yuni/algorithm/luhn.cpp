/*
** YUNI's default license is the GNU Lesser Public License (LGPL), with some
** exclusions (see below). This basically means that you can get the full source
** code for nothing, so long as you adhere to a few rules.
**
** Under the LGPL you may use YUNI for any purpose you wish, and modify it if you
** require, as long as you:
**
** Pass on the (modified) YUNI source code with your software, with original
** copyrights intact :
**  * If you distribute electronically, the source can be a separate download
**    (either from your own site if you modified YUNI, or to the official YUNI
**    website if you used an unmodified version) – just include a link in your
**    documentation
**  * If you distribute physical media, the YUNI source that you used to build
**    your application should be included on that media
** Make it clear where you have customised it.
**
** In addition to the LGPL license text, the following exceptions / clarifications
** to the LGPL conditions apply to YUNI:
**
**  * Making modifications to YUNI configuration files, build scripts and
**    configuration headers such as yuni/platform.h in order to create a
**    customised build setup of YUNI with the otherwise unmodified source code,
**    does not constitute a derived work
**  * Building against YUNI headers which have inlined code does not constitute a
**    derived work
**  * Code which subclasses YUNI classes outside of the YUNI libraries does not
**    form a derived work
**  * Statically linking the YUNI libraries into a user application does not make
**    the user application a derived work.
**  * Using source code obsfucation on the YUNI source code when distributing it
**    is not permitted.
** As per the terms of the LGPL, a "derived work" is one for which you have to
** distribute source code for, so when the clauses above define something as not
** a derived work, it means you don't have to distribute source code for it.
** However, the original YUNI source code with all modifications must always be
** made available.
*/
#include "luhn.h"
#include "../core/string.h"


namespace Yuni
{
namespace Algorithm
{

	int Luhn::Mod10(const AnyString& s)
	{
		// The string must have at least one char
		if (s.size() > 1)
		{
			// The algorithm :
			// 1 - Counting from the check digit, which is the rightmost, and moving
			//     left, double the value of every second digit.
			// 2 - Sum the digits of the products together with the undoubled digits
			//     from the original number.
			// 3 - If the total ends in 0 (put another way, if the total modulo 10 is
			//     congruent to 0), then the number is valid according to the Luhn formula
			//
			static const int prefetch[] = {0, 2, 4, 6, 8, 1, 3, 5, 7, 9};

			int sum = 0;
			bool alternate = true;

			const AnyString::iterator end = s.end();
			// For each char
			for (AnyString::iterator i = s.begin(); end != i; ++i)
			{
				// Each char in the string must be a digit
				if (!String::IsDigit(i.value()))
					return false;
				// The `real` digit
				int n = i.value() - '0';
				// Computing the sum
				sum += (alternate = !alternate) ? prefetch[n] : n;
			}
			return sum % 10;
		}
		return -1;
	}



} // namespace Algorithm
} // namespace Yuni


