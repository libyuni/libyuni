#ifndef __YUNI_CORE_VERSION_H__
# define __YUNI_CORE_VERSION_H__

# include "../../yuni.h"
# include "../string.h"



namespace Yuni
{


	/*!
	** \brief Version number
	**
	** A version number is composed by two numbers (major and minor), plus a
	** revision number for fine-grained level.
	**
	** \ingroup Core
	*/
	class YUNI_DECL Version
	{
	public:
		/*!
		** \brief Get the version of the Yuni Library
		*/
		static void InternalLib(Version& v);

	public:
		//! \name Constructor
		//@{
		//! Default constructor
		Version();

		/*!
		** \brief Constructor with a given version
		**
		** \param h The major version number
		*/
		explicit Version(uint h);

		/*!
		** \brief Constructor with a given version
		**
		** \param h The major version number
		** \param l The minor version number
		*/
		Version(uint h, uint l);

		/*!
		** \brief Constructor with a given version
		**
		** \param h The major version number
		** \param l The minor version number
		** \param r The revision number
		*/
		Version(uint h, uint l, uint r);

		//! Copy constructor
		Version(const Version& c);
		//@}


		//! \name Conversions
		//@{
		/*!
		** \brief Get the version in an human-readable string
		*/
		String toString() const;
		//@}


		//! \name Initialization
		//@{
		/*!
		** \brief Assign a new version numner
		*/
		void assign(uint h, uint lo = 0, uint r = 0);

		/*!
		** \brief Set the version to 0.0.0
		*/
		void clear();
		//@}


		//! \name Comparisons
		//@{
		/*!
		** \brief Get if the version is null
		*/
		bool null() const;

		/*!
		** \brief Check if this version is less than another one
		*/
		bool isLessThan(const Version& rhs) const;
		/*!
		** \brief Check if this version is equal to another one
		*/
		bool isEqualTo(const Version& rhs) const;
		/*!
		** \brief Check if this version is greater than another one
		*/
		bool isGreaterThan(const Version& rhs) const;
		//@}


		//! \name ostream
		//@{
		/*!
		** \brief Print the version to a ostream
		*/
		void print(std::ostream& out) const;
		//@}


		//! \name Operators
		//@{
		//! The operator <
		bool operator <  (const Version& rhs) const;
		//! The operator <=
		bool operator <= (const Version& rhs) const;
		//! The operator >
		bool operator >  (const Version& rhs) const;
		//! The operator <=
		bool operator >= (const Version& rhs) const;

		//! The operator ==
		bool operator == (const Version& rhs) const;
		//! The operator !=
		bool operator != (const Version& rhs) const;

		//! The operator =
		Version& operator = (const Version& rhs);
		//@}


	public:
		//! The major version number
		uint hi;
		//! The minor version number
		uint lo;
		//! Revision
		uint revision;

	}; // class Version




} // namespace Yuni

# include "version.hxx"

#endif // __YUNI_CORE_VERSION_H__
