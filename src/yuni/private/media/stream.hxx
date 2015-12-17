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
#ifndef __YUNI_PRIVATE_MEDIA_STREAM_HXX__
# define __YUNI_PRIVATE_MEDIA_STREAM_HXX__

#include "openal.h"
#include "../../core/math.h"
#include <iostream>

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
		pCrtPts((double)AV_NOPTS_VALUE),
		pCrtFrameIndex(0),
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
		{
			uint bitsPerSample = ::av_get_bytes_per_sample(pCodec->sample_fmt) * 8;
			pALFormat = Private::Media::OpenAL::GetFormat(bitsPerSample, pCodec->channels);
		}
	}


	template<StreamType TypeT>
	Stream<TypeT>::~Stream()
	{
		if (pCodec and pCodec->codec)
		{
			//::avcodec_close(pCodec);
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
			#if LIBAVUTIL_VERSION_INT > AV_VERSION_INT(52, 20, 100)
			if (!(pFrame = ::av_frame_alloc()))
			#else
			if (!(pFrame = ::avcodec_alloc_frame()))
			#endif
			{
				std::cerr << "Error allocating a frame for audio decoding !" << std::endl;
				return 0;
			}
		}
		else
		{
			// Should not happen, but this is a security.
			#if LIBAVUTIL_VERSION_INT > AV_VERSION_INT(52, 20, 100)
			::av_frame_unref(pFrame);
			#else
			::avcodec_get_frame_defaults(pFrame);
			#endif
		}

		int bytesRead = 0;
		int frameFinished = 0;
		AVPacket* packet = nullptr;

		while (not frameFinished)
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
					continue;
					// Do not do anything here, just act normally and try to recover from the error
				}

				// If the frame is finished (should be in one shot)
				if (frameFinished)
				{
					pCrtPts = ::av_frame_get_best_effort_timestamp(pFrame) / fps();
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
					continue;
					// Do not do anything here, just act normally and try to recover from the error
				}

				// If the frame is finished (should be in one shot)
				if (frameFinished)
					break;
			}

			// Free packet before looping
			::av_free_packet(packet);
			delete packet;
			packet = nullptr;
		}

		++pCrtFrameIndex;

		// Free packet before quitting
		if (packet)
		{
			::av_free_packet(packet);
			delete packet;
		}

		return bytesRead;
	}


	template<StreamType TypeT>
	inline Frame::Ptr Stream<TypeT>::nextFrame()
	{
		if (!readFrame())
			return nullptr;

		Frame* frame = new Frame(pCrtFrameIndex, pCrtPts);
		// Our Frame object takes custody of the AVFrame
		// and will take care of its deletion
		frame->setData(pFrame);
		// Reset the current frame
		pFrame = nullptr;
		return frame;
	}


	template<StreamType TypeT>
	inline void Stream<TypeT>::rewind()
	{
		if (pFrame)
		{
			::av_free(pFrame);
			pFrame = nullptr;
		}
		::av_seek_frame(pFormat, pIndex, 0, 0);
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
		#if LIBAVCODEC_VERSION_INT > AV_VERSION_INT(52,30,0)
		return ::av_get_bits_per_pixel(::av_pix_fmt_desc_get(pCodec->pix_fmt));
		#else
		return ::av_get_bits_per_pixel(&::av_pix_fmt_descriptors[pCodec->pix_fmt]);
		#endif
	}


	template<StreamType TypeT>
	inline float Stream<TypeT>::fps() const
	{
		if (!IsVideo)
			return 0.0f;

		assert(pCodec);
		assert(pFormat);
		assert(pFormat->streams[pIndex]);

		auto rational = ::av_guess_frame_rate(pFormat, pFormat->streams[pIndex], pFrame);
		if (rational.den > 0.0f && rational.num > 0.0f)
			return (float)rational.num / rational.den;

		// Fallback methods
		auto* avStream = pFormat->streams[pIndex];

		float den = (float)avStream->avg_frame_rate.den;
		float variable = 0.0f;
		if (den > 0.0f) // avoid divide by 0
			variable = (float)::av_q2d(avStream->avg_frame_rate);

		den = (float)avStream->time_base.num;
		float constant = 0.0f;
		if (den > 0.0f) // avoid divide by 0
			constant = (float)(pCodec->ticks_per_frame * avStream->time_base.den) / den;

		return Math::Max(variable, constant);
	}


	template<StreamType TypeT>
	inline uint Stream<TypeT>::rate() const
	{
		YUNI_STATIC_ASSERT(IsAudio, NotAccessibleInVideo);
		assert(pCodec);
		return pCodec->sample_rate;
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
		return 16u;
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
