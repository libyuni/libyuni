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
#include "../../yuni.h"



namespace Yuni
{
namespace UTF8
{

	enum Error
	{
		//! No error
		errNone = 0,
		//! Out of bound (offset >= size)
		errOutOfBound,
		//! Invalid lead character
		errInvalidLead,
		//! Not enough data
		errNotEnoughData,
		//! The sequence is incomplete
		errIncompleteSequence,
		errInvalidCodePoint,
	};


	class YUNI_DECL Char final
	{
	public:
		/*!
		** \brief Get the length in bytes of the UTF8 character
		**
		** This information is provided by the lead character (= the first char given by \p p)
		** \param p Address of a potential utf8 char
		** \return The size in bytes of the UTF8 char at the address \p p  (1,2,3, or 4, 0 if invalid).
		*/
		static uint Size(const void* p);

		/*!
		** \brief Extract the first uchar from a raw buffer
		** \param p A valid raw buffer
		*/
		static uchar Mask8Bits(const void* p);
		/*!
		** \brief Extract the first uchar from a raw buffer
		** \param p Any char
		*/
		static uchar Mask8Bits(const char p);

		/*!
		** \brief Is the UTF-8 a simple ascii char ?
		** \param c An unsigned char
		*/
		static bool IsASCII(uchar c);

		/*!
		** \brief Check if the two first bits are set
		** \param c An unsigned char
		*/
		static bool IsTrail(uchar c);

		enum
		{
			//! The maximum valid code point
			codePointMax = (uint32) 0x0010ffffu,
			//! Ascii maximum value
			asciiLimit = 0x80,
		};

	public:
		//! \name Constructors
		//@{
		//! Default Constructor
		Char();
		//! Copy constructor
		Char(const Char& rhs);
		//! Constructor from a mere char
		explicit Char(uint c);
		//@}


		/*!
		** \brief The size of the UTF8 character, in bytes
		*/
		uint size() const;

		uint32 value() const;

		template<class StreamT> void write(StreamT& out) const;

		void reset();

		//! Get if the character is a simple ascii
		bool isAscii() const;


		//! \name Operators
		//@{
		Char& operator = (const Char& rhs);
		Char& operator = (uint value);

		bool operator == (uint value) const;
		bool operator == (const Char& ch) const;
		bool operator == (const AnyString& str) const;

		bool operator != (uint value) const;
		bool operator != (const Char& ch) const;
		bool operator != (const AnyString& str) const;

		bool operator < (uint value) const;
		bool operator < (const Char& ch) const;

		bool operator <= (uint value) const;
		bool operator <= (const Char& ch) const;

		bool operator > (uint value) const;
		bool operator > (const Char& ch) const;

		bool operator >= (uint value) const;
		bool operator >= (const Char& ch) const;

		//! Cast into char
		operator char () const;

		//! Cast into unsigned char
		operator uchar () const;
		//@}


	private:
		//! The UTF-8 character
		uint32 pValue;
		// A friend !
		template<uint, bool> friend class Yuni::CString;
	};





} // namespace UTF8
} // namespace Yuni


bool operator == (const AnyString& multiByteStr, Yuni::UTF8::Char c);

bool operator != (const AnyString& multiByteStr, Yuni::UTF8::Char c);


#include "utf8char.hxx"
