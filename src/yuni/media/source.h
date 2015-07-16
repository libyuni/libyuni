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
#ifndef __YUNI_MEDIA_SOURCE_H__
# define __YUNI_MEDIA_SOURCE_H__

# include <map>
# include "../yuni.h"
# include "../core/string.h"
# include "../core/smartptr.h"
# include "../core/string.h"
# include "../thread/signal.h"
# include "../private/media/file.h"

namespace Yuni
{
namespace Media
{


	/*!
	** \brief A media source loaded from a file or network stream
	*/
	class Source final: public Policy::ObjectLevelLockable<Source>
	{
	public:
		//! Pointer type
		typedef SmartPtr<Source> Ptr;
		//! Map from a string to a Source::Ptr
		typedef std::map<String, Ptr> Map;
		//! Threading Policy
		typedef Policy::ObjectLevelLockable<Source>  ThreadingPolicy;
		//! Audio stream pointer type
		typedef Private::Media::Stream<Private::Media::stAudio>::Ptr  AStreamPtr;
		//! Video stream pointer type
		typedef Private::Media::Stream<Private::Media::stVideo>::Ptr  VStreamPtr;

	public:
		enum // anonymous
		{
			maxBufferCount = 4,
			minBufferSize = 61000,
			maxBufferSize = 65536,

			bestQueueSize = 150

		}; // enum anonymous

	public:
		//! Constructor
		Source();
		~Source();

		bool prepareDispatched(uint source);

		bool destroyDispatched(Thread::Signal* signal);

		bool updateDispatched(uint source);

		bool rewindDispatched(uint source);

		void stream(const AStreamPtr& audioStream) { pAStream = audioStream; }

		void stream(const VStreamPtr& videoStream) { pVStream = videoStream; }

		void stream(const VStreamPtr& videoStream, const AStreamPtr& audioStream)
		{
			pAStream = audioStream;
			pVStream = videoStream;
		}

		//! Has either valid audio, valid video, or both.
		bool valid() const { return hasAudio() or hasVideo(); }

		//! Has a valid audio stream
		bool hasAudio() const { return nullptr != pAStream and pAStream->valid(); }

		//! Has a valid video stream
		bool hasVideo() const { return nullptr != pVStream and pVStream->valid(); }

		//! Get the duration of the stream, 0 if not set
		uint duration() const;

		//! Get the current elapsed time of the source
		float elapsedTime() const
		{
			if (!pAStream)
				return 0.0f;
			return pSecondsElapsed + pSecondsCurrent;
		}

		//! Width of the video (in pixels). Only valid if there is video !
		uint width() const { assert(hasVideo()); return pVStream->width(); }

		//! Height of the video (in pixels). Only valid if there is video !
		uint height() const { assert(hasVideo()); return pVStream->height(); }

		//! Depth of the video (in bits / pixel). Only valid if there is video !
		uint depth() const { assert(hasVideo()); return pVStream->depth(); }

		//! Frames per second of the video. Only valid if there is video !
		float fps() const { assert(hasVideo()); return pVStream->fps(); }

		//! Audio sampling rate. Only valid if there is audio !
		uint samplingRate() const { assert(hasAudio()); return pAStream->rate(); }

		//! Number of audio channels. Only valid if there is audio !
		uint channels() const { assert(hasAudio()); return pAStream->channels(); }

		//! Bits per sample for audio. Only valid if there is audio !
		uint bitsPerSample() const { assert(hasAudio()); return pAStream->bits(); }

		//! Get the next video frame. Only valid if there is video !
		Private::Media::Frame::Ptr nextFrame();

	private:
		//! Refill audio buffer
		uint fillBuffer();

		//! Refill the video frame queue
		void fillQueue();

	private:
		//! The audio stream
		AStreamPtr pAStream;

		//! The video stream
		VStreamPtr pVStream;

		//! Actual number of buffers
		uint pBufferCount;

		//! Seconds already played from previous buffers
		float pSecondsElapsed;

		//! Seconds already played from current buffer
		float pSecondsCurrent;

		//! Identifiers of the OpenAL buffers used
		uint pIDs[maxBufferCount];

		//! Buffer for storing decoded audio data
		CString<maxBufferSize, false> pData;

		//! Currently loaded video frames
		Private::Media::Frame::Queue pFrames;

	}; // class Source






} // namespace Media
} // namespace Yuni

# include "source.hxx"

#endif // __YUNI_MEDIA_SOURCE_H__
