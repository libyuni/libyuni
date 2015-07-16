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
#include <algorithm> // std::swap



namespace Yuni
{
namespace System
{

/*!
** \brief Endianness
** \ingroup Endianness
*/
namespace Endianness
{



	/*!
	** \brief Information about the CPU byte order
	** \ingroup Endianness
	*/
	enum ByteOrder
	{
		/*! Little-endian order */ LittleEndian = 0,
		/*! Big-endian order    */ BigEndian
	};



	/*!
	** \brief Determine the CPU byte order
	** \ingroup Endianness
	**
	** \return LittleEndian if the cpu seems to use the Little-endian order, BigEndian otherwise
	*/
	inline ByteOrder FindByteOrder()
	{
		static char big_endian_value_1[2] = { 0, 1 };
		return  ( (*((short*) big_endian_value_1)) == 1) ? BigEndian : LittleEndian;
	}



	/*!
	** \brief Swap bytes for a `N` bytes long variable
	** \ingroup Endianness
	**
	** \param[in,out] in A pointer to the variable to swap
	** \param size Size of the data
	*/
	inline void SwapBytes(char* in, const unsigned int size)
	{
		char* start = in;
		char* end = start + size - 1;
		while (start < end)
			std::swap(*start++,*end--);
	}


	/*!
	** \brief Swap bytes for a 2 bytes long variable
	** \ingroup Endianness
	**
	** \param[in,out] in The variable
	*/
	inline void SwapBytes2(char* in) { std::swap(in[0], in[1]); }


	/*!
	** \brief Swap bytes for a 4 bytes long variable
	** \ingroup Endianness
	**
	** \param[in,out] in The variable
	*/
	inline void SwapBytes4(char* in) { std::swap(in[0], in[3]); std::swap(in[1], in[2]); }


	/*!
	** \brief Swap bytes for a 8 bytes long variable
	** \ingroup Endianness
	**
	** \param[in,out] in The variable
	*/
	inline void SwapBytes8(char* in)
	{
		std::swap(in[0], in[7]);
		std::swap(in[1], in[6]);
		std::swap(in[2], in[5]);
		std::swap(in[3], in[4]);
	}


	/*!
	** \brief Swap bytes for a 16 bytes long variable
	** \ingroup Endianness
	**
	** \param[in,out] in The variable
	*/
	inline void SwapBytes16(char* in)
	{
		std::swap(in[0], in[15]);
		std::swap(in[1], in[14]);
		std::swap(in[2], in[13]);
		std::swap(in[3], in[12]);
		std::swap(in[4], in[11]);
		std::swap(in[5], in[10]);
		std::swap(in[6], in[9]);
		std::swap(in[7], in[8]);
	}






} // namespace Endian
} // namespace System
} // namespace Yuni

