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
#pragma once
#include "tribool.h"



namespace Yuni
{

	inline Tribool::Tribool()
	{
		pValue.flags[0] = -1;
		pValue.flags[1] =  0;
	}


	inline Tribool::Tribool(bool value, bool defvalue)
	{
		pValue.flags[0] = static_cast<yint8>(value);
		pValue.flags[1] = static_cast<yint8>(defvalue);
	}


	inline Tribool::Tribool(const NullPtr&, bool defvalue)
	{
		pValue.flags[0] = static_cast<yint8>(-1);
		pValue.flags[1] = static_cast<yint8>(defvalue);
	}


	inline Tribool::Tribool(const Tribool& rhs)
	{
		pValue.u16 = rhs.pValue.u16;
	}


	inline void Tribool::clear()
	{
		pValue.flags[0] = -1; // reset to indeterminate
		pValue.flags[1] =  0;
	}


	inline bool Tribool::indeterminate() const
	{
		return (pValue.flags[0] < 0);
	}


	inline bool Tribool::defaultValue() const
	{
		return (0 != pValue.flags[1]);
	}

	inline void Tribool::defaultValue(bool defvalue)
	{
		pValue.flags[1] = static_cast<yuint8>(defvalue);
	}


	inline bool Tribool::toBool() const
	{
		return (indeterminate())
			? (0 != pValue.flags[1])   // using the default value
			: (0 != pValue.flags[0]);
	}


	inline Tribool& Tribool::operator = (const NullPtr&)
	{
		pValue.flags[0] = -1;
		return *this;
	}


	inline Tribool& Tribool::operator = (bool value)
	{
		pValue.flags[0] = static_cast<yint8>(value);
		return *this;
	}


	inline Tribool& Tribool::operator = (const Tribool& rhs)
	{
		pValue.u16 = rhs.pValue.u16;
		return *this;
	}


	inline bool Tribool::operator == (bool value) const
	{
		return (operator bool ()) == value;
	}


	inline bool Tribool::operator == (const NullPtr&) const
	{
		return indeterminate();
	}


	inline bool Tribool::operator == (const Tribool& rhs) const
	{
		return pValue.u16 == rhs.pValue.u16;
	}


	inline bool Tribool::operator != (bool value) const
	{
		return (operator bool ()) != value;
	}


	inline bool Tribool::operator != (const NullPtr&) const
	{
		return not indeterminate();
	}


	inline bool Tribool::operator != (const Tribool& rhs) const
	{
		return pValue.u16 != rhs.pValue.u16;
	}


	inline Tribool::operator bool () const
	{
		return toBool();
	}


	template<class StreamT> inline void Tribool::print(StreamT& out) const
	{
		switch (pValue.flags[0])
		{
			case 0:  out.write("false", 5); break;
			case 1:  out.write("true", 4); break;
			default: out.write("indeterminate", 13); break;
		}
	}



} // namespace Yuni



//! \name Operator overload for stream printing
//@{
inline std::ostream& operator << (std::ostream& out, const Yuni::Tribool& rhs)
{
	rhs.print(out);
	return out;
}
//@}



namespace Yuni
{
namespace Extension
{
namespace CString
{

	template<class CStringT>
	class Append<CStringT, Yuni::Tribool>
	{
	public:
		static void Perform(CStringT& s, const Yuni::Tribool& rhs)
		{
			rhs.print(s);
		}
	};


	template<>
	class Into<Yuni::Tribool>
	{
	public:
		enum { valid = 1 };

		template<class StringT> static bool Perform(const StringT& s, Yuni::Tribool& out)
		{
			AnyString s1 = s;
			s1.trim();

			switch (s1.size())
			{
				case 1:
				{
					switch (s1.first())
					{
						case '1': out = true;  return true;
						case '0': out = false; return true;
						case '?': out.clear(); return true;
					}
					break;
				}
				case 0:
				{
					break;
				}
				default:
				{
					Yuni::ShortString16 s2 = s1;
					s2.toLower();
					if ("true" == s2 or "on" == s2 or "yes" == s2)
					{
						out = true;
						return true;
					}
					if ("false" == s2 or "off" == s2 or "no" == s2)
					{
						out = false;
						return true;
					}
					if ("indeterminate" == s2 or "null" == s2 or "undefined" == s2 or "undef" == s2 or "default" == s2)
					{
						out.clear();
						return true;
					}
					break;
				}
			}

			out.clear();
			return false;
		}

		template<class StringT> static Yuni::Tribool Perform(const StringT& s)
		{
			Tribool out;
			Perform(s, out);
			return out;
		}
	};



} // namespace CString
} // namespace Extension
} // namespace Yuni
