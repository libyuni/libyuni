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
#ifndef __YUNI_PRIVATE_MEDIA_FILE_H__
# error "Do not include stream.h directly, please include file.h instead !"
#endif

#ifndef __YUNI_PRIVATE_MEDIA_STREAM_H__
# define __YUNI_PRIVATE_MEDIA_STREAM_H__

# include "../../yuni.h"
# include "../../core/smartptr.h"
# include <map>

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

# include "streamtype.h"
# include "frame.h"


namespace Yuni
{
namespace Private
{
namespace Media
{

	//! Forward declaration
	class File;


	/*!
	** \brief A media stream can be either audio or video data encoded with a given codec
	*/
	template<StreamType TypeT>
	class Stream final
	{
	public:
		enum // anonymous
		{
			IsVideo = TypeT == stVideo,
			IsAudio = TypeT == stAudio
		};


	public:
		//! Smart pointer
		typedef SmartPtr<Stream>  Ptr;

		//! Map
		typedef std::map<uint, Ptr>  Map;

		//! Packet queue
		typedef std::list<AVPacket*>  PacketQueue;

	public:
		//! Constructor
		Stream(File* parent, AVFormatContext* format, AVCodecContext* codec, uint index);
		//! Destructor
		~Stream();

		//! Stream index in file
		uint index() const;

		//! Parent file
		File* parent() { return pParent; }

		//! Image width (Video only !)
		uint width() const;
		//! Image height (Video only !)
		uint height() const;
		//! Color depth, in bits per pixel (Video only !)
		uint depth() const;
		//! Number of frames per second (Video only !)
		float fps() const;

		//! Sample rate (Audio only !)
		uint rate() const;
		//! Number of channels (Audio only !)
		uint channels() const;
		//! Bits per sample (Audio only !)
		uint bits() const;

		//! Stream duration in seconds
		uint duration() const;

		//! Get the stream type
		StreamType type() const;

		//! OpenAL audio format (Audio only !)
		uint alFormat() const { YUNI_STATIC_ASSERT(IsAudio, NotAccessibleInVideo); return pALFormat; }

		/*!
		** \brief Get the next frame
		*/
		Frame::Ptr nextFrame();

		//! Rewind the stream
		void rewind();

		/*!
		** \brief Is the stream ready for decoding ?
		**
		** \note The OpenAL format check is done only for audio
		*/
		bool valid() const { return nullptr != pCodec && IsAudio == (0 != pALFormat); }

	private:
		//! Read a frame from the stream
		uint readFrame();

		//! Get the next packet, either from queue, or from the stream if the queue is empty
		AVPacket* nextPacket();

	private:
		//! Codec information
		AVCodecContext* pCodec;

		//! Format information
		AVFormatContext* pFormat;

		//! Index in the media file
		uint pIndex;

		//! Stream format
		uint pALFormat;

		//! Stream data full size
		uint64 pSize;

		//! Current presentation time stamp
		double pCrtPts;

		//! Current frame index
		uint pCrtFrameIndex;

		//! Currently read frame
		AVFrame* pFrame;

		//! Queue for this stream's packets
		PacketQueue pPackets;

		//! Parent file
		File* pParent;

	private:
		//! Friend declaration
		friend class File;

	}; // class Stream



} // namespace Media
} // namespace Private
} // namespace Yuni

#endif // __YUNI_PRIVATE_MEDIA_STREAM_H__
