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
#include "../../yuni.h"
#include <vector>
#include <list>
#include "../../core/string.h"
#include "../../core/smartptr/smartptr.h"



namespace Yuni
{
namespace Device
{
namespace Display
{

	/*!
	** \brief Screen/Monitor resolution
	*/
	class Resolution final
	{
	public:
		//! The smallest value allowed for the width of the screen
		constexpr static uint32_t minimumWidth  = 320;
		//! The smallest allowed value for the height of the screen
		constexpr static uint32_t minimumHeight = 200;
		//! The highest allowed for the width of the screen
		constexpr static uint32_t maximumWidth  = 8192; // 8K Video Format
		//! The highest allowed value for the height of the screen
		constexpr static uint32_t maximumHeight = 4320;

	public:
		//! \name Constructors
		//@{
		/*!
		** \brief Constructor
		**
		** \param width  Width of the monitor/screen
		** \param height Height of the monitor/screen
		** \param bits   Bit per Pixel
		*/
		Resolution(uint width, uint height, uint8 bits = 32);

		/*!
		** \brief Constructor by copy
		** \param rhs The instance to copy
		*/
		Resolution(const Resolution& rhs) = default;
		//@}

		/*!
		** \brief Convert these informations into an human readable string
		*/
		String toString() const;


		//! \name Information about the current mode
		//@{
		//! The width of the monitor/screen
		uint width() const;
		//! The height of the monitor/screen
		uint height() const;
		//! Bit per pixel
		uint8 bitPerPixel() const;
		//@}


		//! \name Operators
		//@{
		/*!
		** \brief Comparison operator (equal with)
		**
		** \param rhs The other resolution to compare with
		** \return True if the two resolution are equal
		*/
		bool operator == (const Resolution& rhs) const;

		/*!
		** \brief Comparison operator (non equal with)
		**
		** \param rhs The other resolution to compare with
		** \return True if the two resolution are not equal
		*/
		bool operator != (const Resolution& rhs) const;

		/*!
		** \brief Comparison operator (less than)
		**
		** \param rhs The other resolution to compare with
		** \return True if *this < rhs
		*/
		bool operator < (const Resolution& rhs) const;

		/*!
		** \brief Comparison operator (less than or equal)
		**
		** \param rhs The other resolution to compare with
		** \return True if *this <= rhs
		*/
		bool operator <= (const Resolution& rhs) const;

		/*!
		** \brief Comparison operator (greater than)
		**
		** \param rhs The other resolution to compare with
		** \return True if *this > rhs
		*/
		bool operator > (const Resolution& rhs) const;

		/*!
		** \brief Comparison operator (greater than or equal)
		**
		** \param rhs The other resolution to compare with
		** \return True if *this >= rhs
		*/
		bool operator >= (const Resolution& rhs) const;

		/*!
		** \brief Assign new values from another resolution
		**
		** \param p The new values
		** \return Always *this
		*/
		Resolution& operator = (const Resolution& p) = default;
		//@}


		//! \name Stream printing
		//@{
		/*!
		** \brief Print the resolution
		**
		** \param[in,out] out An output stream
		** \return The output stream `out`
		*/
		std::ostream& print(std::ostream& out) const;
		//@}

	private:
		//! Height of the screen
		uint pWidth;
		//! Width of the screen
		uint pHeight;
		//! Bits per pixel
		uint8 pBitsPerPixel;

	}; // class Resolution





} // namespace Display
} // namespace Device
} // namespace Yuni

#include "resolution.hxx"


//! \name Operator overload for stream printing
//@{
inline std::ostream& operator << (std::ostream& out, const Yuni::Device::Display::Resolution& rhs)
{ return rhs.print(out); }
//@}
