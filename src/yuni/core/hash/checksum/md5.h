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
#ifndef __YUNI_CORE_HASH_CHECKSUM_MD5_H__
# define __YUNI_CORE_HASH_CHECKSUM_MD5_H__

# include "../../../yuni.h"
# include "checksum.h"



namespace Yuni
{
namespace Hash
{
namespace Checksum
{


	/*!
	** \brief MD5 Checksum
	**
	** Compute the MD5 from a string :
	** \code
	** Yuni::Hash::Checksum::MD5 md5;
	** md5.fromString("Hello world");
	** std::cout << md5.value() << std::endl;
	** \endcode
	**
	** Another way to do it (1/2 - recommended) :
	** \code
	** Yuni::Hash::Checksum::MD5 md5;
	** std::cout << md5.fromString("Hello world") << std::endl;
	** std::cout << md5.value() << std::endl; // will remain the same than before
	** \endcode
	**
	** Another way to do it (2/2) :
	** \code
	** std::cout << Yuni::Hash::Checksum::MD5::FromString("Hello world") << std::endl;
	** \endcode
	**
	** Compute a lot of MD5 from a string :
	** \code
	** MD5 md5;
	** std::cout << md5["Hello"] << std::endl;
	** std::cout << md5["World"] << std::endl;
	** std::cout << md5["Hello world"] << std::endl;
	** \endcode
	*/
	class YUNI_DECL MD5 final : public Hash::Checksum::IChecksum
	{
	public:
		/*!
		** \brief Compute the hash from a string
		**
		** \param s The string
		** \return The hash value
		*/
		static String FromString(const String& s);

		/*!
		** \brief Compute the hash from raw data
		**
		** \param rawdata The original buffer
		** \param size Size of the given buffer.
		** \return The hash value
		*/
		static String FromRawData(const void* rawdata, uint64 size = AutoDetectNullChar);

	public:
		//! \name Constructor & Destructor
		//@{
		//! Default constructor
		MD5() {}
		//! Destructor
		virtual ~MD5() {}
		//@}

		virtual const String& fromRawData(const void* rawdata, uint64 size = AutoDetectNullChar) override;
		virtual const String& fromFile(const String& filename) override;

	}; // class Hash::Checksum::MD5




} // namespace Checksum
} // namespace Hash
} // namespace Yuni

# include "md5.hxx"

#endif // __YUNI_CORE_HASH_CHECKSUM_MD5_H__
