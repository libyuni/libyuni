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
#ifndef __YUNI_DBI_CURSOR_HXX__
# define __YUNI_DBI_CURSOR_HXX__


namespace Yuni
{
namespace DBI
{

	inline Cursor::Cursor(::yn_dbi_adapter& adapter, void* handle) :
		pAdapter(adapter),
		pHandle(handle)
	{}


	inline Cursor::Cursor(Cursor&& other) :
		pAdapter(other.pAdapter),
		pHandle(other.pHandle)
	{
		other.pHandle = nullptr;
	}


	inline Cursor::~Cursor()
	{
		if (pHandle)
			pAdapter.query_ref_release(pHandle);
	}


	inline Cursor& Cursor::bind(uint index, const AnyString& value)
	{
		if (pHandle)
			pAdapter.bind_str(pHandle, index, value.c_str(), value.size());
		return *this;
	}


	inline Cursor& Cursor::bind(uint index, bool value)
	{
		if (pHandle)
			pAdapter.bind_bool(pHandle, index, (int) value);
		return *this;
	}


	inline Cursor& Cursor::bind(uint index, sint32 value)
	{
		if (pHandle)
			pAdapter.bind_int32(pHandle, index, value);
		return *this;
	}


	inline Cursor& Cursor::bind(uint index, sint64 value)
	{
		if (pHandle)
			pAdapter.bind_int64(pHandle, index, value);
		return *this;
	}


	inline Cursor& Cursor::bind(uint index, double value)
	{
		if (pHandle)
			pAdapter.bind_double(pHandle, index, value);
		return *this;
	}


	inline Cursor& Cursor::bind(uint index, const NullPtr&)
	{
		if (pHandle)
			pAdapter.bind_null(pHandle, index);
		return *this;
	}


	template<class A1>
	inline Cursor& Cursor::map(const A1& a1)
	{
		(void) bind(0, a1);
		return *this;
	}


	template<class A1, class A2>
	inline Cursor& Cursor::map(const A1& a1, const A2& a2)
	{
		(void) bind(0, a1);
		(void) bind(1, a2);
		return *this;
	}


	template<class A1, class A2, class A3>
	inline Cursor& Cursor::map(const A1& a1, const A2& a2, const A3& a3)
	{
		(void) bind(0, a1);
		(void) bind(1, a2);
		(void) bind(2, a3);
		return *this;
	}


	template<class A1, class A2, class A3, class A4>
	inline Cursor& Cursor::map(const A1& a1, const A2& a2, const A3& a3, const A4& a4)
	{
		(void) bind(0, a1);
		(void) bind(1, a2);
		(void) bind(2, a3);
		(void) bind(3, a4);
		return *this;
	}


	inline DBI::Error Cursor::next()
	{
		assert(pAdapter.cursor_go_to_next != NULL);
		return (pHandle)
			? (DBI::Error) pAdapter.cursor_go_to_next(pHandle)
			: DBI::errNoRow;
	}


	inline DBI::Error Cursor::previous()
	{
		assert(pAdapter.cursor_go_to_previous != NULL);
		return (pHandle)
			? (DBI::Error) pAdapter.cursor_go_to_previous(pHandle)
			: DBI::errNoRow;
	}


	inline Row Cursor::fetch()
	{
		return Row(pAdapter, pHandle);
	}


	inline DBI::Error Cursor::moveTo(uint64 rowindex)
	{
		assert(pAdapter.cursor_go_to != NULL);
		return (pHandle)
			? (DBI::Error) pAdapter.cursor_go_to(pHandle, rowindex)
			: DBI::errNoRow;
	}


	template<class CallbackT>
	inline DBI::Error Cursor::each(const CallbackT& callback)
	{
		do
		{
			// go to the next statement
			auto error = next();

			switch (error)
			{
				case errNone:
					{
						// Got another row !
						Row row(pAdapter, pHandle);
						if (not callback(row))
							return errNone; // asked to stop
						break;
					}
				case errNoRow:
					{
						// no more row
						return errNone;
					}
				default:
					{
						// another error ! Strange however...
						return error;
					}
			}
		}
		while (true);

		// generic error, which should never happen here
		return errFailed;
	}




} // namespace DBI
} // namespace Yuni

#endif // __YUNI_DBI_CURSOR_HXX__
