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
# include "file.h"
# include <cassert>

namespace Yuni
{
namespace Private
{
namespace Media
{


	File::File(const AnyString& filePath):
		pFormat(nullptr)
	{
		if (filePath.empty())
			return;

		# if LIBAVFORMAT_VERSION_MAJOR < 53
		if (::av_open_input_file(&pFormat, filePath.c_str(), nullptr, 0, nullptr))
		# else
		if (::avformat_open_input(&pFormat, filePath.c_str(), nullptr, nullptr))
		# endif // LIBAVFORMAT_VERSION_MAJOR < 53
		{
			pFormat = nullptr;
			return;
		}

		// After opening, we must search for the stream information because not
		// all formats will have it in stream headers (eg. system MPEG streams)
		# if LIBAVFORMAT_VERSION_MAJOR < 53
		if (::av_find_stream_info(pFormat) < 0)
		# else
		if (::avformat_find_stream_info(pFormat, nullptr) < 0)
		# endif // LIBAVFORMAT_VERSION_MAJOR < 53
		{
			# if LIBAVFORMAT_VERSION_MAJOR < 53
			::av_close_input_file(pFormat);
			# else
			::avformat_close_input(&pFormat);
			# endif // LIBAVFORMAT_VERSION_MAJOR < 53
			pFormat = nullptr;
			return;
		}
	}


	File::~File()
	{
		pAStreams.clear();
		pVStreams.clear();

		if (pFormat)
		{
			# if LIBAVFORMAT_VERSION_MAJOR < 53
			::av_close_input_file(pFormat);
			# else
			::avformat_close_input(&pFormat);
			# endif // LIBAVFORMAT_VERSION_MAJOR < 53
		}
	}


	uint File::duration() const
	{
		assert(pFormat and "invalid format");
		return pFormat ? (uint)(pFormat->duration / AV_TIME_BASE) : 0;
	}


	void File::rewind()
	{
		assert(pFormat and "invalid format");

		for (auto stream : pVStreams)
			stream.second->rewind();
		for (auto stream : pAStreams)
			stream.second->rewind();
	}




} // namespace Media
} // namespace Private
} // namespace Yuni

