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
#ifndef __YUNI_CORE_EVENT_FLOW_CONTINUOUS_H__
# define __YUNI_CORE_EVENT_FLOW_CONTINUOUS_H__

# include "../../../yuni.h"
# include "../../../thread/thread.h"


namespace Yuni
{
namespace Core
{
namespace EventLoop
{
namespace Flow
{


	template<class EventLoopT>
	class YUNI_DECL Continuous
	{
	public:
		//! Type of the event loop
		typedef EventLoopT EventLoopType;

	public:
		//! \name Constructor
		//@{
		/*!
		** \brief Default constructor
		*/
		Continuous() {}
		//@}

	protected:
		//! \name Events triggered by the public interface of the event loop (from any thread)
		//@{
		/*!
		** \brief The event loop has just started
		**
		** The event loop is locked when this method is called
		*/
		static bool onStart() {return true;}

		/*!
		** \brief The event loop has just stopped
		**
		** The event loop is locked when this method is called
		*/
		static bool onStop() {return true;}

		/*!
		** \brief A new request has just been added into the queue
		**
		** The event loop is locked when this method is called
		** \param request The request (bind, see EventLoopType::RequestType)
		** \return True to allow the request to be posted
		*/
		template<class U> static bool onRequestPosted(const U& request)
		{ (void) request; return true; }
		//@}

		//! \name Events triggered from the main thread of the event loop
		//@{
		/*!
		** \brief The event loop has started a new cycle
		**
		** This method is called from the main thread of the event loop.
		** No lock is provided.
		*/
		static bool onNewCycle()
		{
			// Do not wait, directly execute the cycle
			return true;
		}
		//@}

		/*!
		** \brief Event triggered from the constructor of the event loop
		** \param e Pointer to the original event loop
		*/
		static void onInitialize(EventLoopType* e)
		{
			(void) e;
			// Do nothing
		}

	}; // class Continuous<>





} // namespace Flow
} // namespace EventLoop
} // namespace Core
} // namespace Yuni

#endif // __YUNI_CORE_EVENT_FLOW_CONTINUOUS_H__
