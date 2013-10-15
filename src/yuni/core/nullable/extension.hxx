#ifndef __YUNI_CORE_NULLABLE_EXTENSION_HXX__
# define __YUNI_CORE_NULLABLE_EXTENSION_HXX__


namespace Yuni
{
namespace Extension
{
namespace CString
{

	template<class CStringT, class T, class Alloc>
	struct Append<CStringT, Yuni::Nullable<T, Alloc> >
	{
		static void Perform(CStringT& s, const Yuni::Nullable<T,Alloc>& rhs)
		{
			if (!rhs.null())
				s << rhs.value();
		}
	};



	template<class T, class Alloc>
	class Into<Yuni::Nullable<T,Alloc> >
	{
	public:
		typedef Yuni::Nullable<T,Alloc> TargetType;
		enum { valid = 1 };

		template<class StringT> static bool Perform(const StringT& s, TargetType& out)
		{
			T tmp;
			if (s.to(tmp))
				out = tmp;
			else
				out = nullptr;
			return true;
		}

		template<class StringT> static TargetType Perform(const StringT& s)
		{
			return s.template to<T>();
		}

	};






} // namespace CString
} // namespace Extension
} // namespace Yuni



template<typename T, class Alloc>
inline std::ostream& operator << (std::ostream& out, const Yuni::Nullable<T,Alloc>& rhs)
{
	rhs.print(out, "(null)");
	return out;
}


#endif // __YUNI_CORE_NULLABLE_EXTENSION_HXX__
