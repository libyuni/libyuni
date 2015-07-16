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
#ifndef __CPP_PARSER_JSON_PRODUCTIONS_HXX__
# define __CPP_PARSER_JSON_PRODUCTIONS_HXX__


// Generated by grammar-converter

# include <yuni/yuni.h>


namespace Grammar
{
namespace Private
{
namespace JSON
{
namespace Productions
{

	enum { count = 18 };


	static const uint symbolsForRule0[2] = {18,  (uint) -1 };
	static const uint symbolsForRule1[2] = {14,  (uint) -1 };
	static const uint symbolsForRule2[3] = {7, 8,  (uint) -1 };
	static const uint symbolsForRule3[4] = {7, 17, 8,  (uint) -1 };
	static const uint symbolsForRule4[2] = {19,  (uint) -1 };
	static const uint symbolsForRule5[4] = {19, 3, 17,  (uint) -1 };
	static const uint symbolsForRule6[4] = {12, 4, 20,  (uint) -1 };
	static const uint symbolsForRule7[3] = {5, 6,  (uint) -1 };
	static const uint symbolsForRule8[4] = {5, 15, 6,  (uint) -1 };
	static const uint symbolsForRule9[2] = {20,  (uint) -1 };
	static const uint symbolsForRule10[4] = {20, 3, 15,  (uint) -1 };
	static const uint symbolsForRule11[2] = {12,  (uint) -1 };
	static const uint symbolsForRule12[2] = {11,  (uint) -1 };
	static const uint symbolsForRule13[2] = {18,  (uint) -1 };
	static const uint symbolsForRule14[2] = {14,  (uint) -1 };
	static const uint symbolsForRule15[2] = {13,  (uint) -1 };
	static const uint symbolsForRule16[2] = {9,  (uint) -1 };
	static const uint symbolsForRule17[2] = {10,  (uint) -1 };




	struct Entry
	{
		//! Head symbol index
		const uint head;
		//! Total number of symbols
		const uint symbolCount;
		//! List of symbols
		const uint* symbols;
	};


	static const Entry table[count] = 
	{
		/* 0 */ { 16,  /*symbols*/ 1, symbolsForRule0 },
		/* 1 */ { 16,  /*symbols*/ 1, symbolsForRule1 },
		/* 2 */ { 18,  /*symbols*/ 2, symbolsForRule2 },
		/* 3 */ { 18,  /*symbols*/ 3, symbolsForRule3 },
		/* 4 */ { 17,  /*symbols*/ 1, symbolsForRule4 },
		/* 5 */ { 17,  /*symbols*/ 3, symbolsForRule5 },
		/* 6 */ { 19,  /*symbols*/ 3, symbolsForRule6 },
		/* 7 */ { 14,  /*symbols*/ 2, symbolsForRule7 },
		/* 8 */ { 14,  /*symbols*/ 3, symbolsForRule8 },
		/* 9 */ { 15,  /*symbols*/ 1, symbolsForRule9 },
		/* 10 */ { 15,  /*symbols*/ 3, symbolsForRule10 },
		/* 11 */ { 20,  /*symbols*/ 1, symbolsForRule11 },
		/* 12 */ { 20,  /*symbols*/ 1, symbolsForRule12 },
		/* 13 */ { 20,  /*symbols*/ 1, symbolsForRule13 },
		/* 14 */ { 20,  /*symbols*/ 1, symbolsForRule14 },
		/* 15 */ { 20,  /*symbols*/ 1, symbolsForRule15 },
		/* 16 */ { 20,  /*symbols*/ 1, symbolsForRule16 },
		/* 17 */ { 20,  /*symbols*/ 1, symbolsForRule17 },
	};







} // namespace Productions
} // namespace JSON
} // namespace Private
} // namespace Grammar

#endif // __CPP_PARSER_JSON_PRODUCTIONS_HXX__
