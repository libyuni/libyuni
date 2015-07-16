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
#ifndef __YUNI_NET_ERRORS_H__
# define __YUNI_NET_ERRORS_H__

# include "../yuni.h"
# include "../core/string.h"


namespace Yuni
{
namespace Net
{

	/*!
	** \brief Errors that can be returned by routines in the \p 'Yuni::Net' module
	*/
	enum Error
	{
		//! No error
		errNone = 0,
		//! Unknown error
		errUnknown,
		//! The port is invalid (range [1..65535])
		errInvalidPort,
		//! The address is invalid
		errInvalidHostAddress,
		//! The given address already exists in the list
		errDupplicatedAddress,
		//! No transport layer
		errNoTransport,
		//! Invalid transport layer
		errInvalidTransport,
		//! Impossible to start web server
		errStartFailed,
		//! The maximum number of errors
		errMax
	};



	/*!
	** \brief Convert an error to a human readable string
	**
	** For standard uses, you would prefer using Yuni::String
	*/
	const char* ErrorToCString(Error error);


} // namespace Net
} // namespace Yuni




namespace Yuni
{
namespace Extension
{
namespace CString
{

	template<class CStringT>
	class Append<CStringT, Yuni::Net::Error>
	{
	public:
		static inline void Perform(CStringT& string, Yuni::Net::Error error)
		{
			string += Yuni::Net::ErrorToCString(error);
		}
	};



} // namespace CString
} // namespace Extension
} // namespace Yuni


#endif // __YUNI_NET_ERRORS_H__
