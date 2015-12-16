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
#include "array.h"
#include <iostream>


namespace Yuni
{
namespace Bit
{

	namespace // anonymous
	{

		template<bool ValueT>
		static inline uint Find(const Bit::Array::BufferType& pBuffer, uint pCount, uint offset)
		{
			// bitmask
			static const uchar mask[] = { 128, 64, 32, 16, 8, 4, 2, 1 };
			// alias to npos
			enum { npos = Yuni::Bit::Array::npos };

			for (uint i = (offset >> 3); i < pBuffer.size(); ++i)
			{
				if (static_cast<uchar>(pBuffer[i]) != (ValueT ? static_cast<uchar>(0) : static_cast<uchar>(0xFF)))
				{
					const uchar c = static_cast<uchar>(pBuffer[i]);

					// trivial cases
					if (ValueT and c == 0xFF)
					{
						uint p = i * 8;
						p = (p < offset) ? offset : p;
						return (p < pCount) ? p : npos;
					}
					if (not ValueT and c == 0)
					{
						uint p = i * 8;
						p = (p < offset) ? offset : p;
						return (p < pCount) ? p : npos;
					}

					// the current absolute position
					const uint absOffset = i * 8;

					// bit scan
					if (absOffset < offset)
					{
						for (uint p = 0; p != 8; ++p)
						{
							if (ValueT == (0 != (c & mask[p])))
							{
								p += absOffset;
								if (p >= offset)
									return (p < pCount) ? p : npos;
								// restoring previous value
								p -= absOffset;
							}
						}
					}
					else
					{
						// TODO : we can use ffs here
						for (uint p = 0; p != 8; ++p)
						{
							if (ValueT == (0 != (c & mask[p])))
							{
								p += absOffset;
								return (p < pCount) ? p : npos;
							}
						}

					}
				}
			}
			return npos;
		}

	} // anonymous namespace



	template<> uint Array::find<true>(uint offset) const
	{
		return Find<true>(pBuffer, pCount, offset);
	}


	template<> uint Array::find<false>(uint offset) const
	{
		return Find<false>(pBuffer, pCount, offset);
	}



	bool Array::any() const
	{
		// try to find a byte not null
		for (uint i = 0; i != pBuffer.size(); ++i)
		{
			if (pBuffer[i] != 0)
				return true;
		}
		return false;
	}


	bool Array::none() const
	{
		for (uint i = 0; i != pBuffer.size(); ++i)
		{
			if (pBuffer[i] != 0)
				return false;
		}
		return true;
	}


	bool Array::all() const
	{
		for (uint i = 0; i != pBuffer.size(); ++i)
		{
			if (static_cast<uchar>(pBuffer[i]) != 0xFF)
				return false;
		}
		return true;
	}




} // namespace Bit
} // namespace Yuni

