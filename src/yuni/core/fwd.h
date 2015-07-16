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
#pragma once
#include <iosfwd> // STL

#ifdef None
# undef None // From X11/X.h
#endif



namespace Yuni
{

	//! Type for a default behavior / policy
	struct /*YUNI_DECL*/ Default {};

	//! Absence Option
	struct /*YUNI_DECL*/ None {};


	// Forward declaration for the base class String
	template<uint ChunkSizeT = 128, bool ExpandableT = true>
	class /*YUNI_DECL*/ CString;


	//! Standard string implementation
	typedef /*YUNI_DECL*/ CString<> String;

	//! Character Large Object
	typedef /*YUNI_DECL*/ CString<4096>  Clob;

	//! Convenient alias for short string 16 bytes
	typedef /*YUNI_DECL*/  CString<16,false>  ShortString16;
	//! Convenient alias for short string 32 bytes
	typedef /*YUNI_DECL*/  CString<32,false>  ShortString32;
	//! Convenient alias for short string 64 bytes
	typedef /*YUNI_DECL*/  CString<64,false>  ShortString64;
	//! Convenient alias for short string 128 bytes
	typedef /*YUNI_DECL*/  CString<128,false>  ShortString128;
	//! Convenient alias for short string 256 bytes
	typedef /*YUNI_DECL*/  CString<256,false>  ShortString256;
	//! Convenient alias for short string 512 bytes
	typedef /*YUNI_DECL*/  CString<512,false>  ShortString512;


} // namespace Yuni


//! Convenient typedef for cross namespace lookup
typedef Yuni::String  YString;

/*!
** \brief String adapters
**
** This is a convenient typedef for declaring a string adapter.
** A string adapter allow you to perform all read-only operations
** provided by a string to an arbitrary raw buffer, without copying it.
** This may be extremly useful to reduce memory consumption and to reduce
** some useless memory allocation.
**
** \code
** AnyString s;
** s.adapt("Here is a raw C-string");
** std::cout << "length     : " << s.size() << std::endl;
** std::cout << "find 'raw' : " << s.find("raw") << std::endl;
** \endcode
**
** Using a sub-string as it were a real string :
** \code
** String s = "abcdefghijklmnopqrst";
** AnyString adapter (s.begin() + 2, s.begin() + 9);
** std::cout << adapter << " (size: " << adapter.size() << ")" << std::endl;
** \endcode
*/
typedef ::Yuni::CString<0, true>  AnyString;


