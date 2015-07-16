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
#ifndef __YUNI_CORE_UNIT_LENGTH_METRIC_H__
# define __YUNI_CORE_UNIT_LENGTH_METRIC_H__

# include "length.h"
# include "../unit.h"
# include "../define.h"



namespace Yuni
{
namespace Unit
{
namespace Length
{

	//! \brief Yottametre
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Yottametre, "yottameter", "Ym", int64_t, 1e24);
	//! \brief Zettametre
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Zettametre, "zettameter", "Zm", int64_t, 1e21);
	//! \brief Exametre
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Exametre, "exameter", "Em", int64_t, 1e18);
	//! \brief Petametre
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Petametre, "petameter", "Pm", int64_t, 1e15);
	//! \brief Terametre
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Terametre, "terameter", "Tm", int64_t, 1e12);
	//! \brief Gigametre
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Gigametre, "gigameter", "Gm", int, 1e9);
	//! \brief Megametre
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Megametre, "megameter", "Mm", int, 1e6);
	//! \brief Kilometre
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Kilometre, "kilometer", "km", int, 1e3);
	//! \brief Hectometre
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Hectometre, "hectometer", "hm", int, 1e2);
	//! \brief Decametre
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Decametre, "decameter", "dam", int, 10);


	//! \brief Metre (Distance light travels in 1/299 792 458 of a second in vacuum)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Metre, "meter", "m", int,1);

	//! \brief Decimeter
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Decimetre, "decimetre", "dm", double, 1e-1);
	//! \brief Centimeter
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Centimetre, "centimetre", "cm", double, 1e-2);
	//! \brief Millimeter
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Millimetre, "millimetre", "mm", double, 1e-3);
	//! \brief Micrometre
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Micrometre, "micrometre", "µ,", double, 1e-6);
	//! \brief Nanometre
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Nanometre, "nanometre", "nm,", double, 1e-9);
	//! \brief Picometre
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Picometre, "picometre", "pm,", double, 1e-12);
	//! \brief Fentometre
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Fentometre, "fentometre", "fm,", double, 1e-15);
	//! \brief Attometre
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Attometre, "attometre", "am,", double, 1e-18);
	//! \brief Zeptometre
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Zeptometre, "zeptometre", "zm,", double, 1e-21);
	//! \brief Yoctometre
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Yoctometre, "yoctometre", "ym,", double, 1e-24);



} // namespace Length
} // namespace Unit
} // namespace Yuni

# include "../undef.h"

#endif // __YUNI_CORE_UNIT_LENGTH_METRIC_H__
