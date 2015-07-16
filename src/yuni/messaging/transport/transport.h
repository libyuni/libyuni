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
#ifndef __YUNI_MESSAGING_TRANSPORT_TRANSPORT_H__
# define __YUNI_MESSAGING_TRANSPORT_TRANSPORT_H__

# include "../../yuni.h"
# include "../../core/string.h"
# include "../../core/noncopyable.h"
# include "../../thread/thread.h"
# include "../fwd.h"
# include "../../net/errors.h"
# include "../../net/hostaddressport.h"
# include "../../net/port.h"
# include "../../core/dictionary.h"
# include "../protocol.h"


namespace Yuni
{
namespace Messaging
{
namespace Transport
{

	enum Mode
	{
		tmNone = 0,
		//! Server mode
		tmServer,
		//! Client mode
		tmClient
	};


	/*!
	** \brief Transport layer for messages (abstract)
	**
	** A transport layer is not thread-safe.
	*/
	class ITransport : private NonCopyable<ITransport>
	{
	public:
		//! The most suitable smart pointer for the class
		typedef SmartPtr<ITransport>  Ptr;

		//! Hash
		typedef Dictionary<Net::HostAddressPort, Ptr>::Hash Hash;
		//! Map
		typedef Dictionary<Net::HostAddressPort, Ptr>::Ordered Map; // TODO: remove this typdef

	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default constructor
		*/
		explicit ITransport(Mode m);
		//! Destructor
		virtual ~ITransport();
		//@}

		//! \name Attached thread
		//@{
		//! Get the attached thread
		Thread::IThread*  attachedThread();
		//! Get the attached thread (const)
		const Thread::IThread*  attachedThread() const;
		//! Set the attached thread
		void attachedThread(Thread::IThread* thread);
		//@}


		//! Service
		//@{
		//! Start the service
		virtual Yuni::Net::Error  start() = 0;
		//! Run the transport layer (must block until finished)
		virtual Yuni::Net::Error  run() = 0;
		//! Ask to stop the transport layer (if not already done)
		virtual void stop() = 0;
		//@}

		//! \name Protocol
		//@{
		/*!
		** \brief Install a new protocol
		*/
		virtual void protocol(const Protocol& protocol) = 0;
		//@}


	public:
		//! Address to listen
		Net::HostAddress  address;
		//! Port
		Net::Port port;
		//! Mode (server/client)
		const Mode mode;

	protected:
		//! The attached thread, if any
		Thread::IThread* pAttachedThread;
		//! Service (initialized by the service owner)
		Yuni::Messaging::Service* pService;
		// Our friends
		friend class Yuni::Messaging::Service;

	}; // class Transport






} // namespace Transport
} // namespace Server
} // namespace Yuni

# include "transport.hxx"

#endif // __YUNI_MESSAGING_TRANSPORT_TRANSPORT_H__
