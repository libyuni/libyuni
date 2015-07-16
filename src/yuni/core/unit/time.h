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
**    website if you used an unmodified version) â€“ just include a link in your
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
#ifndef __YUNI_CORE_UNIT_TIME_H__
# define __YUNI_CORE_UNIT_TIME_H__

# include "unit.h"
# include "define.h"

namespace Yuni
{
namespace Unit
{


/*!
** \defgroup UnitTime  Time
** \ingroup Units
*/

/*!
** \brief Time
** \ingroup UnitTime
*/
namespace Time
{
	//! \ingroup UnitTime
	struct Quantity;

	//! \brief SI (Second)
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(SIBaseUnit, "second", "s", int, 1);

	//! \brief Millenium (1000 years)
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Millenium, "millenium", "", int64_t, 31556952000);
	//! \brief Century (100 years)
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Century, "century", "c", int64_t, 3155695200);
	//! \brief Decade (10 years)
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Decade, "decade", "dec", int, 315569520);
	//! \brief Gregorian Year (365.2425 days)
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Year, "year", "a", int, 31556952);
	//! \brief Julian Year (365.25 days)
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(JulianYear, "julianyear", "", int, 31557600);
	//! \brief Sidereal Year (time taken for Sun to return to the same position with respect to the stars of the celestial sphere)
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(SiderealYear, "siderealyear", "", double, 31558149.7632);
	//! \brief Tropical Year (time it takes for the Sun to return to the same position in the cycle of seasons)
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(TropicalYear, "tropicalyear", "", int, 31556925);
	//! \brief Gregorian month (Average Gregorian calendar month duration)
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Month, "month", "mo", int, 2629700);
	//! \brief Synodic month (Average cycle time of moon phases)
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(SynodicMonth, "synodicmonth", "", int, 2551000);
	//! \brief Fortnight (2 weeks)
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Fortnight, "fortnight", "fn", int, 1209600);
	//! \brief Week (7 days)
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Week, "week", "wk", int, 604800);
	//! \brief Day (24 hours)
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Day, "day", "d", int, 86400);
	//! \brief Ke (Chinese calendar : 14.4 minutes since 100 ke make a day)
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Ke, "ke", "", 864);
	//! \brief Hour (60 minutes)
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Hour, "hour", "h", int, 3600);
	//! \brief Minute (60 seconds)
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Minute, "minute", "min", int, 60);
	//! \brief Helek (3 and a third seconds, since 1080 halakim make an hour in the Hebrew calendar)
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Helek, "helek", "", double, 10.0/3.0);

	//! \brief Second (Duration of 9,192,631,770 periods of the radiation corresponding to the transition between the two hyperfine levels of the ground state of the caesium-133 atom)
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Second, "second", "s", int, 1);

	//! \brief Decisecond
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Decisecond, "decisecond", "ds", double, 1e-1);
	//! \brief Centisecond
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Centisecond, "centisecond", "cs", double, 1e-2);
	//! \brief Millisecond
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Millisecond, "millisecond", "ms", double, 1e-3);
	//! \brief Microsecond
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Microsecond, "microsecond", "µs,", double, 1e-6);
	//! \brief Nanosecond
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Nanosecond, "nanosecond", "ns,", double, 1e-9);
	//! \brief Picosecond
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Picosecond, "picosecond", "ps,", double, 1e-12);
	//! \brief Fentosecond
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Fentosecond, "fentosecond", "fs,", double, 1e-15);
	//! \brief Attosecond
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Attosecond, "attosecond", "as,", double, 1e-18);
	//! \brief Zeptosecond
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Zeptosecond, "zeptosecond", "zs,", double, 1e-21);
	//! \brief Yoctosecond
	//! \ingroup UnitTime
	YUNI_UNIT_IMPL(Yoctosecond, "yoctosecond", "ys,", double, 1e-24);



} // namespace Time
} // namespace Unit
} // namespace Yuni

# include "../undef.h"

#endif // __YUNI_CORE_UNIT_TIME_H__
