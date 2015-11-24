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
#include "../string.h"



namespace Yuni
{
namespace System
{
namespace Environment
{

	/*!
	** \brief Get a value from the current environment and convert it as a bool
	**
	** \param name Name of the variable
	** \return true if present and equal to "1", "on" or "true"
	*/
	YUNI_DECL bool ReadAsBool(const AnyString& name);

	/*!
	** \brief Get a value from the current environment and convert it as an integer
	**
	** \param name Name of the variable
	** \param defvalue The default value to take if the value of the variable is empty
	*/
	YUNI_DECL yint64 ReadAsInt64(const AnyString& name, yint64 defvalue = 0);

	/*!
	** \brief Get a value from the current environment
	**
	** \param name Name of the variable
	** \return The value of the variable, empty is an error has occured
	*/
	YUNI_DECL String Read(const AnyString& name);


	/*!
	** \brief Get a value from the current environment (Yuni::String version)
	**
	** \param name Name of the variable
	** \param[out] out Variable of type 'string'/'container' where the value will be appened
	** \param emptyBefore True to empty the parameter `out` before
	** \return True if the operation succeeded, false otherwise
	*/
	YUNI_DECL bool Read(const AnyString& name, String& out, bool emptyBefore = true);

	/*!
	** \brief Get a value from the current environment (Yuni::Clob version)
	**
	** \param name Name of the variable
	** \param[out] out Variable of type 'string'/'container' where the value will be appened
	** \param emptyBefore True to empty the parameter `out` before
	** \return True if the operation succeeded, false otherwise
	*/
	YUNI_DECL bool Read(const AnyString& name, Clob& out, bool emptyBefore = true);





} // namespace Environment
} // namespace System
} // namespace Yuni

