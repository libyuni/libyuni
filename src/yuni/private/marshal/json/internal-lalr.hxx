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
#ifndef __CPP_PARSER_JSON_LALR_HXX__
# define __CPP_PARSER_JSON_LALR_HXX__

// Generated by grammar-converter


# include <yuni/yuni.h>


namespace Grammar
{
namespace Private
{
namespace JSON
{
namespace LALR
{

	enum { count = 28 };


	enum ActionType
	{
		shift,
		reduce,
		goTo,
		accept,
		syntaxError,
	};



	struct Action
	{
		//! Entry symbol
		uint symbol;
		//! Action
		ActionType action;
		/*!
		** \brief Next state
		**
		** if action == shift then lalr
		** if action == reduce then production
		** if action == goto then lalr
		*/
		uint nextState;
	};




	static const Action symbolsForLALR0[5] = {
		{    5, shift,   1 },
		{    7, shift,   2 },
		{   14, goTo,    3 },
		{   16, goTo,    4 },
		{   18, goTo,    5 },
	};

	static const Action symbolsForLALR1[12] = {
		{    5, shift,   1 },
		{    6, shift,   6 },
		{    7, shift,   2 },
		{    9, shift,   7 },
		{   10, shift,   8 },
		{   11, shift,   9 },
		{   12, shift,   10 },
		{   13, shift,   11 },
		{   14, goTo,    12 },
		{   15, goTo,    13 },
		{   18, goTo,    14 },
		{   20, goTo,    15 },
	};

	static const Action symbolsForLALR2[4] = {
		{    8, shift,   16 },
		{   12, shift,   17 },
		{   17, goTo,    18 },
		{   19, goTo,    19 },
	};

	static const Action symbolsForLALR3[1] = {
		{    0, reduce,  1 },
	};

	static const Action symbolsForLALR4[1] = {
		{    0, accept,  0 },
	};

	static const Action symbolsForLALR5[1] = {
		{    0, reduce,  0 },
	};

	static const Action symbolsForLALR6[4] = {
		{    0, reduce,  7 },
		{    3, reduce,  7 },
		{    6, reduce,  7 },
		{    8, reduce,  7 },
	};

	static const Action symbolsForLALR7[3] = {
		{    3, reduce,  16 },
		{    6, reduce,  16 },
		{    8, reduce,  16 },
	};

	static const Action symbolsForLALR8[3] = {
		{    3, reduce,  17 },
		{    6, reduce,  17 },
		{    8, reduce,  17 },
	};

	static const Action symbolsForLALR9[3] = {
		{    3, reduce,  12 },
		{    6, reduce,  12 },
		{    8, reduce,  12 },
	};

	static const Action symbolsForLALR10[3] = {
		{    3, reduce,  11 },
		{    6, reduce,  11 },
		{    8, reduce,  11 },
	};

	static const Action symbolsForLALR11[3] = {
		{    3, reduce,  15 },
		{    6, reduce,  15 },
		{    8, reduce,  15 },
	};

	static const Action symbolsForLALR12[3] = {
		{    3, reduce,  14 },
		{    6, reduce,  14 },
		{    8, reduce,  14 },
	};

	static const Action symbolsForLALR13[1] = {
		{    6, shift,   20 },
	};

	static const Action symbolsForLALR14[3] = {
		{    3, reduce,  13 },
		{    6, reduce,  13 },
		{    8, reduce,  13 },
	};

	static const Action symbolsForLALR15[2] = {
		{    3, shift,   21 },
		{    6, reduce,  9 },
	};

	static const Action symbolsForLALR16[4] = {
		{    0, reduce,  2 },
		{    3, reduce,  2 },
		{    6, reduce,  2 },
		{    8, reduce,  2 },
	};

	static const Action symbolsForLALR17[1] = {
		{    4, shift,   22 },
	};

	static const Action symbolsForLALR18[1] = {
		{    8, shift,   23 },
	};

	static const Action symbolsForLALR19[2] = {
		{    3, shift,   24 },
		{    8, reduce,  4 },
	};

	static const Action symbolsForLALR20[4] = {
		{    0, reduce,  8 },
		{    3, reduce,  8 },
		{    6, reduce,  8 },
		{    8, reduce,  8 },
	};

	static const Action symbolsForLALR21[11] = {
		{    5, shift,   1 },
		{    7, shift,   2 },
		{    9, shift,   7 },
		{   10, shift,   8 },
		{   11, shift,   9 },
		{   12, shift,   10 },
		{   13, shift,   11 },
		{   14, goTo,    12 },
		{   15, goTo,    25 },
		{   18, goTo,    14 },
		{   20, goTo,    15 },
	};

	static const Action symbolsForLALR22[10] = {
		{    5, shift,   1 },
		{    7, shift,   2 },
		{    9, shift,   7 },
		{   10, shift,   8 },
		{   11, shift,   9 },
		{   12, shift,   10 },
		{   13, shift,   11 },
		{   14, goTo,    12 },
		{   18, goTo,    14 },
		{   20, goTo,    26 },
	};

	static const Action symbolsForLALR23[4] = {
		{    0, reduce,  3 },
		{    3, reduce,  3 },
		{    6, reduce,  3 },
		{    8, reduce,  3 },
	};

	static const Action symbolsForLALR24[3] = {
		{   12, shift,   17 },
		{   17, goTo,    27 },
		{   19, goTo,    19 },
	};

	static const Action symbolsForLALR25[1] = {
		{    6, reduce,  10 },
	};

	static const Action symbolsForLALR26[2] = {
		{    3, reduce,  6 },
		{    8, reduce,  6 },
	};

	static const Action symbolsForLALR27[1] = {
		{    8, reduce,  5 },
	};




	struct Entry
	{
		//! Total number of symbols
		const uint actionCount;
		//! List of symbols
		const Action* actions;
	};

	static const Entry table[count] = 
	{
		/*    0 */ {    5, symbolsForLALR0 },
		/*    1 */ {   12, symbolsForLALR1 },
		/*    2 */ {    4, symbolsForLALR2 },
		/*    3 */ {    1, symbolsForLALR3 },
		/*    4 */ {    1, symbolsForLALR4 },
		/*    5 */ {    1, symbolsForLALR5 },
		/*    6 */ {    4, symbolsForLALR6 },
		/*    7 */ {    3, symbolsForLALR7 },
		/*    8 */ {    3, symbolsForLALR8 },
		/*    9 */ {    3, symbolsForLALR9 },
		/*   10 */ {    3, symbolsForLALR10 },
		/*   11 */ {    3, symbolsForLALR11 },
		/*   12 */ {    3, symbolsForLALR12 },
		/*   13 */ {    1, symbolsForLALR13 },
		/*   14 */ {    3, symbolsForLALR14 },
		/*   15 */ {    2, symbolsForLALR15 },
		/*   16 */ {    4, symbolsForLALR16 },
		/*   17 */ {    1, symbolsForLALR17 },
		/*   18 */ {    1, symbolsForLALR18 },
		/*   19 */ {    2, symbolsForLALR19 },
		/*   20 */ {    4, symbolsForLALR20 },
		/*   21 */ {   11, symbolsForLALR21 },
		/*   22 */ {   10, symbolsForLALR22 },
		/*   23 */ {    4, symbolsForLALR23 },
		/*   24 */ {    3, symbolsForLALR24 },
		/*   25 */ {    1, symbolsForLALR25 },
		/*   26 */ {    2, symbolsForLALR26 },
		/*   27 */ {    1, symbolsForLALR27 },

	}; // table






} // namespace LALR
} // namespace JSON
} // namespace Private
} // namespace Grammar

#endif // __CPP_PARSER_JSON_LALR_HXX__
