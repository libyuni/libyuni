#include "tribool.h"
#include <iostream>

using namespace Yuni;




std::ostream& operator << (std::ostream& out, const Yuni::Tribool& rhs)
{
	rhs.print(out);
	return out;
}
