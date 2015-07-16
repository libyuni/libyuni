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
#ifndef __YUNI_NET_PORT_H__
# define __YUNI_NET_PORT_H__

# include "../yuni.h"


namespace Yuni
{
namespace Net
{

	/*!
	** \brief Socket Port number
	**
	** \see http://www.iana.org/assignments/port-numbers
	*/
	class Port
	{
	public:
		struct Range
		{
			//! Get if the port is well-known (0..1023)
			static bool IsWellKnown(const Port& port);
			//! Get if the port is registered (0..49151)
			static bool IsRegistered(const Port& port);
			//! Get if the port is dynamic and/or private (>= 49152)
			static bool IsDynamic(const Port& port);
		};

	public:
		//! \name Constructors
		//@{
		//! Default constructor
		Port();
		//! Constructor with a given value
		Port(uint rhs);
		//! Copy constructor
		Port(const Port& rhs);
		//! Constructor from a null pointer
		Port(const NullPtr&);
		//@}

		//! \name Value
		//@{
		//! Get the port number
		uint value() const;
		//! Get if the port is valid
		bool valid() const;

		//! Get if no port is allocated
		bool none() const;
		//@}

		//! \name Operators
		//@{
		//! Assignment
		Port& operator = (uint rhs);
		Port& operator = (const Port& rhs);
		Port& operator = (const Yuni::NullPtr&);
		//! Append
		Port& operator += (uint rhs);
		//! Sub
		Port& operator -= (uint rhs);
		//! Comparison
		bool operator == (uint rhs) const;
		bool operator == (const Port& rhs) const;
		bool operator != (uint rhs) const;
		bool operator != (const Port& rhs) const;
		bool operator ! () const;
		//! Inequality
		bool operator < (const Port& rhs) const;
		bool operator > (const Port& rhs) const;
		bool operator <= (const Port& rhs) const;
		bool operator >= (const Port& rhs) const;

		//! Cast uint
		operator uint () const;
		//@}

	private:
		//! Port value
		uint pValue;

	}; // class Port






} // namespace Net
} // namespace Yuni

# include "port.hxx"

#endif // __YUNI_NET_PORT_H__
