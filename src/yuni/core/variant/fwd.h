#ifndef __YUNI_CORE_VARIANT_FWD_H__
# define __YUNI_CORE_VARIANT_FWD_H__


namespace Yuni
{
	class Variant;

namespace Private
{
namespace Variant
{
	class IDataConverter;
	class IDataHolder;

	typedef Yuni::SmartPtr<IDataHolder, Yuni::Policy::Ownership::COMReferenceCounted>  IDataHolderPtr;



} // namespace Variant
} // namespace Private
} // namespace Yuni

#endif // __YUNI_CORE_VARIANT_FWD_H__
