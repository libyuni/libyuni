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
#ifndef __YUNI_CORE_HASH_CHECKSUM_CHECKSUM_H__
# define __YUNI_CORE_HASH_CHECKSUM_CHECKSUM_H__

# include "../../../yuni.h"
# include "../../string.h"


namespace Yuni
{

/*!
** \brief Tools which use hashing mechanisms
*/
namespace Hash
{

/*!
** \brief Checksums
*/
namespace Checksum
{



	/*!
	** \brief Checksum Implementation (Abstract)
	*/
	class YUNI_DECL IChecksum
	{
	public:
		//! \name Constructor & Destructor
		//@{
		//! Default constructor
		IChecksum() {}
		//! Destructor
		virtual ~IChecksum() {}
		//@}

		/*!
		** \brief Reset the hash value
		*/
		void reset();

		/*!
		** \brief Compute the hash from a string
		**
		** \param s The string
		** \return The hash value
		*/
		const String& fromString(const String& s);

		/*!
		** \brief Compute the hash from raw data
		**
		** \param rawdata The buffer
		** \param size The size of the buffer. AutoDetectNullChar will make an autodetection of the length
		** \return The hash value
		*/
		virtual const String& fromRawData(const void* rawdata, uint64 size = AutoDetectNullChar) = 0;

		/*!
		** \brief Compute the hash of a given file
		**
		** \param filename The filename to analyze
		** \return The hash value
		*/
		virtual const String& fromFile(const String& filename) = 0;

		/*!
		** \brief Get the last hash value
		*/
		const String& value() const;
		//! Get the hash value
		const String& operator() () const;

		/*!
		** \brief Compute the hash value from a string and returns it
		**
		** \param s The string to compute
		** \return The hash value
		*/
		const String& operator[] (const String& s);

	protected:
		//! The hash value
		String pValue;

	}; // class Hash::IChecksum




} // namespace Checksum
} // namespace Hash
} // namespace Yuni

# include "checksum.hxx"

#endif // __YUNI_CORE_HASH_CHECKSUM_CHECKSUM_H__
