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
#include <algorithm>
#include "any.h"


namespace Yuni
{


	Any::Any()
	{
		pTable = Private::Any::Table<Private::Any::Empty>::Get();
		pObject = NULL;
	}


	Any::Any(const Any& rhs)
	{
		pTable = Private::Any::Table<Private::Any::Empty>::Get();
		assign(rhs);
	}


	Any::~Any()
	{
		pTable->staticDelete(&pObject);
	}


	Any& Any::assign(const Any& rhs)
	{
		// Are we copying from the same type (using the same table) ?
		if (pTable == rhs.pTable)
		{
			// If so, we can avoid reallocation
			pTable->move(&rhs.pObject, &pObject);
		}
		else
		{
			reset();
			rhs.pTable->clone(&rhs.pObject, &pObject);
			pTable = rhs.pTable;
		}
		return *this;
	}


	void Any::Swap(Any& one, Any& other)
	{
		std::swap(one.pTable, other.pTable);
		std::swap(one.pObject, other.pObject);
	}


	void Any::reset()
	{
		if (!this->empty())
		{
			pTable->staticDelete(&pObject);
			pTable = Private::Any::Table<Private::Any::Empty>::Get();
			pObject = NULL;
		}
	}



} // namespace Yuni
