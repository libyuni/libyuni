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
#include "hexdump.h"



namespace Yuni
{
namespace Core
{
namespace Utils
{

	inline Hexdump::Hexdump(const char* buffer, uint size)
		: pBuffer(buffer), pSize(size)
	{}


	inline Hexdump::Hexdump(const Hexdump& rhs)
		: pBuffer(rhs.pBuffer), pSize(rhs.pSize)
	{}


	template<class U>
	inline Hexdump::Hexdump(const U& buffer)
		: pBuffer((const char *)buffer.data()), pSize(buffer.sizeInBytes())
	{}


	template <class U>
	void Hexdump::dump(U& stream) const
	{
		Yuni::String line;
		uint printed;
		uint remains = pSize;

		for (printed = 0; printed <	pSize; printed += 0x10)
		{
			remains = pSize - printed;

			// Print the line's address
			line.appendFormat("%08.8p: ", (pBuffer + printed));

			// Print the next 16 bytes (or less) in hex.
			dumpHexadecimal(line, pBuffer + printed, (remains > 0x10) ? 0x10 : remains);

			// Print the next 16 bytes (or less) in printable chars.
			dumpPrintable(line, pBuffer + printed, (remains > 0x10) ? 0x10 : remains);

			// Add the position in the buffer, padded to 2 bytes.
			line.appendFormat(" %04.4x-%04.4x\n", printed, printed + 0x0f);

			// Put the line in the stream
			stream << line;
			line.clear();
		}
	}


	inline String Hexdump::dump() const
	{
		String s;
		dump(s);
		return s;
	}



} // namespace Utils
} // namespace Core
} // namespace Yuni




inline std::ostream& operator<< (std::ostream& outStream, const Yuni::Core::Utils::Hexdump& hexDumper)
{
	hexDumper.dump(outStream);
	return outStream;
}
