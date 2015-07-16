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
#ifndef __YUNI_MESSAGING_THREAD_CONTEXT_H__
# define __YUNI_MESSAGING_THREAD_CONTEXT_H__

# include "../yuni.h"
# include "fwd.h"
# include "../core/dictionary.h"


namespace Yuni
{
namespace Messaging
{

	/*!
	** \brief Schema for API methods
	**
	** A schema is some kink of namespace, where methods belong to
	*/
	class YUNI_DECL Context final
	{
	public:
		class HttpStatusCode final
		{
		public:
			enum
			{
				max2xx = 204,
				max4xx = 404,
				max5xx = 500,
			};
			ShortString32 header2xx[5];
			ShortString32 header4xx[5];
			ShortString32 header5xx[1];
			ShortString64 response4xx[5];
			ShortString64 response5xx[1];
		};

	public:
		/*!
		** \brief Default constructor
		**
		** \param service Reference to the original service instance
		*/
		Context(Service& service);

		//! Clear all internal variables
		void clear();

		//! Reduce memory usage by internal variables if necessary,
		// usefull for some memory-hungry apps
		void autoshrink();


	public:
		//! Parameters
		KeyValueStore params;
		//! User data pointer
		void* userdata;

		//! String preallocated for various usage (text with small size, < 6Ko)
		// to reduce memory usage, the internal memory of this variable will automatically
		// be wipe out if using more than 6KiB
		String text;
		//! Clob for various uses (text from medium to large size)
		// to reduce memory usage, the internal memory of this variable will automatically
		// be wipe out if using more than 5MiB
		Clob clob;

		//! Name of the method invoked
		AnyString method;
		//! Schema of the method invoked
		AnyString schema;

		//! Http status
		uint httpStatus;

		//! Remote port of the current connection
		uint remotePort;

		//! Buffer reused by transports for generating a response
		// However, it can be safely used by the method invoked
		Clob buffer;

		//! Service
		Service& service;

		//! Pre-prepared header responses
		const HttpStatusCode httpStatusCode;

	private:
		template<uint MaxSize, class StringT> void AutoShrink(StringT& variable);

	}; // class ThreadContext





} // namespace Messaging
} // namespace Yuni

# include "threadcontext.hxx"

#endif // __YUNI_MESSAGING_THREAD_CONTEXT_H__
