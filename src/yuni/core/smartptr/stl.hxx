#ifndef __YUNI_CORE_SMARTPTR_STL_HXX__
# define __YUNI_CORE_SMARTPTR_STL_HXX__



// Specialization for std::less
namespace std
{

	template< typename T,
		template <class> class OwspP, template <class> class ChckP, class ConvP,
		template <class> class StorP, template <class> class ConsP>
	struct YUNI_DECL less<Yuni::SmartPtr<T, OwspP,ChckP,ConvP,StorP,ConsP> > final
	{
		//! SmartPtr
		typedef Yuni::SmartPtr<T, OwspP,ChckP,ConvP,StorP,ConsP>  SmartPtrType;

		// C++11 compatibility
		typedef SmartPtrType first_argument_type;
		typedef SmartPtrType second_argument_type;
		typedef bool result_type;


		bool operator()(const SmartPtrType& lhs, const SmartPtrType& rhs) const
		{
			return lhs < rhs;
		}
	};



	# ifdef YUNI_HAS_STL_HASH

	template< typename T,
		template <class> class OwspP, template <class> class ChckP, class ConvP,
		template <class> class StorP, template <class> class ConsP>
	struct YUNI_DECL hash<Yuni::SmartPtr<T, OwspP,ChckP,ConvP,StorP,ConsP> > final
	{
		//! SmartPtr
		typedef Yuni::SmartPtr<T, OwspP,ChckP,ConvP,StorP,ConsP>  SmartPtrType;

		inline size_t operator() (const SmartPtrType& ptr) const
		{
			return std::hash<void*>()(SmartPtrType::WeakPointer(ptr));
		}
	};

	# endif // YUNI_HAS_STL_HASH



} // namespace std

#endif // __YUNI_CORE_SMARTPTR_STL_HXX__
