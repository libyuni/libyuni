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
#pragma once
#include "../../yuni.h"



namespace Yuni
{
namespace System
{

	/*!
	** \brief Retrieves the calling user's name
	**
	** On Unixes, the value will be read from the environment variables
	** `LOGNAME`. On Windows, the method GetUserName will be used.
	**
	** Windows (from MSDN):
	** If the current thread is impersonating another client, the GetUserName
	** function returns the user name of the client that the thread is impersonating.
	** \see http://msdn.microsoft.com/en-us/library/ms724432%28v=vs.85%29.aspx
	**
	** \param out Variable of type 'string' where the value will be appened
	** \param emptyBefore True to empty the parameter `out` before
	** \return True if the operation succeeded (a valid username has been found), false otherwise
	*/
	template<class StringT> YUNI_DECL bool Username(StringT& out, bool emptyBefore = true);



} // namespace System
} // namespace Yuni

# include "username.hxx"

