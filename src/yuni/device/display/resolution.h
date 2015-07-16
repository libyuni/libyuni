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
#ifndef __YUNI_DEVICE_DISPLAY_RESOLUTION_H__
# define __YUNI_DEVICE_DISPLAY_RESOLUTION_H__

# include "../../yuni.h"
# include <vector>
# include <list>
# include "../../core/string.h"
# include "../../core/smartptr/smartptr.h"



namespace Yuni
{
namespace Device
{
namespace Display
{

	/*!
	** \brief Screen/Monitor resolution
	*/
	class Resolution
	{
	public:
		//! The most suitable smart pointer to use with the class `Resolution`
		typedef SmartPtr<Resolution> Ptr;

		enum
		{
			//! The smallest value allowed for the width of the screen
			minimumWidth  = 320u,
			//! The smallest allowed value for the height of the screen
			minimumHeight = 200u,

			//! The highest allowed for the width of the screen
			maximumWidth  = 8192u, // 8K Video Format
			//! The highest allowed value for the height of the screen
			maximumHeight = 4320u,
		};

		//! Vector of resolutions
		typedef std::vector<Ptr>  Vector;

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
		Resolution(const Resolution& rhs);
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
		Resolution& operator = (const Resolution& p);
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

# include "resolution.hxx"


//! \name Operator overload for stream printing
//@{
inline std::ostream& operator << (std::ostream& out, const Yuni::Device::Display::Resolution& rhs)
{ return rhs.print(out); }
inline std::ostream& operator << (std::ostream& out, const Yuni::Device::Display::Resolution::Ptr& rhs)
{ return rhs->print(out); }
//@}


#endif // __YUNI_DEVICE_DISPLAY_RESOLUTION_H__
