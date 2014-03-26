
#include "dimension.h"



Yuni::UI::Dimension operator "" _px(long double value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uPixel);
}
Yuni::UI::Dimension operator "" _px(unsigned long long value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uPixel);
}

Yuni::UI::Dimension operator "" _pt(long double value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uPoint);
}
Yuni::UI::Dimension operator "" _pt(unsigned long long value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uPoint);
}

Yuni::UI::Dimension operator "" _em(long double value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uEm);
}
Yuni::UI::Dimension operator "" _em(unsigned long long value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uEm);
}

Yuni::UI::Dimension operator "" _en(long double value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uEn);
}
Yuni::UI::Dimension operator "" _en(unsigned long long value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uEn);
}

Yuni::UI::Dimension operator "" _cm(long double value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uCentimeters);
}
Yuni::UI::Dimension operator "" _cm(unsigned long long value)
{
	return Yuni::UI::Dimension((float)value, Yuni::UI::Dimension::uCentimeters);
}

