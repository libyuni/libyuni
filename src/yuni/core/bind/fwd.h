#ifndef __YUNI_CORE_BIND_FWD_H__
# define __YUNI_CORE_BIND_FWD_H__



namespace Yuni
{
namespace Private
{
namespace BindImpl
{

	template<bool> struct MoveConstructor final
	{
		template<class A, class B> static void SwapBind(A& self, B& rhs)
		{
			// instanciating the swap method only when the type requires it
			self.swap(rhs.pHolder);
		}
	};

	template<> struct MoveConstructor<false> final
	{
		template<class A, class B> static void SwapBind(A&, B&)
		{
			// empty on purpose
		}
	};







} // namespace BindImpl
} // namespace Private
} // anonymous namespace

#endif // __YUNI_CORE_BIND_FWD_H__
