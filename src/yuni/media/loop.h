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
#ifndef __YUNI_MEDIA_LOOP_H__
# define __YUNI_MEDIA_LOOP_H__

# include "../yuni.h"
# include "../core/event/loop.h"



namespace Yuni
{
namespace Media
{

	//! Forward Declaration
	class QueueService;


	/*!
	** \brief The media loop is where every media event is run
	*/
	class Loop: public Core::EventLoop::IEventLoop<Loop, Core::EventLoop::Flow::Timer,
		Core::EventLoop::Statistics::None, true>
	{
	public:
		//! The loop itself
		typedef Core::EventLoop::IEventLoop<Loop,
			Core::EventLoop::Flow::Timer,       // The flow policy
			Core::EventLoop::Statistics::None,  // The statistics policy
			true>                               // Use a separate thread
			LoopType;

		typedef LoopType::RequestType RequestType;

	public:
		/*!
		** \brief Constructor
		**
		** \param queueservice Media queue service corresponding to this loop
		*/
		Loop(QueueService* queueservice);

		//! Destructor (not virtual, this is static inheritance)
		~Loop() { stop(); }

		/*!
		** \brief Redefinition of the onLoop() callback
		*/
		bool onLoop();

		void beginClose();

		void endClose();

	private:
		//! Media queue service corresponding to this loop
		QueueService* pMediaService;
		//! When closing we must not execute onLoop()
		bool pClosing;

	}; // class Loop






} // namespace Media
} // namespace Yuni

#endif // __YUNI_MEDIA_LOOP_H__
