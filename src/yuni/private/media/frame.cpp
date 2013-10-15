#include "frame.h"

# if (YUNI_OS_GCC_VERSION >= 40102)
#	pragma GCC diagnostic ignored "-Wconversion"
#	pragma GCC diagnostic ignored "-Wundef"
# endif

extern "C"
{
# include "libavutil/pixfmt.h"
# include "libavutil/pixdesc.h"
# include "libavcodec/avcodec.h"
# include "libavformat/avformat.h"
}


namespace Yuni
{
namespace Private
{
namespace Media
{


	// Hidden implementation for PImpl idiom
	class FrameImpl final
	{
	public:
		FrameImpl():
			frame(nullptr)
		{}
		~FrameImpl()
		{
			::av_free(frame);
		}

		// The AV Frame
		::AVFrame* frame;

	}; // class FrameImpl



	Frame::Frame(uint index, double pts):
		pIndex(index),
		pTimestamp(pts),
		pImpl(new FrameImpl())
	{}


	Frame::~Frame()
	{
		delete pImpl;
	}


	bool Frame::valid() const
	{
		return pImpl->frame and pImpl->frame->linesize[0] > 0
			and pImpl->frame->linesize[1] > 0 and pImpl->frame->linesize[2] > 0;
	}


	uint Frame::width() const
	{
		assert(pImpl->frame);
		return pImpl->frame->width;
	}

	uint Frame::height() const
	{
		assert(pImpl->frame);
		return pImpl->frame->height;
	}


	uint8* Frame::audioData()
	{
		assert(pImpl->frame);
		return pImpl->frame->extended_data[0];
	}

	uint Frame::audioSize() const
	{
		assert(pImpl->frame);
		return pImpl->frame->linesize[0];
	}


	uint8* Frame::dataY() const
	{
		assert(pImpl->frame);
		return pImpl->frame->data[0];
	}

	uint8* Frame::dataCb() const
	{
		assert(pImpl->frame);
		return pImpl->frame->data[1];
	}

	uint8* Frame::dataCr() const
	{
		assert(pImpl->frame);
		return pImpl->frame->data[2];
	}


	uint Frame::lineSizeY() const
	{
		assert(pImpl->frame);
		return pImpl->frame->linesize[0];
	}

	uint Frame::lineSizeCb() const
	{
		assert(pImpl->frame);
		return pImpl->frame->linesize[1];
	}

	uint Frame::lineSizeCr() const
	{
		assert(pImpl->frame);
		return pImpl->frame->linesize[0];
	}


	double Frame::timestamp() const
	{
		return pTimestamp;
	}


	void Frame::setData(void* data)
	{
		pImpl->frame = reinterpret_cast< ::AVFrame*>(data);
	}





} // namespace Media
} // namespace Private
} // namespace Yuni
