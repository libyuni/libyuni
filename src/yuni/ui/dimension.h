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
		float unitPerPixel;
		float unitPerPoint;
		float unitPerEm;
		float unitPerEn;
		float unitPerCentimeters;

	}; // class DimensionConversionData



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

		float operator () () const { return pValue; }
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




Yuni::UI::Dimension operator "" _px(long double value);
Yuni::UI::Dimension operator "" _px(unsigned long long value);

Yuni::UI::Dimension operator "" _pt(long double value);
Yuni::UI::Dimension operator "" _pt(unsigned long long value);

Yuni::UI::Dimension operator "" _em(long double value);
Yuni::UI::Dimension operator "" _em(unsigned long long value);

Yuni::UI::Dimension operator "" _en(long double value);
Yuni::UI::Dimension operator "" _en(unsigned long long value);

Yuni::UI::Dimension operator "" _cm(long double value);
Yuni::UI::Dimension operator "" _cm(unsigned long long value);


#endif // __YUNI_UI_DIMENSION_H__
