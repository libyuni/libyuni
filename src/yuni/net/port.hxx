#ifndef __YUNI_NET_PORT_HXX__
# define __YUNI_NET_PORT_HXX__


namespace Yuni
{
namespace Net
{

	inline bool Port::Range::IsWellKnown(const Port& port)
	{
		return port.value() < 1024u;
	}


	inline bool Port::Range::IsRegistered(const Port& port)
	{
		return (port.value() >= 1024) and (port.value() <= 49151);
	}


	inline bool Port::Range::IsDynamic(const Port& port)
	{
		return port.value() > 49151;
	}





	inline Port::Port() :
		pValue()
	{}


	inline Port::Port(uint rhs) :
		pValue(rhs)
	{}


	inline Port::Port(const Port& rhs) :
		pValue(rhs.pValue)
	{}


	inline Port::Port(const NullPtr&) :
		pValue()
	{}


	inline uint Port::value() const
	{
		return pValue;
	}


	inline bool Port::valid() const
	{
		return pValue && pValue <= 65535;
	}


	inline bool Port::none() const
	{
		// note: prevent warning from MSVC
		return (pValue == 0) ? true : false;
	}


	inline Port& Port::operator = (uint rhs)
	{
		pValue = rhs;
		return *this;
	}


	inline Port& Port::operator = (const Port& rhs)
	{
		pValue = rhs.pValue;
		return *this;
	}


	inline Port& Port::operator = (const NullPtr&)
	{
		pValue = 0;
		return *this;
	}


	inline Port& Port::operator += (uint rhs)
	{
		pValue += rhs;
		return *this;
	}


	inline Port& Port::operator -= (uint rhs)
	{
		pValue -= rhs;
		return *this;
	}


	inline bool Port::operator == (uint rhs) const
	{
		return rhs == pValue;
	}


	inline bool Port::operator == (const Port& rhs) const
	{
		return rhs.pValue == pValue;
	}


	inline bool Port::operator != (uint rhs) const
	{
		return rhs != pValue;
	}


	inline bool Port::operator != (const Port& rhs) const
	{
		return rhs.pValue != pValue;
	}


	inline bool Port::operator ! () const
	{
		// note: prevent warning from MSVC
		return (pValue == 0) ? true : false;
	}


	inline bool Port::operator < (const Port& rhs) const
	{
		return pValue < rhs.pValue;
	}


	inline bool Port::operator > (const Port& rhs) const
	{
		return pValue > rhs.pValue;
	}


	inline bool Port::operator <= (const Port& rhs) const
	{
		return pValue <= rhs.pValue;
	}


	inline bool Port::operator >= (const Port& rhs) const
	{
		return pValue >= rhs.pValue;
	}


	inline Port::operator uint () const
	{
		return pValue;
	}




} // namespace Net
} // namespace Yuni

#endif // __YUNI_NET_PORT_HXX__
