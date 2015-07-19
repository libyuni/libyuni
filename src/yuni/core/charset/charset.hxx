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
#include "charset.h"



namespace Yuni
{
namespace Charset
{

	template <typename SrcStringT, typename DtsStringT>
	bool Converter::convert(SrcStringT& source, DtsStringT& destination, bool isLastBuffer)
	{
		// Since the source and destination length may not be the same,
		// and the algorithm to predict the destination size depends on
		// the conversion performed, and since we want to perform complete
		// conversions, we require an expandable string as destination.
		YUNI_STATIC_ASSERT(DtsStringT::expandable == 1, CharsetConverterRequiresExpandableBuffers);
		enum
		{
			DSTSZ = sizeof(typename DtsStringT::Type),
			SRCSZ = sizeof(typename SrcStringT::Type),
		};

		/*
		{
			String dbg;
			dbg.appendFormat("SRC[@%p, capa = %d, size = %d]\n",
							 source.data(),
							 source.capacityInBytes(),
							 source.sizeInBytes());
			dbg.appendFormat("DST[@%p, capa = %d, size = %d]\n",
							 destination.data(),
							 destination.capacityInBytes(),
							 destination.sizeInBytes());

			std::cout << dbg;
		}
		*/

		/*
		** Reserve at least the exact sufficient space.
		** We take the size of the source in bytes and we resize the destination
		** so that it contains at least the same number of bytes plus its original
		** size.
		*/
		destination.reserve(destination.size()
							+ (source.sizeInBytes() / DSTSZ) + 1);

		/*
		{
			// Debug this.
			std::cout << "Reserving => " 
				<< (source.sizeInBytes() / DSTSZ) + 1
				<< " more elements, for "
				<<  source.sizeInBytes()
				<< " more bytes \n";


			String dbg;
			dbg.appendFormat("SRC[@%p, capa = %d, size = %d]\n",
							 source.data(),
							 source.capacityInBytes(),
							 source.sizeInBytes());
			dbg.appendFormat("DST[@%p, capa = %d, size = %d]\n",
							 destination.data(),
							 destination.capacityInBytes(),
							 destination.sizeInBytes());

			std::cout << dbg;
		}
		*/


		// The source data buffer is taken as-is.
		char* srcData = reinterpret_cast<char *>(source.data());
		size_t srcSize = source.sizeInBytes();



		bool retry;
		do
		{
			retry = false;

			/*
			** We must take a pointer to the position just after the current
			** content of the buffer.
			**
			** We also need to compute the remaining size in the buffer:
			**  = total capacity - current size
			*/
			char* dstData = reinterpret_cast<char*>(destination.data()) + destination.sizeInBytes();
			size_t dstSize = destination.capacityInBytes() - destination.sizeInBytes();

			/*
			{
				String dbg;

				dbg.appendFormat("Before iconv() : SRC[@%p, remain = %d] DST[@%p, remain = %d]\n",
								 srcData, srcSize, dstData, dstSize);
				std::cout << dbg;
			}
			*/

			// Try a conversion pass.
			size_t result = IconvWrapper(pContext, &srcData, &srcSize, &dstData, &dstSize);

			/*
			{
				String dbg;

				dbg.appendFormat("After iconv()  : SRC[@%p, remain = %d] DST[@%p, remain = %d]\n",
								 srcData, srcSize, dstData, dstSize);
				std::cout << dbg;
			}
			*/

			/*
			** Next, resize the destination buffer to contain every character put in it
			** by iconv(). We resize it to the converted length.
			** Beware of the Memory buffer data type. Incomplete elements will be truncated,
			** so don't store UTF-8 in wchar_ts.
			**
			** This will probably destroy the destination.data(), so we must reset it at the beginning.
			*/
			destination.resize((dstData - reinterpret_cast<char *>(destination.data())) / DSTSZ);


			if ((size_t)-1 == result)
			{
				// Now we will treat the common error cases.
				switch (errno)
				{
					case E2BIG:
						/*
						** The destination buffer is too small, we must augment it.
						** For the time being, we augment the buffer by the size of
						** the original source buffer.
						**
						** The destination has been resized, so the new size() includes
						** the already converted data.
						*/
						destination.reserve(destination.size()
											+ (source.sizeInBytes() / DSTSZ) + 1);
						retry = true;
						break;
					case EILSEQ:
						// Fixme: handle illegal sequences
						break;
					case EINVAL:
						// Fixme: handle incomplete sequences differently depending
						// on the fact that this is the last part of the conversion
						break;
					default:
						// Fixme: handle unknown error codes
						break;
				}
			}
		} while (retry);

		/*
		** Consume the fully converted elements from the source buffer.
		** The incompletely consumed elements should not be problematic, i think.
		** They would result from incoherences in the input types and encodings
		** (utf8 string in wchar_t memory buffer for example).
		** Anyways, this is a lead in case you have a strange bug.
		*/
		source.consume((srcData - reinterpret_cast<char *>(source.data())) / SRCSZ);

		return true;
	}


	inline int Converter::lastError() const
	{
		return pLastError;
	}


	inline bool Converter::valid() const
	{
		return ((void*)-1 != pContext);
	}




} // namespace Charset
} // namespace Yuni
