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
#include "bind.h"


namespace Yuni
{

	template<class R, class... A>
	inline Bind<R (A...)>::Bind()
		: pHolder(new Private::BindImpl::None<R (A...)>()) // unbind
	{}


	template<class R, class... A>
	inline Bind<R (A...)>::Bind(const Bind<R (A...)>& rhs) :
		pHolder(rhs.pHolder)
	{}


	template<class R, class... A>
	inline Bind<R (A...)>::Bind(Bind&& rhs)
	{
		pHolder.swap(rhs.pHolder);
	}


	template<class R, class... A>
	inline Bind<R (A...)> Bind<R (A...)>::FromSymbol(const Yuni::DynamicLibrary::Symbol& symbol)
	{
		BindType result;
		result.bindFromSymbol(symbol);
		return result;
	}


	template<class R, class... A>
	template<class C>
	inline Bind<R (A...)>::Bind(C&& functor)
	{
		// When moving a bind (instead of copying it), it seems that Visual Studio might call
		// this constructor instead of `Bind&&` (works as expected with gcc and clang)
		if (Static::Type::Equal<C, BindType>::Yes) // moving Bind&& -> Bind&&
		{
			// instanciating the swap method only when the type requires it
			Yuni::Private::BindImpl::MoveConstructor<Static::Type::Equal<C, BindType>::Yes>::SwapBind(pHolder, functor);
		}
		else
			pHolder = new Private::BindImpl::BoundWithFunctor<C, R (A...)>(std::forward<C>(functor));
	}


	template<class R, class... A>
	template<class C>
	inline Bind<R (A...)>::Bind(C* c, R (C::*member)(A...))
	{
		bind(c, member);
	}


	// Bind: Pointer-to-function
	template<class R, class... A>
	inline void Bind<R (A...)>::bind(R (*pointer)(A...))
	{
		pHolder = new Private::BindImpl::BoundWithFunction<R (A...)>(pointer);
	}


	// Bind: Pointer-to-function (from a library symbol)
	template<class R, class... A>
	inline void Bind<R (A...)>::bindFromSymbol(const Yuni::DynamicLibrary::Symbol& symbol)
	{
		if (symbol.valid())
		{
			pHolder = new Private::BindImpl::BoundWithFunction<R (A...)>(
				reinterpret_cast<FunctionType>(symbol.ptr()));
		}
		else
			unbind();
	}

	// Bind: Pointer-to-function + user data
	template<class R, class... A>
	template<class U>
	inline void Bind<R (A...)>::bind(R (*pointer)(A..., U),
		typename Bind<R (A...)>::template WithUserData<U>::ParameterType userdata)
	{
		pHolder = new Private::BindImpl::BoundWithFunctionAndUserData
			<typename WithUserData<U>::ParameterType, R (A..., U)>(pointer, userdata);
	}


	template<class R, class... A>
	template<class C>
	inline void Bind<R (A...)>::bind(C&& functor)
	{
		// When moving a bind (instead of copying it), it seems that Visual Studio might call
		// this constructor instead of `Bind&&` (works as expected with gcc and clang)
		if (Static::Type::Equal<C, BindType>::Yes) // moving Bind&& -> Bind&&
		{
			// instanciating the swap method only when the type requires it
			Yuni::Private::BindImpl::MoveConstructor<Static::Type::Equal<C, BindType>::Yes>::SwapBind(pHolder, functor);
		}
		else
			pHolder = new Private::BindImpl::BoundWithFunctor<C, R (A...)>(std::forward<C>(functor));
	}


	template<class R, class... A>
	template<class C>
	void Bind<R (A...)>::bind(C* c, R (C::*member)(A...))
	{
		if (c)
			pHolder = new Private::BindImpl::BoundWithMember<C, R(A...)>(c, member);
		else
			unbind();
	}


	template<class R, class... A>
	template<class C>
	void Bind<R (A...)>::bind(const C* c, R (C::*member)(A...) const)
	{
		if (c)
		{
			typedef R (C::*MemberType)(A...);
			pHolder = new Private::BindImpl::BoundWithMember<C, R(A...)>
				(const_cast<C*>(c), reinterpret_cast<MemberType>(member));
		}
		else
			unbind();
	}


	template<class R, class... A>
	template<class C,
		template <class> class OwspP, template <class> class ChckP, class ConvP,
		template <class> class StorP, template <class> class ConsP>
	inline void
	Bind<R (A...)>::bind(const SmartPtr<C, OwspP,ChckP,ConvP,StorP,ConsP>& c, R (C::*member)(A...))
	{
		if (!!c)
		{
			typedef R (C::*MemberType)(A...);
			typedef SmartPtr<C, OwspP,ChckP,ConvP,StorP,ConsP> PtrT;
			pHolder = new Private::BindImpl::BoundWithSmartPtrMember<PtrT, R(A...)>
				(c, reinterpret_cast<MemberType>(member));
		}
		else
			unbind();
	}


	template<class R, class... A>
	template<class C,
		template <class> class OwspP, template <class> class ChckP, class ConvP,
		template <class> class StorP, template <class> class ConsP>
	inline void
	Bind<R (A...)>::bind(const SmartPtr<C, OwspP,ChckP,ConvP,StorP,ConsP>& c, R (C::*member)(A...) const)
	{
		if (!!c)
		{
			typedef R (C::*MemberType)(A...);
			typedef SmartPtr<C, OwspP,ChckP,ConvP,StorP,ConsP> PtrT;
			pHolder = new Private::BindImpl::BoundWithSmartPtrMember<PtrT, R(A...)>
				(c, reinterpret_cast<MemberType>(member));
		}
		else
			unbind();
	}


	template<class R, class... A>
	template<class U, class C,
		template <class> class OwspP, template <class> class ChckP, class ConvP,
		template <class> class StorP, template <class> class ConsP>
	inline void
	Bind<R (A...)>::bind(const SmartPtr<C, OwspP,ChckP,ConvP,StorP,ConsP>& c, R (C::*member)(A..., U),
		typename Bind<R (A...)>::template WithUserData<U>::ParameterType userdata)
	{
		if (!!c)
		{
			typedef SmartPtr<C, OwspP,ChckP,ConvP,StorP,ConsP> PtrT;
			pHolder = new Private::BindImpl::BoundWithSmartPtrMemberAndUserData<
				typename WithUserData<U>::ParameterType, PtrT, R(A..., U)>
				(c, member, userdata);
		}
		else
			unbind();
	}


	template<class R, class... A>
	template<class U, class C,
		template <class> class OwspP, template <class> class ChckP, class ConvP,
		template <class> class StorP, template <class> class ConsP>
	inline void
	Bind<R (A...)>::bind(const SmartPtr<C, OwspP,ChckP,ConvP,StorP,ConsP>& c, R (C::*member)(A..., U) const,
		typename Bind<R (A...)>::template WithUserData<U>::ParameterType userdata)
	{
		if (!!c)
		{
			typedef SmartPtr<C, OwspP,ChckP,ConvP,StorP,ConsP> PtrT;
			pHolder = new Private::BindImpl::BoundWithSmartPtrMemberAndUserData<
				typename WithUserData<U>::ParameterType, PtrT, R(A..., U)>
				(c, member, userdata);
		}
		else
			unbind();
	}


	template<class R, class... A>
	template<class U, class C>
	void Bind<R (A...)>::bind(C* c, R (C::*member)(A..., U),
		typename Bind<R (A...)>::template WithUserData<U>::ParameterType userdata)
	{
		if (c)
		{
			pHolder = new Private::BindImpl::BoundWithMemberAndUserData<
				typename WithUserData<U>::ParameterType,C, R(A..., U)>
				(c, member, userdata);
		}
		else
			unbind();
	}


	template<class R, class... A>
	template<class U, class C>
	void Bind<R (A...)>::bind(const C* c, R (C::*member)(A..., U) const,
		typename Bind<R (A...)>::template WithUserData<U>::ParameterType userdata)
	{
		if (c)
		{
			typedef R (C::*MemberType)(A..., U);
			pHolder = new Private::BindImpl::BoundWithMemberAndUserData<
				typename WithUserData<U>::ParameterType,C, R(A..., U)>
				(const_cast<C*>(c), reinterpret_cast<MemberType>(member), userdata);
		}
		else
			unbind();
	}


	template<class R, class... A>
	inline void Bind<R (A...)>::bind(const Bind<R (A...)>& rhs)
	{
		pHolder = rhs.pHolder;
	}


	// Bind: unbind
	template<class R, class... A>
	inline void Bind<R (A...)>::unbind()
	{
		pHolder = new Private::BindImpl::None<R (A...)>(); // unbind
	}


	// Bind: unbind
	template<class R, class... A>
	inline void Bind<R (A...)>::clear()
	{
		unbind();
	}


	template<class R, class... A>
	inline bool Bind<R (A...)>::empty() const
	{
		return pHolder->empty();
	}


	template<class R, class... A>
	template<class UserTypeT, template<class UserTypeGT, class ArgumentIndexTypeT> class ArgGetterT>
	inline R
	Bind<R (A...)>::callWithArgumentGetter(UserTypeT userdata) const
	{
		return invoke(ArgGetterT<UserTypeT, A...>::Get(userdata, 0));
	}


	template<class R, class... A>
	inline R Bind<R (A...)>::invoke(A... parameters) const
	{
		return pHolder->invoke(parameters...);
	}


	template<class R, class... A>
	inline bool Bind<R (A...)>::operator ! () const
	{
		return empty();
	}


	template<class R, class... A>
	inline R Bind<R (A...)>::operator () (A... parameters) const
	{
		return pHolder->invoke(parameters...);
	}


	template<class R, class... A>
	inline R Bind<R (A...)>::emptyCallback(A...)
	{
		return R();
	}


	template<class R, class... A>
	inline const void* Bind<R (A...)>::object() const
	{
		return pHolder->object();
	}


	template<class R, class... A>
	inline const IEventObserverBase* Bind<R (A...)>::observerBaseObject() const
	{
		return pHolder->observerBaseObject();
	}


	template<class R, class... A>
	inline bool Bind<R (A...)>::isDescendantOf(const IEventObserverBase* obj) const
	{
		return pHolder->isDescendantOf(obj);
	}


	template<class R, class... A>
	inline bool Bind<R (A...)>::isDescendantOfIEventObserverBase() const
	{
		return pHolder->isDescendantOfIEventObserverBase();
	}


	template<class R, class... A>
	inline void Bind<R (A...)>::print(std::ostream& out) const
	{
		return pHolder->print(out);
	}



	template<class R, class... A>
	inline void Bind<R (A...)>::emptyCallbackReturnsVoid(A...)
	{
		// Ddo nothing
	}


	template<class R, class... A>
	inline Bind<R (A...)>& Bind<R (A...)>::operator = (R (*pointer)(A...))
	{
		bind(pointer);
		return *this;
	}


	template<class R, class... A>
	inline Bind<R (A...)>& Bind<R (A...)>::operator = (const Yuni::DynamicLibrary::Symbol& symbol)
	{
		bind(symbol);
		return *this;
	}


	template<class R, class... A>
	inline Bind<R (A...)>& Bind<R (A...)>::operator = (const Bind<R (A...)>& rhs)
	{
		// Inc the reference count
		pHolder = rhs.pHolder;
		return *this;
	}


	template<class R, class... A>
	template<class C>
	inline Bind<R (A...)>& Bind<R (A...)>::operator = (C&& functor)
	{
		// Inc the reference count
		pHolder = new Private::BindImpl::BoundWithFunctor<C, R (A...)>(std::forward<C>(functor));
		return *this;
	}


	template<class R, class... A>
	inline Bind<R (A...)>& Bind<R (A...)>::operator = (Bind<R (A...)>&& rhs)
	{
		pHolder.swap(rhs.pHolder);
		return *this;
	}


	template<class R, class... A>
	inline bool Bind<R (A...)>::operator == (R (*pointer)(A...)) const
	{
		return pHolder->compareWithPointerToFunction(pointer);
	}


	template<class R, class... A>
	template<class U>
	inline bool Bind<R (A...)>::operator == (const U* object) const
	{
		return pHolder->compareWithPointerToObject(static_cast<const void*>(object));
	}


} // namespace Yuni
