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
