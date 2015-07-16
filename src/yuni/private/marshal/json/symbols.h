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
#ifndef __GRAMMAR_PARSER_JSON_PARSER_SYMBOLS_H__
# define __GRAMMAR_PARSER_JSON_PARSER_SYMBOLS_H__


// Generated by grammar-converter



namespace Grammar
{
namespace JSON
{

	/*!
	** \brief All IDs for symbols
	*/
	enum Symbol
	{
		symUnknown = (uint) -1,
		//! EOF
		symNTEOF = 0,
		//! Error
		symNTError = 1,
		//! Whitespace
		symNTWhitespace = 2,
		//! ,
		symComma = 3,
		//! :
		symColon = 4,
		//! [
		symBracketLeft = 5,
		//! ]
		symBracketRight = 6,
		//! {
		symCurlyBracketLeft = 7,
		//! }
		symCurlyBracketRight = 8,
		//! false
		symTFalse = 9,
		//! null
		symTNull = 10,
		//! Number
		symTNumber = 11,
		//! String
		symTString = 12,
		//! true
		symTTrue = 13,
		//! Array
		symNTArray = 14,
		//! Elements
		symNTElements = 15,
		//! Json
		symNTJson = 16,
		//! Members
		symNTMembers = 17,
		//! Object
		symNTObject = 18,
		//! Pair
		symNTPair = 19,
		//! Value
		symNTValue = 20,
		//! The maximum number of symbols,
		symMax = 21,

	}; // enum







} // namespace JSON
} // namespace Grammar

#endif // __GRAMMAR_PARSER_JSON_PARSER_SYMBOLS_H__
