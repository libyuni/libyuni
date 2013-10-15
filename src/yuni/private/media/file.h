#ifndef __YUNI_PRIVATE_MEDIA_FILE_H__
# define __YUNI_PRIVATE_MEDIA_FILE_H__

# include "../../yuni.h"
# include "../../core/smartptr.h"
# include "../../core/string.h"
# include "stream.h"
# include <cassert>


namespace Yuni
{
namespace Private
{
namespace Media
{

	/*!
	** \brief A media file contains one or several video and/or audio streams
	*/
	class File final
	{
	public:
		//! Smart pointer
		typedef SmartPtr<File>  Ptr;

		# if LIBAVFORMAT_VERSION_MAJOR < 53
		typedef enum CodecType  MediaType;
		# else
		typedef enum AVMediaType  MediaType;
		# endif // LIBAVFORMAT_VERSION_MAJOR < 53

	public:
		//! Constructor
		File(const AnyString& filePath);
		//! Destructor
		~File();

		//! Get a video or audio stream, add it if necessary
		template<StreamType TypeT>
		typename Stream<TypeT>::Ptr getStream(uint index = 0);

		//! Get next packet for a given stream
		template<StreamType TypeT>
		AVPacket* getNextPacket(Stream<TypeT>* stream);

		//! Get file duration
		uint duration() const;

		//! Is the file valid for reading ?
		bool valid() const { return nullptr != pFormat; }

	private:
		//! Add a stream of the given media type at given index
		template<StreamType TypeT>
		typename Stream<TypeT>::Ptr addStream(uint index = 0);

		//! Get the proper cache depending on the media type (audio or video)
		template<StreamType TypeT>
		typename Stream<TypeT>::Map& getCache();

	private:
		//! File format and streams
		AVFormatContext* pFormat;

		//! Video stream cache
		mutable Stream<stVideo>::Map pVStreams;
		//! Audio stream cache
		mutable Stream<stAudio>::Map pAStreams;

	private:
		//! Friend declaration
		template<StreamType>
		friend class Stream;

	}; // class File





} // namespace Media
} // namespace Private
} // namespace Yuni

# include "stream.hxx"
# include "file.hxx"

#endif // __YUNI_PRIVATE_MEDIA_FILE_H__
