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
