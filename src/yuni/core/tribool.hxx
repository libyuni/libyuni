#ifndef __YUNI_CORE_TRIBOOL_HXX__
# define __YUNI_CORE_TRIBOOL_HXX__

# include "tribool.h"



namespace Yuni
{

	inline Tribool::Tribool()
		: pValue(-1)
	{}


	inline Tribool::Tribool(bool value)
		: pValue((yint8) value)
	{}


	inline Tribool::Tribool(const NullPtr&)
		: pValue(-1)
	{}


	inline Tribool::Tribool(const Tribool& rhs)
		: pValue(rhs.pValue)
	{}


	inline void Tribool::clear()
	{
		pValue = -1;
	}


	inline bool Tribool::indeterminate() const
	{
		return (pValue == -1);
	}


	inline Tribool& Tribool::operator = (const NullPtr&)
	{
		pValue = -1;
		return *this;
	}


	inline Tribool& Tribool::operator = (bool value)
	{
		pValue = (yint8) value;
		return *this;
	}


	inline Tribool& Tribool::operator = (const Tribool& rhs)
	{
		pValue = rhs.pValue;
		return *this;
	}


	inline bool Tribool::operator == (bool value) const
	{
		return (pValue == (yint8) value);
	}


	inline bool Tribool::operator == (const NullPtr&) const
	{
		return (pValue == -1);
	}


	inline bool Tribool::operator == (const Tribool& rhs) const
	{
		return pValue == rhs.pValue;
	}


	inline Tribool::operator bool () const
	{
		return pValue == 1;
	}


	template<class StreamT> inline void Tribool::print(StreamT& out) const
	{
		switch (pValue)
		{
			case 1:  out << "true"; break;
			case 0:  out << "false"; break;
			case -1: out << "indeterminate"; break;
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
					if ("indeterminate" == s2 or "null" == s2 or "undefined" == s2 or "undef" == s2)
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

#endif // __YUNI_CORE_TRIBOOL_HXX__
