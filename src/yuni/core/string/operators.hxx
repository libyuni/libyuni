#ifndef __YUNI_CORE_STRING_OPERATORS_HXX__
# define __YUNI_CORE_STRING_OPERATORS_HXX__




//! \name Operator overload for stream printing
//@{

template<uint SizeT, bool ExpT>
inline std::ostream& operator << (std::ostream& out, const Yuni::CString<SizeT,ExpT>& rhs)
{
	out.write(rhs.data(), static_cast<std::streamsize>(rhs.size()));
	return out;
}

inline std::ostream& operator << (std::ostream& out, const Yuni::UTF8::Char& rhs)
{
	rhs.write(out);
	return out;
}


namespace Yuni
{
namespace Private
{

	template<class ContainerT>
	class OStreamYuniHelper
	{
	public:
		static void Print(std::ostream& out, const ContainerT& rhs)
		{
			switch (rhs.size())
			{
				case 0:
					break;
				case 1:
					{
						out << rhs[0];
						break;
					}
				default:
					{
						const Yuni::String::Vector::const_iterator end = rhs.end();
						Yuni::String::Vector::const_iterator i = rhs.begin();
						out << *i;
						++i;
						for (; i != end; ++i)
						{
							out.write(", ", 2);
							out << *i;
						}
						break;
					}
			}
		}

	}; // class OStreamYuniHelper

} // namespace Private
} // namespace Yuni



template<class T, class AllocatorT>
inline std::ostream& operator << (std::ostream& out, const std::vector<T, AllocatorT>& rhs)
{
	typedef std::vector<T, AllocatorT>  SourceType;
	Yuni::Private::OStreamYuniHelper<SourceType>::Print(out, rhs);
	return out;
}


template<class T, class AllocatorT>
inline std::ostream& operator << (std::ostream& out, const std::list<T, AllocatorT>& rhs)
{
	typedef std::list<T, AllocatorT>  SourceType;
	Yuni::Private::OStreamYuniHelper<SourceType>::Print(out, rhs);
	return out;
}


template<class CharT, class TraitsT, uint SizeT, bool ExpT>
inline std::basic_istream<CharT, TraitsT>& operator >> (std::basic_istream<CharT, TraitsT>& stream, Yuni::CString<SizeT,ExpT>& string)
{
	string.clear();
	while (stream.good())
	{
		char c;
		stream.get(c);
		if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
			break;
		string.append(c);
	}
	return stream;
}



template<uint SizeT, bool ExpT>
inline Yuni::CString<SizeT,ExpT>
operator + (const Yuni::CString<SizeT,ExpT>& rhs, const char* const u)
{
	return Yuni::CString<SizeT,ExpT>(rhs) += u;
}

template<uint SizeT, bool ExpT>
inline Yuni::CString<SizeT,ExpT>
operator + (const Yuni::CString<SizeT,ExpT>& rhs, const wchar_t* u)
{
	return Yuni::CString<SizeT,ExpT>(rhs) += u;
}

template<uint SizeT, bool ExpT>
inline Yuni::CString<SizeT,ExpT>
operator + (const Yuni::CString<SizeT,ExpT>& rhs, const char u)
{
	return Yuni::CString<SizeT,ExpT>(rhs) += u;
}

template<uint SizeT, bool ExpT>
inline Yuni::CString<SizeT,ExpT>
operator + (const Yuni::CString<SizeT,ExpT>& rhs, const wchar_t u)
{
	return Yuni::CString<SizeT,ExpT>(rhs) += u;
}



template<uint SizeT, bool ExpT>
inline Yuni::CString<SizeT,ExpT>
operator + (const wchar_t* u, const Yuni::CString<SizeT,ExpT>& rhs)
{
	return Yuni::CString<SizeT,ExpT>(u) += rhs;
}

template<uint SizeT, bool ExpT>
inline Yuni::CString<SizeT,ExpT>
operator + (const char* const u, const Yuni::CString<SizeT,ExpT>& rhs)
{
	return Yuni::CString<SizeT,ExpT>(u) += rhs;
}

template<uint SizeT, bool ExpT>
inline Yuni::CString<SizeT,ExpT>
operator + (const char u, const Yuni::CString<SizeT,ExpT>& rhs)
{
	return Yuni::CString<SizeT,ExpT>(u) += rhs;
}

template<uint SizeT, bool ExpT>
inline Yuni::CString<SizeT,ExpT>
operator + (const wchar_t u, const Yuni::CString<SizeT,ExpT>& rhs)
{
	return Yuni::CString<SizeT,ExpT>(u) += rhs;
}




template<uint SizeT, bool ExpT, class TraitsT, class AllocT>
inline Yuni::CString<SizeT,ExpT>
operator + (const std::basic_string<char,TraitsT,AllocT>& u, const Yuni::CString<SizeT,ExpT>& rhs)
{
	return Yuni::CString<SizeT,ExpT>(u) += rhs;
}


template<uint SizeT, bool ExpT, class TraitsT, class AllocT>
inline Yuni::CString<SizeT,ExpT>
operator + (const Yuni::CString<SizeT,ExpT>& rhs, const std::basic_string<char,TraitsT,AllocT>& u)
{
	return Yuni::CString<SizeT,ExpT>(rhs) += u;
}


template<uint SizeT, bool ExpT>
inline bool
operator == (const char* lhs, const Yuni::CString<SizeT,ExpT>& rhs)
{
	return rhs == lhs;
}

template<uint SizeT, bool ExpT>
inline bool
operator != (const char* lhs, const Yuni::CString<SizeT,ExpT>& rhs)
{
	return rhs != lhs;
}

template<int N, uint SizeT, bool ExpT>
inline bool
operator == (const char lhs[N], const Yuni::CString<SizeT,ExpT>& rhs)
{
	return rhs == lhs;
}

template<int N, uint SizeT, bool ExpT>
inline bool
operator != (const char lhs[N], const Yuni::CString<SizeT,ExpT>& rhs)
{
	return rhs != lhs;
}

template<class TraitsT, class AllocT, uint SizeT, bool ExpT>
inline bool
operator == (const std::basic_string<char,TraitsT,AllocT>& lhs, const Yuni::CString<SizeT,ExpT>& rhs)
{
	return rhs == lhs;
}

template<class TraitsT, class AllocT, uint SizeT, bool ExpT>
inline bool
operator != (const std::basic_string<char,TraitsT,AllocT>& lhs, const Yuni::CString<SizeT,ExpT>& rhs)
{
	return rhs != lhs;
}


//@}




# ifdef YUNI_HAS_STL_HASH

namespace std
{
	template<uint SizeT, bool ExpT>
	struct YUNI_DECL hash<Yuni::CString<SizeT, ExpT> > final
	{
		inline size_t operator() (const Yuni::CString<SizeT, ExpT>& string) const
		{
			return string.hash();
		}
	};

} // namespace std

# endif // YUNI_HAS_STL_HASH

#endif // __YUNI_CORE_STRING_OPERATORS_HXX__
