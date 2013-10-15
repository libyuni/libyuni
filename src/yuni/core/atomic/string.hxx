#ifndef __YUNI_CORE_ATOMIC_STRING_HXX__
# define __YUNI_CORE_ATOMIC_STRING_HXX__


namespace Yuni
{
namespace Extension
{
namespace CString
{

	template<class CStringT, class C> class Append;
	template<class T> class Into;


	// Atomic<>
	template<class CStringT, int SizeT, template<class> class TP>
	class Append<CStringT, Yuni::Atomic::Int<SizeT,TP> >
	{
	public:
		typedef typename CStringT::Type TypeC;
		typedef typename Static::Remove::Const<TypeC>::Type C;
		static void Perform(CStringT& s, const Yuni::Atomic::Int<SizeT, TP>& rhs)
		{
			s.append((typename Yuni::Atomic::Int<SizeT, TP>::ScalarType) rhs);
		}
	};


	template<int SizeT, template<class> class TP>
	class Into<Yuni::Atomic::Int<SizeT,TP> >
	{
	public:
		enum { valid = 1 };

		template<class StringT> static bool Perform(const StringT& s, Yuni::Atomic::Int<SizeT, TP>& out)
		{
			typedef typename Yuni::Atomic::Int<SizeT, TP>::ScalarType Scalar;
			Scalar tmp;
			if (s.template to<Scalar>(tmp))
			{
				out = tmp;
				return true;
			}
			return false;
		}

		template<class StringT>
		static typename Yuni::Atomic::Int<SizeT, TP>::ScalarType Perform(const StringT& s)
		{
			return s.template to<typename Yuni::Atomic::Int<SizeT, TP>::ScalarType>();
		}
	};




} // namespace CString
} // namespace Extension
} // namespace Yuni

#endif // __YUNI_CORE_ATOMIC_STRING_HXX__
