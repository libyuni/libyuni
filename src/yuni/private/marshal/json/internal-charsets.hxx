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
#ifndef __CPP_PARSER_CHARSETS_HXX__
# define __CPP_PARSER_CHARSETS_HXX__

// Generated by grammar-converter


# include <yuni/yuni.h>
# include <yuni/core/string/utf8char.h>


namespace Grammar
{
namespace Private
{
namespace JSON
{
namespace Charset
{

	enum { count = 28 };


	struct Entry
	{
		//! All ascii characters for the current table
		const char* const ascii;
		//! characters count
		const uint  asciiSize;
		//! Allow UTF8 characterst
		const bool  utf8chars;
	};



	/*!
	** \brief All charset tables
	*/
	static const Entry table[count] =
	{
		{ // CHARSET INDEX 0
			/* ascii */ "\t\n\013\014\r ",
			/* size  */ 6,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 1
			/* ascii */ ",",
			/* size  */ 1,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 2
			/* ascii */ ":",
			/* size  */ 1,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 3
			/* ascii */ "[",
			/* size  */ 1,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 4
			/* ascii */ "]",
			/* size  */ 1,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 5
			/* ascii */ "{",
			/* size  */ 1,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 6
			/* ascii */ "}",
			/* size  */ 1,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 7
			/* ascii */ "f",
			/* size  */ 1,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 8
			/* ascii */ "n",
			/* size  */ 1,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 9
			/* ascii */ "-",
			/* size  */ 1,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 10
			/* ascii */ "0",
			/* size  */ 1,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 11
			/* ascii */ "123456789",
			/* size  */ 9,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 12
			/* ascii */ "\"",
			/* size  */ 1,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 13
			/* ascii */ "t",
			/* size  */ 1,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 14
			/* ascii */ "a",
			/* size  */ 1,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 15
			/* ascii */ "l",
			/* size  */ 1,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 16
			/* ascii */ "s",
			/* size  */ 1,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 17
			/* ascii */ "e",
			/* size  */ 1,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 18
			/* ascii */ "u",
			/* size  */ 1,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 19
			/* ascii */ ".",
			/* size  */ 1,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 20
			/* ascii */ "Ee",
			/* size  */ 2,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 21
			/* ascii */ "0123456789",
			/* size  */ 10,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 22
			/* ascii */ "+-",
			/* size  */ 2,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 23
			/* ascii */ "\032\033\034\035\036\037 !#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~\0177\00\01\02\03\04\05\06\07\010\t\n\013\014\r\016\017\020\021\022\023\024\025\026\027\030\031\032\033\034\035\036\037 !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\0177",
			/* size  */ 228,
			/* utf8  */ true
		},
		{ // CHARSET INDEX 24
			/* ascii */ "\\",
			/* size  */ 1,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 25
			/* ascii */ "\"/\\bfnrt",
			/* size  */ 8,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 26
			/* ascii */ "0123456789ABCDEFabcdef",
			/* size  */ 22,
			/* utf8  */ false
		},
		{ // CHARSET INDEX 27
			/* ascii */ "r",
			/* size  */ 1,
			/* utf8  */ false
		},
	};





} // namespace Charset
} // namespace JSON
} // namespace Private
} // namespace Grammar

#endif // __CPP_PARSER_JSON_CHARSETS_HXX__
