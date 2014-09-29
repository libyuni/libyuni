#ifndef __YUNI_CORE_FUNCTIONAL_MAPPING_HXX__
# define __YUNI_CORE_FUNCTIONAL_MAPPING_HXX__

# include "../../yuni.h"


namespace Yuni
{
namespace Functional
{

namespace FilteringPolicy
{


	template<class T, class FuncT = None>
	class AcceptAll
	{
	public:
		AcceptAll(const FuncT&) {}

		bool accept(const T&) { return true; }
	};


	template<class T, class FuncT>
	class Function
	{
	public:
		Function(const FuncT& func): pFilter(func) {}

		bool accept(const T& elt) { return pFilter(elt); }

	private:
		const FuncT& pFilter;
	};



} // namespace FilteringPolicy




namespace MappingPolicy
{


	template<class SrcT, class DstT = SrcT, class FuncT = None>
	class Identity
	{
	public:
		Identity(const FuncT&) {}

		DstT map(const SrcT& src) { return src; }
	};


	template<class SrcT, class DstT, class FuncT>
	class Function
	{
	public:
		Function(const FuncT& func): pMapper(func) {}

		DstT map(const SrcT& src) { return pMapper(src); }

	private:
		const FuncT& pMapper;
	};



} // namespace MappingPolicy

} // namespace Functional
} // namespace Yuni

#endif // __YUNI_CORE_FUNCTIONAL_MAPPING_HXX__
