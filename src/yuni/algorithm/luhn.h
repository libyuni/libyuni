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
#ifndef __YUNI_ALGORITHM_LUHN_H__
# define __YUNI_ALGORITHM_LUHN_H__

# include "../yuni.h"
# include "../core/string/string.h"


namespace Yuni
{
namespace Algorithm
{


	/*!
	** \brief The Luhn algorithms (or Luhn formula)
	**
	** The Luhn algorithm is a simple checksum formula used to validate a variety of
	** identification numbers, such as credit card numbers.
	** It was created by IBM scientist Hans Peter Luhn and described in U.S. Patent
	** 2,950,048, filed on January 6, 1954, and granted on August 23, 1960.
	**
	** it was designed to protect against accidental errors, not malicious attacks
	** and it is a simple method for distinguishing valid numbers from collections
	** of random digits.
	*/
	class Luhn
	{
	public:
		/*!
		** \brief Checks whether a string of digits is a valid credit card number
		**
		** This method uses the `Luhn Mod 10` algorithm.
		** \code
		** // A valid credit card number
		** std::cout << Yuni::Algorithm::Luhn::IsValidCreditCardNumber("49927398716") << std::endl;
		** \endcode
		**
		** \param s A string with an arbitrary length (> 1, returns false otherwise)
		** \return True if the string is a valid credit card number, False otherwise
		*/
		static bool IsValidCreditCardNumber(const AnyString& s);

	public:
		/*!
		** \brief Generate the check digit of a string using the `Luhn mode 10` algorithm
		**
		** \param s A string with an arbitrary length (> 1, error otherwise)
		** \return The check digit, -1 if an error has occured
		*/
		static int Mod10(const AnyString& s);


		/*!
		** \brief Generate the check character of a string using the `Luhn mode N` algorithm
		**
		** TODO !
		** A mapping between valid input characters and code-points is needed, to
		** specify what characters are valid, and the numerical order of those characters.
		**
		** \code
		** static int CodePointsHexa(const char c)
		** {
		** 	switch (c)
		** 	{
		** 		case '0' : return 0;
		** 		case '1' : return 1;
		** 		case '2' : return 2;
		** 		case '3' : return 3;
		** 		case '4' : return 4;
		** 		case '5' : return 5;
		** 		case '6' : return 6;
		** 		case '7' : return 7;
		** 		case '8' : return 8;
		** 		case '8' : return 8;
		** 		case '8' : return 8;
		** 	}
		** }
		**
		** int main(void)
		** {
		** 	char check = Yuni::Algorithms::Luhn::ModN("a340bd2f", &CodePointsHexa);
		** 	std::
		** 	return 0;
		** }
		** \endcode
		**
		** \param s A string with an arbitrary length (> 1, error otherwise)
		** \return The check digit, -1 if an error has occured
		*/
		//! TODO Need implementation for `Luhn Mod N`
		//		static int ModN(const AnyStringT& s, int (*codepoint)(const StringT::Char));

	}; // class Luhn





} // namespace Algorithm
} // namespace Yuni

# include "luhn.hxx"

#endif // __YUNI_ALGORITHM_LUHN_H__
