/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** github: https://github.com/libyuni/libyuni/
** gitlab: https://gitlab.com/libyuni/libyuni/ (mirror)
*/
#pragma once
#include "../yuni.h"
#include "fwd.h"
#include "../core/string.h"


namespace Yuni
{

	/*!
	** \brief Universally unique identifier (UUID)
	**
	** Printing to cout a new UUID :
	** \code
	** #include <yuni/yuni.h>
	** #include <yuni/uuid/uuid.h>
	** #include <iostream>
	**
	** using namespace Yuni;
	**
	** int main()
	** {
	**	UUID uuid;
	**	uuid.generate();
	**	std::cout << "GUID: " << uuid << std::endl;
	**	return 0;
	** }
	** \endcode
	**
	** Conversions :
	** \code
	** UUID uuid;
	** uuid.generate();
	** String s = uuid;
	** std::cout << s << std::endl;
	**
	** uuid = "BDBBD619-DA48-4BAE-B62A-CFFF1C291A8C";
	** std::cout << uuid << std::endl;
	**
	** s = "70395DB8-E113-46D8-A2C0-522B59B03A96";
	** uuid = s;
	** std::cout << uuid << std::endl;
	** \endcode
	*/
	struct YUNI_DECL UUID final
	{
		enum class Flag
		{
			//! Set the UUID to null
			null,
			//! Generate a new UUID
			generate,
		};

		//! \name Constructor
		//@{
		/*!
		** \brief Default constructor
		**
		** The UUID will be set to null.
		*/
		UUID();
		//! Copy constructor
		UUID(const UUID& rhs) = default;
		/*!
		** \brief Constructor with flags
		** \param flag Flag to know how to initialize the UUID
		*/
		explicit UUID(Flag flag);
		/*!
		** \brief Constructor from a string (ex: "c705cf38-1d2f-4d7f-93d4-6b84c44a45b9")
		** \see assign()
		*/
		UUID(const AnyString& string);
		//@}


		//! \name UUID
		//@{
		//! Generate a new UUID
		void generate();

		//! Reset the UUID to null
		void clear();

		//! Get if the UUID is null (e.g. 000000000-0000-0000-0000-00000000000)
		bool null() const;

		/*!
		** \brief Assign from a string
		**
		** The input string will be trimmed
		**
		** \code
		** UUID uuid;
		** uuid.assign(" C32b24FE-71AE-440D-a473-56355415BDB1");
		** uuid.assign("{C8B8431C-C3F3-4FBC-BB39-DC8DA8D801A7}");
		** \endcode
		*/
		void assign(AnyString string);
		//@}


		//! \name Hashing
		//@{
		//! hash of the uuid
		size_t hash() const;
		//@}


		//! \name Operators
		//@{
		//! Copy operator
		UUID& operator = (const UUID& rhs) = default;
		/*!
		** \brief Assignment from a string
		**
		** The UUID will be reset if the conversion failed.
		*/
		UUID& operator = (const AnyString& string);

		//! Null
		bool operator ! () const;

		//! Comparison
		bool operator == (const UUID& rhs) const;
		//! Comparison
		bool operator != (const UUID& rhs) const;
		//! Strict weak comparison
		bool operator < (const UUID& rhs) const;
		//! Greater strict
		bool operator > (const UUID& rhs) const;
		//! Less or equal
		bool operator <= (const UUID& rhs) const;
		//! Greater or equal
		bool operator >= (const UUID& rhs) const;
		//@}


	private:
		//! Write the UUID into string in a C-String (with at least 36 bytes)
		void writeToCString(char*) const;

	private:
		union StorageType
		{
			uint64 u64[2];
			uint32 u32[4];
			uchar ubytes[16];
		};

	private:
		//! Inner value
		StorageType m_value;
		// Friend
		friend class Yuni::Private::UUID::Helper;

	}; // class UUID


} // namespace Yuni

#include "uuid.hxx"
