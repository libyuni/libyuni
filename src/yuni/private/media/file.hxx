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
#ifndef __YUNI_PRIVATE_MEDIA_FILE_HXX__
# define __YUNI_PRIVATE_MEDIA_FILE_HXX__

namespace Yuni
{
namespace Private
{
namespace Media
{

	namespace // anonymous
	{

		//! Check that the given media type is the same as the static one
		template<StreamType TypeT>
		struct IsSameType;

		template<>
		struct IsSameType<stVideo> final
		{
			static inline bool Check(File::MediaType type)
			{
				# if LIBAVFORMAT_VERSION_MAJOR < 53
				return CODEC_TYPE_VIDEO == type;
				# else
				return AVMEDIA_TYPE_VIDEO == type;
				# endif // LIBAVFORMAT_VERSION_MAJOR < 53
			}
		};

		template<>
		struct IsSameType<stAudio> final
		{
			static inline bool Check(File::MediaType type)
			{
				# if LIBAVFORMAT_VERSION_MAJOR < 53
				return CODEC_TYPE_AUDIO == type;
				# else
				return AVMEDIA_TYPE_AUDIO == type;
				# endif // LIBAVFORMAT_VERSION_MAJOR < 53
			}
		};


	} // anonymous namespace






	template<StreamType TypeT>
	inline typename Stream<TypeT>::Ptr File::addStream(uint index)
	{
		assert(pFormat and "invalid format pointer");

		typedef Stream<TypeT> MyStream;

		// Loop on streams in the file
		for (uint i = 0; i != pFormat->nb_streams; ++i)
		{
			assert(pFormat->streams[i] and "invalid stream pointer");
			assert(pFormat->streams[i]->codec and "invalid stream pointer");

			// Reject streams that are not the requested codec type
			if (not IsSameType<TypeT>::Check(pFormat->streams[i]->codec->codec_type))
				continue;

			// Continue until we find the requested stream
			if (index > 0)
			{
				--index;
				continue;
			}

			// Allocate a new stream object and fill in its info
			typename MyStream::Ptr stream = new MyStream(this, pFormat, pFormat->streams[i]->codec, i);

			// Check that the codec was properly loaded
			if (not stream->valid())
			{
				// The new stream is destroyed here by the SmartPtr
				continue;
			}

			// Append the new stream object to the stream list.
			getCache<TypeT>()[stream->index()] = stream;
			return stream;
		}

		return nullptr;
	}


	template<StreamType TypeT>
	inline typename Stream<TypeT>::Ptr File::getStream(uint index)
	{
		assert(pFormat);
		typename Stream<TypeT>::Map& cache = getCache<TypeT>();
		typename Stream<TypeT>::Map::iterator it = cache.find(index);
		if (cache.end() != it)
			return it->second;

		return addStream<TypeT>(index);
	}


	template<StreamType TypeT>
	inline AVPacket* File::getNextPacket(Stream<TypeT>* stream)
	{
		AVPacket* packet = new AVPacket();

		// Get a packet
		while (::av_read_frame(pFormat, packet) >= 0)
		{
			// Search for the corresponding stream in the video stream cache
			auto vIt = pVStreams.find((uint)packet->stream_index);
			if (pVStreams.end() != vIt)
			{
				// This is a stream we handle, so enqueue the packet
				vIt->second->pPackets.push_back(packet);

				// Return if this stream is what we needed a packet for
				if (TypeT == stVideo and vIt->second->index() == stream->index())
					return packet;

				// Packet was stored, get another one
				packet = new AVPacket();
				continue;
			}
			else
			{
				// Search for the corresponding stream in the audio stream cache
				auto aIt = pAStreams.find((uint)packet->stream_index);
				if (pAStreams.end() != aIt)
				{
					// This is a stream we handle, so enqueue the packet
					aIt->second->pPackets.push_back(packet);

					// Return if this stream is what we needed a packet for
					if (TypeT == stAudio and aIt->second->index() == stream->index())
						return packet;

					// Packet was stored, get another one
					packet = new AVPacket();
					continue;
				}
			}

			// Unmanaged packet, free it and look for another one
			::av_free_packet(packet);
		}

		delete packet;
		return nullptr;
	}


	template<>
	inline Stream<stVideo>::Map& File::getCache<stVideo>()
	{
		return pVStreams;
	}


	template<>
	inline Stream<stAudio>::Map& File::getCache<stAudio>()
	{
		return pAStreams;
	}


	template<StreamType TypeT>
	inline typename Stream<TypeT>::Map& File::getCache()
	{
		assert(false and "Invalid stream type");
		return pVStreams;
	}


	inline bool File::valid() const
	{
		return nullptr != pFormat;
	}





} // namespace Media
} // namespace Private
} // namespace Yuni

#endif // __YUNI_PRIVATE_MEDIA_FILE_HXX__
