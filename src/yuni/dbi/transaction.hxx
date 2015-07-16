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
#include "transaction.h"



namespace Yuni
{
namespace DBI
{


	inline Cursor Transaction::operator () (const AnyString& stmt)
	{
		return prepare(stmt);
	}


	template<class A1>
	inline DBI::Error Transaction::perform(const AnyString& script, const A1& a1)
	{
		Cursor stmt = prepare(script);
		stmt.map(a1);
		return stmt.perform();
	}

	template<class A1, class A2>
	inline DBI::Error Transaction::perform(const AnyString& script, const A1& a1, const A2& a2)
	{
		Cursor stmt = prepare(script);
		stmt.map(a1, a2);
		return stmt.perform();
	}


	template<class A1, class A2, class A3>
	inline DBI::Error Transaction::perform(const AnyString& script, const A1& a1, const A2& a2, const A3& a3)
	{
		Cursor stmt = prepare(script);
		stmt.map(a1, a2, a3);
		return stmt.perform();
	}


	template<class A1, class A2, class A3, class A4>
	inline DBI::Error Transaction::perform(const AnyString& script, const A1& a1, const A2& a2, const A3& a3, const A4& a4)
	{
		Cursor stmt = prepare(script);
		stmt.map(a1, a2, a3, a4);
		return stmt.perform();
	}


	template<class CallbackT>
	inline DBI::Error Transaction::each(const AnyString& query, const CallbackT& callback)
	{
		Cursor stmt = prepare(query);
		stmt.execute();
		return stmt.each(callback);
	}


	template<class CallbackT, class A1>
	inline
	DBI::Error Transaction::each(const AnyString& query, const A1& a1, const CallbackT& callback)
	{
		Cursor stmt = prepare(query);
		stmt.map(a1);
		stmt.execute();
		return stmt.each(callback);
	}


	template<class CallbackT, class A1, class A2>
	inline
	DBI::Error Transaction::each(const AnyString& query, const A1& a1, const A2& a2, const CallbackT& callback)
	{
		Cursor stmt = prepare(query);
		stmt.map(a1, a2);
		stmt.execute();
		return stmt.each(callback);
	}


	template<class CallbackT, class A1, class A2, class A3>
	inline
	DBI::Error Transaction::each(const AnyString& query, const A1& a1, const A2& a2, const A3& a3, const CallbackT& callback)
	{
		Cursor stmt = prepare(query);
		stmt.map(a1, a2, a3);
		stmt.execute();
		return stmt.each(callback);
	}


	template<class CallbackT, class A1, class A2, class A3, class A4>
	inline
	DBI::Error Transaction::each(const AnyString& query, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const CallbackT& callback)
	{
		Cursor stmt = prepare(query);
		stmt.map(a1, a2, a3, a4);
		stmt.execute();
		return stmt.each(callback);
	}





} // namespace DBI
} // namespace Yuni

