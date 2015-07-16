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
#ifndef __YUNI_CORE_UNIT_DATA_DATA_H__
# define __YUNI_CORE_UNIT_DATA_DATA_H__

# include "unit.h"
# include "define.h"


namespace Yuni
{
namespace Unit
{

/*!
** \defgroup UnitData Digital information storage
** \ingroup Units
*/


/*!
** \brief Digital information storage
** \ingroup UnitData
**
**
** \defgroup UnitDataOctet Measured in octets
** \ingroup UnitData
**
** \defgroup UnitDataBit   Measured in bits
** \ingroup UnitData
*/
namespace Data
{


/*!
** \name Digital information storage (measured in bits)
** \ingroup UnitDataBit
*/
namespace Bit
{
	//! \ingroup UnitDataBit
	struct Quantity;



	//! SI (byte)
	//! \ingroup UnitDataBit
	YUNI_UNIT_IMPL(SIBaseUnit, "bit", "B", int, 1);

	//! Zettabit
	//! \ingroup UnitDataBit
	YUNI_UNIT_IMPL(Zettabit, "zettabit", "zB", uint64_t, 1e21);
	//! Exabit
	//! \ingroup UnitDataBit
	YUNI_UNIT_IMPL(Exabit, "exabit", "eB", uint64_t, 1e18);
	//! Petabit
	//! \ingroup UnitDataBit
	YUNI_UNIT_IMPL(Petabit, "petabit", "pB", uint64_t, 1e15);
	//! Terabit
	//! \ingroup UnitDataBit
	YUNI_UNIT_IMPL(Terabit, "terabit", "tB", uint64_t, 1e12);
	//! Gigabit
	//! \ingroup UnitDataBit
	YUNI_UNIT_IMPL(Gigabit, "gigabit", "gB", int, 1e9);
	//! Megabit
	//! \ingroup UnitDataBit
	YUNI_UNIT_IMPL(Megabit, "megabit", "mB", int, 1e6);
	//! Kilobit
	//! \ingroup UnitDataBit
	YUNI_UNIT_IMPL(Kilobit, "kilobit", "kB", int, 1e3);
	//! Hectobit
	//! \ingroup UnitDataBit
	YUNI_UNIT_IMPL(Hectobit, "hectobit", "hB", int, 1e2);


	//! Bit
	//! \ingroup UnitDataBit
	YUNI_UNIT_IMPL(Bit, "bit", "B", int, 1);



} // namespace Bit





/*!
** \name Digital information storage (measured in octets)
** \ingroup UnitDataOctet
*/
namespace Octet
{
	//! \ingroup UnitDataOctet
	struct Quantity;


	//! SI (byte)
	//! \ingroup UnitDataOctet
	YUNI_UNIT_IMPL(SIBaseUnit, "octet", "o", int, 1);

	//! Zettaoctet
	//! \ingroup UnitDataOctet
	YUNI_UNIT_IMPL(Zettaoctet, "zettaoctet", "Zo", int64_t, 1e21);
	//! Exaoctet
	//! \ingroup UnitDataOctet
	YUNI_UNIT_IMPL(Exaoctet, "exaoctet", "Eo", int64_t, 1e18);
	//! Petaoctet
	//! \ingroup UnitDataOctet
	YUNI_UNIT_IMPL(Petaoctet, "petaoctet", "Po", int64_t, 1e15);
	//! Teraoctet
	//! \ingroup UnitDataOctet
	YUNI_UNIT_IMPL(Teraoctet, "teraoctet", "To", int64_t, 1e12);
	//! Gigaoctet
	//! \ingroup UnitDataOctet
	YUNI_UNIT_IMPL(Gigaoctet, "gigaoctet", "Go", int, 1e9);
	//! Megaoctet
	//! \ingroup UnitDataOctet
	YUNI_UNIT_IMPL(Megaoctet, "megaoctet", "Mo", int, 1e6);
	//! Kilooctet
	//! \ingroup UnitDataOctet
	YUNI_UNIT_IMPL(Kilooctet, "kilooctet", "Ko", int, 1e3);


	//! Yobioctet
	//! \ingroup UnitDataOctet
	YUNI_UNIT_IMPL(Yobioctet, "yobioctet", "Yio", long double, 1208925819614629174706176.);
	//! Zebioctet
	//! \ingroup UnitDataOctet
	YUNI_UNIT_IMPL(Zebioctet, "zebioctet", "Zio", long double, 1180591620717411303424.);
	//! Exbictet
	//! \ingroup UnitDataOctet
	YUNI_UNIT_IMPL(Exbioctet, "exbioctet", "Eio", uint64_t, 1152921504606846976LL);
	//! Pebioctet
	//! \ingroup UnitDataOctet
	YUNI_UNIT_IMPL(Pebioctet, "pebioctet", "Pio", uint64_t, 1125899906842624LL);
	//! Tebioctet
	//! \ingroup UnitDataOctet
	YUNI_UNIT_IMPL(Tebioctet, "tebioctet", "Tio", uint64_t, 1099511627776LL);
	//! Gibioctet
	//! \ingroup UnitDataOctet
	YUNI_UNIT_IMPL(Gibioctet, "gibioctet", "Gio", uint, 1073741824);
	//! Mebioctet
	//! \ingroup UnitDataOctet
	YUNI_UNIT_IMPL(Mebioctet, "mebioctet", "Mio", uint, 1048576);
	//! Kibioctet
	//! \ingroup UnitDataOctet
	YUNI_UNIT_IMPL(Kibioctet, "kibioctet", "Kio", uint, 1024);

	//! Bit
	//! \ingroup UnitDataOctet
	YUNI_UNIT_IMPL(Octet, "octet", "o", int, 1);



} // namespace Octet


} // namespace Data




	/*!
	** \brief Conversion from Octet to Bit
	*/
	template<typename FromType, typename ToType>
	struct QuantityConversion<Data::Octet::Quantity, FromType, Data::Bit::Quantity, ToType>
	{
		static ToType Value(const FromType u) {return ToType(u * 8);}
	};

	/*!
	** \brief Conversion from Bit to Octet
	*/
	template<typename FromType, typename ToType>
	struct QuantityConversion<Data::Bit::Quantity, FromType, Data::Octet::Quantity, ToType>
	{
		static ToType Value(const FromType u) {return ToType(u / 8);}
	};



} // namespace Unit
} // namespace Yuni

# include "undef.h"

#endif // __YUNI_CORE_UNIT_DATA_DATA_H__
