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

		# if LIBAVFORMAT_VERSION_MAJOR < 53
		::av_close_input_file(pFormat);
		# else
		::avformat_close_input(&pFormat);
		# endif // LIBAVFORMAT_VERSION_MAJOR < 53
	}


	uint File::duration() const
	{
		assert(pFormat);
		return pFormat->duration / AV_TIME_BASE;
	}


} // namespace Media
} // namespace Private
} // namespace Yuni
