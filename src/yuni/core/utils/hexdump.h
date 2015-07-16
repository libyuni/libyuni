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
#include "../string.h"



namespace Yuni
{
namespace Core
{
namespace Utils
{

	/*!
	** \brief A simple hexadecimal buffer dumper.
	**
	** This dumper can dump any buffer (with start address and length),
	** or any class having a data() and size() member function.
	** (for example, String).
	** The output looks like the following:
	**
	** Sample usage:
	** \code
	** // Either call it with a memory buffer
	** std::cout << Core::Utils::Hexdump(myBuffer);
	**
	** // Or with a plain old char buffer.
	** char buf[256];
	** std::cout << Core::Utils::Hexdump(buf, sizeof(buf));
	** \endcode
	**
	** Sample output:
	** \code
	** 0x00604010: 0001 0203 0405 0607 0809 0a0b 0c0d 0e0f |................|
	** 0x00604020: 1011 1213 1415 1617 1819 1a1b 1c1d 1e1f |................|
	** 0x00604030: 2021 2223 2425 2627 2829 2a2b 2c2d 2e2f | !"#$%&'()*+,-./|
	** 0x00604040: 3031 3233 3435 3637 3839 3a3b 3c3d 3e3f |0123456789:;<=>?|
	** 0x00604050: 4041 4243 4445 4647 4849 4a4b 4c4d 4e4f |@ABCDEFGHIJKLMNO|
	** \endcode
	*/
	class YUNI_DECL Hexdump final
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Construct from a simple buffer.
		**
		** \param[in] buffer A pointer to a raw buffer.
		** \param[in] size The size to dump.
		*/
		Hexdump(const char* buffer, uint size);

		/*!
		** \brief Copy ctor.
		**
		** \param[in] rhs The original Hexdump instance
		*/
		Hexdump(const Hexdump& rhs);

		/*!
		** \brief Construct from a string (or like) object.
		**
		** \param[in] buffer The stream to output the hexdump on.
		*/
		template<class U> Hexdump(const U& buffer);
		//@}


		//! \name Dump functions
		//@{
		/*!
		** \brief Dumps the current buffer to a stream.
		** \param[in,out] outStream The buffer will dumped to this stream.
		*/
		template<class U> void dump(U& outStream) const;

		/*!
		** \brief Dumps the current buffer to a string.
		** \return The dump contents.
		*/
		String dump() const;
		//@}

		//! \name Operators
		//@{
		/*!
		** \brief Operator =
		** \param[in] rhs The source Hexdump instance
		*/
		Hexdump& operator = (const Hexdump& rhs);
		//@}


	private:
		/*!
		** \brief Dumps the hexadecimal-version of a sub-buffer into a string
		**
		** \param[in] line The string to append to.
		** \param[in] buffer The start address of the sub-buffer.
		** \param[in] size The size of the sub-buffer
		*/
		void dumpHexadecimal(String& line, const char* buffer, uint size) const;

		/*!
		** \brief Dumps the printable-version of a sub-buffer into a string
		**
		** \param[in] line The string to append to.
		** \param[in] buffer The start address of the sub-buffer.
		** \param[in] size The size of the sub-buffer
		*/
		void dumpPrintable(String& line, const char* buffer, uint size) const;

	private:
		//! Pointer to the current buffer (not owned by Hexdump)
		char const* pBuffer;
		//! The current dump size.
		uint pSize;

	}; // class HexDump






} // namespace Utils
} // namespace Core
} // namespace Yuni



//! Operator overloads for printing
//@{
/*!
** \brief std::ostring print operator
** \param[in] outStream The stream to output the hexdump on.
** \param[in] hexDumper The Hexdump instance to dump.
*/
std::ostream& operator<< (std::ostream& outStream, const Yuni::Core::Utils::Hexdump& hexDumper);
//@}


#include "hexdump.hxx"
