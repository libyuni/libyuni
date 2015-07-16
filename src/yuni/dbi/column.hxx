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
#ifndef __YUNI_DBI_COLUMN_HXX__
# define __YUNI_DBI_COLUMN_HXX__



namespace Yuni
{
namespace DBI
{

	inline Column::Column(::yn_dbi_adapter& adapter, void* handle, uint colindx) :
		pAdapter(adapter),
		pHandle(handle),
		pColumnIndex(colindx)
	{}


	inline Column::Column(const Column& rhs) :
		pAdapter(rhs.pAdapter),
		pHandle(rhs.pHandle),
		pColumnIndex(rhs.pColumnIndex)
	{}


	inline bool Column::null() const
	{
		return (0 != pAdapter.column_is_null(pHandle, pColumnIndex));
	}


	inline String Column::asString() const
	{
		uint length;
		const char* text = pAdapter.column_to_cstring(pHandle, pColumnIndex, &length);
		return String(text, length);
	}


	template<class StringT>
	inline void Column::appendToString(StringT& string)
	{
		uint length;
		const char* text = pAdapter.column_to_cstring(pHandle, pColumnIndex, &length);
		string.append(text, length);
	}


	inline bool Column::asBool() const
	{
		return (0 != pAdapter.column_to_int32(pHandle, pColumnIndex));
	}


	inline double Column::asDouble() const
	{
		return pAdapter.column_to_double(pHandle, pColumnIndex);
	}


	inline sint32 Column::asInt() const
	{
		return pAdapter.column_to_int32(pHandle, pColumnIndex);
	}


	inline sint64 Column::asInt64() const
	{
		return pAdapter.column_to_int64(pHandle, pColumnIndex);
	}




	template<class T>
	inline T Column::as() const
	{
		const String& text = asString();
		return text.to<T>();
	}


	template<> inline String Column::as<String>() const
	{
		return asString();
	}

	template<> inline bool Column::as<bool>() const
	{
		return asBool();
	}

	template<> inline double Column::as<double>() const
	{
		return asDouble();
	}

	template<> inline float Column::as<float>() const
	{
		return (float) asDouble();
	}

	template<> inline sint32 Column::as<sint32>() const
	{
		return asInt();
	}

	template<> inline sint64 Column::as<sint64>() const
	{
		return asInt64();
	}




} // namespace DBI
} // namespace Yuni

#endif // __YUNI_DBI_COLUMN_HXX__
