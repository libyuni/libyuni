/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** gitlab: https://gitlab.com/libyuni/libyuni/
** github: https://github.com/libyuni/libyuni/ {mirror}
*/
#pragma once
#include "../../yuni.h"
#include "../string/string.h"
#include <iosfwd>



namespace Yuni
{

	/*!
	** \brief Version number
	**
	** A version number is composed by two numbers (major and minor), plus a
	** patchision number for fine-grained level.
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
		Version() = default;

		/*!
		** \brief Constructor with a given version
		**
		** \param major The major version number
		*/
		explicit Version(uint32_t major);

		/*!
		** \brief Constructor with a given version
		**
		** \param major The major version number
		** \param minor The minor version number
		*/
		Version(uint32_t major, uint32_t minor);

		/*!
		** \brief Constructor with a given version
		**
		** \param major The major version number
		** \param minor The minor version number
		** \param patch   The patchision number
		*/
		Version(uint32_t major, uint32_t minor, uint32_t patch);

		//! Copy constructor
		Version(const Version& c) = default;
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
		void assign(uint32_t major, uint32_t minor, uint32_t r = 0);

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
		uint32_t hi = 0;
		//! The minor version number
		uint32_t lo = 0;
		//! patchision
		uint32_t patch = 0;
		//! Metadata
		String metadata;

	}; // class Version






} // namespace Yuni

#include "version.hxx"
