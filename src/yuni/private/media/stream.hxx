#ifndef __YUNI_PRIVATE_MEDIA_STREAM_HXX__
# define __YUNI_PRIVATE_MEDIA_STREAM_HXX__

#include "openal.h"
#include "../../core/math.h"

namespace Yuni
{
namespace Private
{
namespace Media
{


	template<StreamType TypeT>
	Stream<TypeT>::Stream(File* parent, AVFormatContext* format, AVCodecContext* codecCtx, uint index):
		pCodec(codecCtx),
		pFormat(format),
		pIndex(index),
		pALFormat(0),
		pSize(0),
		pCrtPts(AV_NOPTS_VALUE),
		pFrame(nullptr),
		pParent(parent)
	{
		if (!pParent)
		{
			pCodec = nullptr;
			return;
		}

		// Try to find the codec for the given codec ID, and open it
		AVCodec* codec = ::avcodec_find_decoder(pCodec->codec_id);
		# if LIBAVFORMAT_VERSION_MAJOR < 53
		if (!codec or ::avcodec_open(pCodec, codec) < 0)
		# else
		if (!codec or ::avcodec_open2(pCodec, codec, NULL) < 0)
		# endif // LIBAVFORMAT_VERSION_MAJOR < 53
		{
			pCodec = nullptr;
			return;
		}

		if (IsAudio)
			pALFormat = Private::Media::OpenAL::GetFormat(16, pCodec->channels);
	}


	template<StreamType TypeT>
	Stream<TypeT>::~Stream()
	{
		if (pCodec)
		{
			::avcodec_close(pCodec);
			pCodec = nullptr;
		}
		if (pFrame)
			::av_free(pFrame);
	}


	template<StreamType TypeT>
	AVPacket* Stream<TypeT>::nextPacket()
	{
		// If the queue is empty
		if (pPackets.empty())
		{
			if (!pParent)
				return nullptr;

			AVPacket* pkt = pParent->getNextPacket(this);
			if (!pkt)
				// No more packets
				return nullptr;
		}

		// Get the first packet in queue
		AVPacket* pkt = pPackets.front();
		pPackets.pop_front();
		return pkt;
	}


	template<StreamType TypeT>
	uint Stream<TypeT>::readFrame()
	{
		// Frame allocation
		if (!pFrame)
		{
			if (!(pFrame = ::avcodec_alloc_frame()))
			{
				std::cerr << "Error allocating a frame for audio decoding !" << std::endl;
				return 0;
			}
		}
		else
			// Should not happen, but this is a security.
			::avcodec_get_frame_defaults(pFrame);


		int bytesRead = 0;
		int frameFinished = 0;
		AVPacket* packet;
		while (!frameFinished)
		{
			// Get next packet
			packet = nextPacket();
			if (!packet)
				return 0;

			// VIDEO
			if (IsVideo)
			{
				pCrtPts = 0;
				// Decode the packet
				#if LIBAVCODEC_VERSION_INT > AV_VERSION_INT(52,30,0)
				if ((bytesRead = ::avcodec_decode_video2(pCodec, pFrame, &frameFinished, packet)) < 0)
				#else
				if ((bytesRead = ::avcodec_decode_video(pCodec, pFrame, &frameFinished, packet->data, packet->size)) < 0)
				#endif
				{
					std::cerr << "Error while decoding video !" << std::endl;
					// Do not do anything here, just act normally and try to recover from the error
				}

				// If the frame is finished (should be in one shot)
				if (frameFinished)
				{
					if (AV_NOPTS_VALUE == (uint64)packet->dts and pFrame->opaque
						&& AV_NOPTS_VALUE != *(uint64*)pFrame->opaque)
						pCrtPts = *(uint64*)pFrame->opaque;
					else if (AV_NOPTS_VALUE != (uint64)packet->dts)
						pCrtPts = packet->dts;
					else
						pCrtPts = 0.0;
					pCrtPts *= ::av_q2d(pCodec->time_base);
					break;
				}
			}
			// AUDIO
			else
			{
				// Decode the packet
				#if LIBAVCODEC_VERSION_INT > AV_VERSION_INT(52,30,0)
				if ((bytesRead = ::avcodec_decode_audio4(pCodec, pFrame, &frameFinished, packet)) < 0)
				#else
				if ((bytesRead = ::avcodec_decode_audio3(pCodec, pFrame, &frameFinished, packet->data, packet->size)) < 0)
				#endif
				{
					std::cerr << "Error while decoding audio !" << std::endl;
					// Do not do anything here, just act normally and try to recover from the error
				}

				// If the frame is finished (should be in one shot)
				if (frameFinished)
				{
					break;
				}
			}

			// Free packet before looping
			::av_free_packet(packet);
			delete packet;
		}

		// Free packet before quitting
		::av_free_packet(packet);
		delete packet;
		return bytesRead;
	}


	template<StreamType TypeT>
	inline Frame::Ptr Stream<TypeT>::nextFrame()
	{
		//YUNI_STATIC_ASSERT(IsVideo, nextFrameNotAccessibleInAudio);
		if (IsVideo)
			readFrame();
		else // IsAudio
			//readAudioFrame();
			readFrame();
		// TODO : Give the real frame index
		Frame::Ptr frame = new Frame(0u, pCrtPts);
		frame->setData(pFrame);
		pFrame = nullptr;
		return frame;
	}


	template<StreamType TypeT>
	inline uint Stream<TypeT>::index() const
	{
		return pIndex;
	}


	template<StreamType TypeT>
	inline uint Stream<TypeT>::duration() const
	{
		assert(pParent);
		return pParent->duration();
	}


	template<StreamType TypeT>
	inline uint Stream<TypeT>::width() const
	{
		YUNI_STATIC_ASSERT(IsVideo, NotAccessibleInAudio);
		return pCodec->width;
	}


	template<StreamType TypeT>
	inline uint Stream<TypeT>::height() const
	{
		YUNI_STATIC_ASSERT(IsVideo, NotAccessibleInAudio);
		return pCodec->height;
	}


	template<StreamType TypeT>
	inline uint Stream<TypeT>::depth() const
	{
		YUNI_STATIC_ASSERT(IsVideo, NotAccessibleInAudio);
		return ::av_get_bits_per_pixel(&::av_pix_fmt_descriptors[pCodec->pix_fmt]);
	}


	template<StreamType TypeT>
	inline float Stream<TypeT>::fps() const
	{
		YUNI_STATIC_ASSERT(IsVideo, NotAccessibleInAudio);
		assert(pCodec);
		float variable = (float)pFormat->streams[pIndex]->avg_frame_rate.num / pFormat->streams[pIndex]->avg_frame_rate.den;
		float constant = (float)pCodec->time_base.den / (pCodec->time_base.num * pCodec->ticks_per_frame);
		return Math::Min(variable, constant);
	}


	template<StreamType TypeT>
	inline uint Stream<TypeT>::rate() const
	{
		YUNI_STATIC_ASSERT(IsAudio, NotAccessibleInVideo);
		assert(pCodec);
		return pCodec->sample_rate / pCodec->channels;
	}


	template<StreamType TypeT>
	inline uint Stream<TypeT>::channels() const
	{
		YUNI_STATIC_ASSERT(IsAudio, NotAccessibleInVideo);
		assert(pCodec);
		return pCodec->channels;
	}


	template<StreamType TypeT>
	inline uint Stream<TypeT>::bits() const
	{
		YUNI_STATIC_ASSERT(IsAudio, NotAccessibleInVideo);
		assert(pCodec);
		// Internal FFMpeg format is always 16 bits
		return 16;
	}


	template<StreamType TypeT>
	inline StreamType Stream<TypeT>::type() const
	{
		return TypeT;
	}



} // namespace Media
} // namespace Private
} // namespace Yuni

#endif // __YUNI_PRIVATE_MEDIA_STREAM_HXX__
