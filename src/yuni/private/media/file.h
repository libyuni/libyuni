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
		bool valid() const;

		//! Rewind the file
		void rewind();


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

		//! Friend declaration
		template<StreamType> friend class Stream;

	}; // class File





} // namespace Media
} // namespace Private
} // namespace Yuni

# include "stream.hxx"
# include "file.hxx"

#endif // __YUNI_PRIVATE_MEDIA_FILE_H__
