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
#ifndef __YUNI_PRIVATE_MEDIA_FRAME_H__
# define __YUNI_PRIVATE_MEDIA_FRAME_H__

# include "../../yuni.h"
# include "../../core/smartptr.h"
# include "../../core/bind.h"
# include "streamtype.h"
# include <queue>


namespace Yuni
{
namespace Private
{
namespace Media
{


	//! Forward declaration
	class FrameImpl;


	/*!
	** \brief A frame contains all the data corresponding to a decoded YCbCr 420 frame
	*/
	class Frame final
	{
	public:
		//! Smart pointer
		typedef SmartPtr<Frame>  Ptr;

		//! Frame queue
		typedef std::list<Ptr>  Queue;

	public:
		// This event is triggered when it is time for a new frame to be displayed
		static Yuni::Bind<void (const Frame::Ptr& frame)>  OnFrameChanged;

	public:
		//! Constructor
		Frame(uint index, double pts);
		//! Destructor
		~Frame();

		//! Is the frame valid ?
		bool valid() const;

		//! Does the frame contain video data ?
		bool isVideo() const;

		//! Does the frame contain audio data ?
		bool isAudio() const;

		//! Image width (Video only !)
		uint width() const;
		//! Image height (Video only !)
		uint height() const;

		//! Frame index in the stream
		uint index() const { return pIndex; }

		//! Audio data (Audio only !)
		uint8* audioData();

		//! Audio data size (Audio only !)
		uint audioSize() const;

		//! Y data (Video only !)
		uint8* dataY() const;
		//! Cb data (Video only !)
		uint8* dataCb() const;
		//! Cr data (Video only !)
		uint8* dataCr() const;

		//! Line size for Y data
		uint lineSizeY() const;
		//! Line size for Cb data (normally: lineSizeY / 2)
		uint lineSizeCb() const;
		//! Line size for Cr data (normally: lineSizeY / 2)
		uint lineSizeCr() const;

		//! Presentation time stamp for this frame
		double timestamp() const;

		uint frameNumber() const;

	private:
		//! Read a frame from the stream
		void readFrame();

		//! Ugly hidden way of setting the frame in our FrameImpl
		void setData(void* data);

	private:
		//! Frame number
		uint pIndex;

		//! Presentation time stamp
		double pTimestamp;

		//! Internal data (wrapper for the AVFrame)
		FrameImpl* pImpl;

		//! Friend declaration
		template<StreamType TypeT> friend class Stream;

	}; // class Frame






} // namespace Media
} // namespace Private
} // namespace Yuni

#endif // __YUNI_PRIVATE_MEDIA_STREAM_H__
