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
#ifndef __YUNI_DBI_COLUMN_H__
# define __YUNI_DBI_COLUMN_H__



namespace Yuni
{
namespace DBI
{

	/*!
	** \ingroup DBI
	*/
	class Column final
	{
	public:
		//! \name Constructors
		//@{
		//! Default constructor
		explicit Column(::yn_dbi_adapter& adapter, void* handle, uint colindx);
		//! Copy constructor
		Column(const Column& rhs);
		//@}


		//! Get if the value is null
		bool null() const;

		//! Convert the value of the column as bool
		bool asBool() const;

		//! Convert the value of the column as int32
		sint32 asInt() const;

		//! Convert the value of the column as int64
		sint64 asInt64() const;

		//! Convert the value of the column as a double
		double asDouble() const;

		//! Convert the value of the column as a string
		String asString() const;

		/*!
		** \brief Convert the value of the column into a specific type
		**
		** If the convertion is not a standard one, the conversion from
		** a string to this type will be used.
		*/
		template<class T> T as() const;

		/*!
		** \brief Append the value of the column to an existing string
		*/
		template<class StringT> void appendToString(StringT& string);


	private:
		//! Alias to the current channel
		// \internal This reference can be null and must never be called if pHandle is null
		::yn_dbi_adapter& pAdapter;
		//! Opaque pointer to the current query
		void* pHandle;
		//! Column index
		uint pColumnIndex;

	}; // class Column





} // namespace DBI
} // namespace Yuni

# include "column.hxx"

#endif // __YUNI_DBI_COLUMN_H__
