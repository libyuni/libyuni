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
#ifndef __YUNI_DBI_CURSOR_H__
# define __YUNI_DBI_CURSOR_H__

# include "../yuni.h"
# include "../core/string.h"
# include "../core/noncopyable.h"
# include "adapter/entries.h"
# include "row.h"


namespace Yuni
{
namespace DBI
{

	/*!
	** \ingroup DBI
	*/
	class Cursor final : private Yuni::NonCopyable<Cursor>
	{
	public:
		//! \name Constructors & Destructor
		//@{
		/*!
		** \brief Default constructor
		*/
		Cursor(::yn_dbi_adapter& adapter, void* handle);
		/*!
		** \brief Move constructor
		*/
		Cursor(Cursor&& other);
		//! Destructor
		~Cursor();
		//@}


		//! \name Bindings
		//@{
		//! Bind a sepcific parameter as a string
		Cursor& bind(uint index, const AnyString& value);
		//! Bind a sepcific parameter as a bool
		Cursor& bind(uint index, bool value);
		//! Bind a sepcific parameter as a sint32
		Cursor& bind(uint index, sint32 value);
		//! Bind a sepcific parameter as a sint64
		Cursor& bind(uint index, sint64 value);
		//! Bind a sepcific parameter as a double
		Cursor& bind(uint index, double value);
		//! Bind a sepcific parameter as a null
		Cursor& bind(uint index, const NullPtr&);

		//! Convenient method for binding the first 1 parameter
		template<class A1>
		Cursor& map(const A1& a1);
		//! Convenient method for binding the first 2 parameters at once
		template<class A1, class A2>
		Cursor& map(const A1& a1, const A2& a2);
		//! Convenient method for binding the first 3 parameters at once
		template<class A1, class A2, class A3>
		Cursor& map(const A1& a1, const A2& a2, const A3& a3);
		//! Convenient method for binding the first 4 parameters at once
		template<class A1, class A2, class A3, class A4>
		Cursor& map(const A1& a1, const A2& a2, const A3& a3, const A4& a4);
		//@}


		//! \name Execution
		//@{
		/*!
		** \brief Execute the query but discard the results
		*/
		DBI::Error perform();

		/*!
		** \brief Execute the query
		*/
		DBI::Error execute();
		//@}


		//! \name Resultset
		//@{
		/*!
		** \brief Fetch the current row
		*/
		Row fetch();

		/*!
		** \brief Iterate over all rows in the resultset
		*/
		template<class CallbackT> DBI::Error each(const CallbackT& callback);

		/*!
		** \brief Go to the next row
		*/
		DBI::Error next();

		/*!
		** \brief Go to the previous row
		*/
		DBI::Error previous();

		/*!
		** \brief Move the cursor to a specific row index in the result set
		*/
		DBI::Error moveTo(uint64 rowindex);
		//@}


	private:
		//! Alias to the current channel
		// \internal This reference can be null and must never be called if pHandle is null
		::yn_dbi_adapter& pAdapter;
		//! Opaque pointer to the current query
		void* pHandle;

	}; // class Cursor





} // namespace DBI
} // namespace Yuni

# include "cursor.hxx"

#endif // __YUNI_DBI_CURSOR_H__
