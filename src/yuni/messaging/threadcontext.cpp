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
#include "threadcontext.h"
#include "../net/http-status-code.h"


namespace Yuni
{
namespace Messaging
{

	Context::Context(Service& service) :
		userdata(nullptr),
		httpStatus(),
		remotePort(),
		service(service),
		httpStatusCode()
	{
		HttpStatusCode& codes = const_cast<HttpStatusCode&>(httpStatusCode);

		// 2xx
		for (uint i = 200; i <= codes.max2xx; ++i)
			codes.header2xx[i - 200] << "HTTP/1.1 " << i << ' ' << Net::HttpStatusCodeToCString(i) << "\r\n";

		// 4xx
		for (uint i = 400; i <= codes.max4xx; ++i)
		{
			codes.header4xx[i - 400] << "HTTP/1.1 " << i << ' '
				<< Net::HttpStatusCodeToCString(i) << "\r\n";

			codes.response4xx[i - 400] << codes.header4xx[i - 400]
				<< "\r\nCache: no-cache\r\n\r\n"
				<< Net::HttpStatusCodeToCString(i) << "\r\n";
		}

		// 5xx
		for (uint i = 500; i <= codes.max5xx; ++i)
		{
			codes.header5xx[i - 500] << "HTTP/1.1 " << i << ' '
				<< Net::HttpStatusCodeToCString(i) << "\r\n";

			codes.response5xx[i - 500] << codes.header5xx[i - 500]
				<< "\r\nCache: no-cache\r\n\r\n"
				<< Net::HttpStatusCodeToCString(i) << "\r\n";
		}
	}





} // namespace Messaging
} // namespace Yuni

