#ifndef __YUNI_CORE_STATIC_HAS_TYPEDEF_H__
# define __YUNI_CORE_STATIC_HAS_TYPEDEF_H__

# include "../../yuni.h"
# include "remove.h"


/*!
** \brief Macro for implementing a check for Determining whether a class has a specific typedef or not
*/
# define YUNI_IMPL_STATIC_HAS_TYPEDEF(TYPEDEF)  \
		template<class T> \
		class TYPEDEF \
		{ \
			typedef struct {char a[2];}   TrueType; \
			typedef char  FalseType; \
			template<class C> static TrueType  CheckIfHasTypedef(typename C::TYPEDEF const *); \
			template<class>   static FalseType CheckIfHasTypedef(...); \
		\
		public: \
			enum \
			{ \
				yes = sizeof(CheckIfHasTypedef<typename ::Yuni::Static::Remove::All<T>::Type>(0)) == sizeof(TrueType), \
				no = !yes, \
			}; \
		}






namespace Yuni
{
namespace Static
{
namespace HasTypedef
{

	/*!
	** \brief Determine whether a class has the typedef `Ptr` or not
	**
	** \code
	** std::cout << Static::HasTypedef::Ptr<MyClass>::yes << std::endl;
	** \endcode
	*/
	YUNI_IMPL_STATIC_HAS_TYPEDEF(Ptr);


	/*!
	** \brief Determine whether a class has the typedef `IntrusiveSmartPtrType` or not
	**
	** This test is mainly used to prevent misuse of some incompatible smartptr.
	*/
	YUNI_IMPL_STATIC_HAS_TYPEDEF(IntrusiveSmartPtrType);



} // namespace HasMethod
} // namespace Static
} // namespace Yuni

#endif // __YUNI_CORE_STATIC_HAS_TYPEDEF_H__
