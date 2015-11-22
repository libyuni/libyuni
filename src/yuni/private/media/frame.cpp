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
		assert(pImpl && pImpl->frame);
		return pImpl->frame and pImpl->frame->linesize[0] > 0
			and pImpl->frame->linesize[1] > 0 and pImpl->frame->linesize[2] > 0;
	}

	bool Frame::isVideo() const
	{
		assert(pImpl && pImpl->frame);
		return pImpl->frame->width > 0;
	}

	bool Frame::isAudio() const
	{
		assert(pImpl && pImpl->frame);
		return pImpl->frame->nb_samples > 0;
	}


	uint Frame::width() const
	{
		assert(pImpl->frame);
		assert(isVideo());
		return pImpl->frame->width;
	}

	uint Frame::height() const
	{
		assert(pImpl->frame);
		assert(isVideo());
		return pImpl->frame->height;
	}


	uint8* Frame::audioData()
	{
		assert(pImpl->frame);
		assert(isAudio());
		return pImpl->frame->extended_data[0];
	}

	uint Frame::audioSize() const
	{
		assert(pImpl->frame);
		assert(isAudio());
		return pImpl->frame->linesize[0];
	}


	uint8* Frame::dataY() const
	{
		assert(pImpl->frame);
		assert(isVideo());
		return pImpl->frame->data[0];
	}

	uint8* Frame::dataCb() const
	{
		assert(pImpl->frame);
		assert(isVideo());
		return pImpl->frame->data[1];
	}

	uint8* Frame::dataCr() const
	{
		assert(pImpl->frame);
		assert(isVideo());
		return pImpl->frame->data[2];
	}


	uint Frame::lineSizeY() const
	{
		assert(pImpl->frame);
		assert(isVideo());
		return pImpl->frame->linesize[0];
	}

	uint Frame::lineSizeCb() const
	{
		assert(pImpl->frame);
		assert(isVideo());
		return pImpl->frame->linesize[1];
	}

	uint Frame::lineSizeCr() const
	{
		assert(pImpl->frame);
		assert(isVideo());
		return pImpl->frame->linesize[0];
	}


	double Frame::timestamp() const
	{
		return pTimestamp;
	}

	uint Frame::frameNumber() const
	{
		assert(isVideo());
		return ::av_frame_get_best_effort_timestamp(pImpl->frame);
	}


	void Frame::setData(void* data)
	{
		// This is not the common usage, but if we end up replacing the data
		// do not forget to release the previous
		if (pImpl->frame)
			::av_free(pImpl->frame);
		pImpl->frame = reinterpret_cast< ::AVFrame*>(data);
	}





} // namespace Media
} // namespace Private
} // namespace Yuni
