#ifndef __YUNI_DBI_COLUMN_H__
# define __YUNI_DBI_COLUMN_H__



namespace Yuni
{
namespace DBI
{

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
