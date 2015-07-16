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
#ifndef __YUNI_UI_DIMENSION_H__
# define __YUNI_UI_DIMENSION_H__

# include <cassert>

namespace Yuni
{
namespace UI
{

	class ConversionData
	{
	public:
		float unitPerPercentParent;
		float unitPerPercentScreen;
		float unitPerPixel = 1.0f;
		float unitPerPoint = 4.0f / 3.0f; // = 96dpi / 72ppi
		float unitPerEm;
		float unitPerEn;
		float unitPerCentimeters = 243.84f; // = 96 dpi * 2.54 (inches in a cm)

	}; // class ConversionData



	/*!
	** \brief A dimension with a unit, used for placement and sizing of graphical elements
	*/
	class Dimension
	{
	public:
		enum Unit
		{
			uPercentParent,
			uPercentScreen,
			uPixel,
			uPoint,
			uEm,
			uEn,
			uCentimeters

		};

	public:
		Dimension(float value, Unit unit):
			pUnit(unit),
			pValue(value)
		{}

		Dimension(const Dimension& other):
			pUnit(other.pUnit),
			pValue(other.pValue)
		{}

		Dimension& operator = (const Dimension& other)
		{
			pUnit = other.pUnit;
			pValue = other.pValue;
			return *this;
		}

		Dimension& operator = (float value)
		{
			pValue = value;
			return *this;
		}

		void reset(float value, Unit unit)
		{
			pValue = value;
			pUnit = unit;
		}

		float operator () () const { return pValue; }
		float operator () (const ConversionData& conversion) const { return convert(conversion); }
		operator float () const { return pValue; }

		float convert(const ConversionData& conversion) const
		{
			switch (pUnit)
			{
				case uPercentParent:
					return pValue * conversion.unitPerPercentParent;
				case uPercentScreen:
					return pValue * conversion.unitPerPercentScreen;
				case uPixel:
					return pValue * conversion.unitPerPixel;
				case uPoint:
					return pValue * conversion.unitPerPoint;
				case uEm:
					return pValue * conversion.unitPerEm;
				case uEn:
					return pValue * conversion.unitPerEn;
				case uCentimeters:
					return pValue * conversion.unitPerCentimeters;
				default:
					assert(false && "Invalid enum value !");
					return pValue;
			}
		}

	private:
		Unit pUnit;

		float pValue;

	}; // class Dimension



} // namespace UI
} // namespace Yuni



inline Yuni::UI::Dimension operator "" _pcs(long double value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uPercentScreen);
}

inline Yuni::UI::Dimension operator "" _pcs(unsigned long long value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uPercentScreen);
}


inline Yuni::UI::Dimension operator "" _pcp(long double value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uPercentParent);
}

inline Yuni::UI::Dimension operator "" _pcp(unsigned long long value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uPercentParent);
}


inline Yuni::UI::Dimension operator "" _px(long double value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uPixel);
}

inline Yuni::UI::Dimension operator "" _px(unsigned long long value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uPixel);
}


inline Yuni::UI::Dimension operator "" _pt(long double value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uPoint);
}

inline Yuni::UI::Dimension operator "" _pt(unsigned long long value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uPoint);
}


inline Yuni::UI::Dimension operator "" _em(long double value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uEm);
}

inline Yuni::UI::Dimension operator "" _em(unsigned long long value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uEm);
}


inline Yuni::UI::Dimension operator "" _en(long double value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uEn);
}

inline Yuni::UI::Dimension operator "" _en(unsigned long long value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uEn);
}


inline Yuni::UI::Dimension operator "" _cm(long double value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uCentimeters);
}

inline Yuni::UI::Dimension operator "" _cm(unsigned long long value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uCentimeters);
}


#endif // __YUNI_UI_DIMENSION_H__
