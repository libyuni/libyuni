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
#ifndef __YUNI_CORE_UNIT_LENGTH_EXTRA_H__
# define __YUNI_CORE_UNIT_LENGTH_EXTRA_H__

# include "length.h"
# include "../unit.h"
# include "../define.h"


namespace Yuni
{
namespace Unit
{
namespace Length
{

	//! \brief Astronomical unit (Distance from Earth to Sun)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Astronomical, "astronomical", "AU", int64_t, 149597871464LL);
	//! \brief barleycorn (H)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Barleycorn, "barleycorn", "", double, 0.008);

	//! \brief Cable length (Imperial), equivalent to 608 ft
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(CableLengthImperial, "cable length (Imperial)", "", double, 185.3184);
	//! \brief Cable length (International), equivalent to 1/10 nmi
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(CableLength, "cable length", "", double, 185.2);
	//! \brief Cable length (International), equivalent to 720 ft
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(CableLengthUS, "cable length (U.S.)", "", double, 219.456);

	//! \brief Chain (Gunter's; Surveyor's)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Chain, "chain (Gunter's; Surveyor's)", "ch", double, 20.1168);
	//! \brief Cubit (H) (Distance from fingers to elbow ≈ 18in)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Cubit, "cubit", "", double, 0.5);
	//! \brief Ell (H)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Ell, "ell", "ell", double, 1.143);
	//! \brief Fathom
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Fathom, "fathom", "fm", double, 1.8288);

	//! \brief Fermi
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Fermi, "fermi", "fm", double, 1e-15);

	//! \brief Finger
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Finger, "finger", "", double, 0.022225);

	//! \brief Foot (Benoît)(H)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(FootBenoit, "foot (Benoit)", "ft(Ben)", double, 0.304799735);
	//! \brief Foot (Clarke's; Cape)(H)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(FootClarkeCape, "foot (Clarke,Cape)", "ft(Cla)", double, 0.3047972654);
	//! \brief Foot (Indian)(H)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(FootIndian, "foot (Indian)", "ft(Ind)", double, 0.304799514);
	//! \brief Foot (⅓ yd = 12 inches)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Foot, "foot", "ft", double, 0.3048);
	//! \brief foot (Sear's)(H)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(FootSear, "foot (Sear)", "ft (Sear)", double, 0.30479947);
	//! \brief foot (U.S. Survey)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(FootUS, "foot (U.S. Survey)", "ft (U.S.)", double, 0.304800610);

	//! \brief French; charriere (French catheter scale)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(FrenchCharriere, "french catheter scale", "F", double, 3.3e-4);
	//! \brief Furlong
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Furlong, "furlong", "fur", double, 201.168);
	//! \brief Geographical mile (H) (6082 ft)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(GeographicalMile, "geographical mile", "", double, 1853.7936);
	//! \brief Hand
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Hand, "hand", "", double, 0.1016);

	//! \brief Inch
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Inch, "inch", "in", double, 0.0254);

	//! \brief League
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(League, "league", "lea", double, 4828.032);

	//! \brief Light-day (24 light-hours)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(LightDay, "light-day", "", int64_t, 2.59020683712e13);
	//! \brief Light-day (60 light-minutes)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(LightHour, "light-hour", "", int64_t, 1.0792528488e12);
	//! \brief Light-day (60 light-seconds)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(LightMinute, "light-minute", "", int64_t, 1.798754748e10);
	//! \brief Light-day (Distance light travels in one second in vacuum)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(LightSecond, "light-second", "", int, 2.99792458e8);
	//! \brief Light-dyear
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(LightYear, "light-year", "l.y.", long double, 9.4607304725808e15);

	//! \brief Line
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Line, "line", "ln", double, 0.002116);
	//! \brief link (Gunter's; Surveyor's)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(LinkGunter, "link (Gunter's; Surveyor's)", "lnk", double, 0.201168);
	//! \brief Link (Ramsden's; Engineer's)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(LinkRamsden, "link (Ramsden's; Engineer's)", "lnk", double, 0.3048);
	//! \brief Mickey
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Mickey, "mickey", "", double, 1.27e-4);

	//! \brief Micron
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Micron, "micron", "µ", double, 1e-6);

	//! \brief Mil (Sweden and Norway)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Mil, "mil", "mil", int, 10000);
	//! \brief Mile
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Mile, "mile", "mi", double, 1609.344);
	//! \brief Mile (U.S. Survey)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(MileUS, "mile (U.S.)", "mi", double, 1609.347219);
	//! \brief Nail (cloth)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Nail, "nail", "", double, 0.05715);
	//! \brief Nautical league
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(NauticalLeague, "nautical league", "nl", int, 5556);
	//! \brief Nautical Mile
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(NauticalMile, "nautical mile", "nmi", int, 1852);
	//! \brief Nautical mile (Admiralty)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(NauticalMileAdmiralty, "nautical mile (admiralty)", "nmii(Adm)", double, 1853.184);



	//! \brief Parsec (Distance of star with parallax shift of one arc second from a base of one astronomical unit)
	//! \ingroup UnitLength
	YUNI_UNIT_IMPL(Parsec, "parsec", "pc", int64_t, 3.08568025e16);



} // namespace Length
} // namespace Unit
} // namespace Yuni

# include "../undef.h"

#endif/// __YUNI_CORE_UNIT_LENGTH_EXTRA_H__
