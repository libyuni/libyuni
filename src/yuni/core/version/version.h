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
	class YUNI_DECL Version final
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
		** \param major The major version number
		*/
		explicit Version(uint major);

		/*!
		** \brief Constructor with a given version
		**
		** \param major The major version number
		** \param minor The minor version number
		*/
		Version(uint major, uint minor);

		/*!
		** \brief Constructor with a given version
		**
		** \param major The major version number
		** \param minor The minor version number
		** \param rev   The revision number
		*/
		Version(uint major, uint minor, uint rev);

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
		void assign(uint major, uint minor, uint r = 0);

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
		template<class S> void print(S& out) const;
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
