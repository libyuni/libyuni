#ifndef __YUNI_CORE_BIND_TRAITS_H__
# define __YUNI_CORE_BIND_TRAITS_H__

<%
require File.dirname(__FILE__) + '/../../../tools/generators/commons.rb'
generator = Generator.new()
%>
<%=generator.thisHeaderHasBeenGenerated("traits.h.generator.hpp")%>
#include "../event/interfaces.h"
#include "../static/inherit.h"
#include "../static/dynamiccast.h"


namespace Yuni
{

	// Forward declaration
	template<typename P, class Dummy> class Bind;


} // namespace Yuni

namespace Yuni
{
namespace Private
{
namespace BindImpl
{


	// Forward declarations for classes which will hold informations about the
	// targetted function or member

	/*!
	** \brief Interface
	**
	** \tparam P The prototype of the function/member
	*/
	template<class P> class IPointer;

	/*!
	** \brief Binding with a function
	**
	** \tparam P The prototype of the member
	*/
	template<class P> class BoundWithFunction;

	/*!
	** \brief Binding with a function
	**
	** \tparam U Type of the user data
	** \tparam P The prototype of the member
	*/
	template<class U, class P> class BoundWithFunctionAndUserData;

	/*!
	** \brief Binding with a member
	**
	** \tparam C Any class
	** \tparam P The prototype of the member
	*/
	template<class C, class P> class BoundWithMember;

	/*!
	** \brief Binding with a member and a user data
	**
	** \tparam U Type of the user data
	** \tparam C Any class
	** \tparam P The prototype of the member
	*/
	template<class U, class C, class P> class BoundWithMemberAndUserData;


	/*!
	** \brief Binding with a member
	**
	** \tparam C Any class
	** \tparam P The prototype of the member
	*/
	template<class C, class P> class BoundWithSmartPtrMember;

	/*!
	** \brief Binding with a member and a user data
	**
	** \tparam U Type of the user data
	** \tparam C Any class
	** \tparam P The prototype of the member
	*/
	template<class U, class C, class P> class BoundWithSmartPtrMemberAndUserData;





	/*!
	** \brief Helper to determine the type of an argument from its index
	**
	** \tparam P The prototype of a function/member
	** \tparam I The index of the argument (zero-based)
	*/
	template<class P, int I>
	struct Argument
	{
		// By default the argument does not exist, but we provide a valid type anyway
		typedef Yuni::None Type;
	};


	// There is no need for a partial specialization when the prototype does not
	// have any argument. `Yuni::None` will always be returned
<% (1..generator.argumentCount).each do |i| %>

	// Partial Specialization when the prototype has <%=generator.xArgumentsToStr(i)%>
<% (0..i-1).each do |j| %>	// Argument <%=j%>
	template<class R<%=generator.templateParameterList(i) %>>
	struct Argument<R(<%=generator.list(i)%>), <%=j%>> { typedef A<%=j%> Type; };
<% end end %>



	template<class T>
	struct Parameter
	{
		typedef const
			typename Static::Remove::Const<	typename Static::Remove::RefOnly<T>::Type>::Type
			& Type;
	};


	template<class T>
	struct Parameter<const T*>
	{
		typedef const T* Type;
	};

	template<class T>
	struct Parameter<T*>
	{
		typedef T* Type;
	};

	template<class T, int N>
	struct Parameter<const T[N]>
	{
		typedef const T* Type;
	};

	template<class T, int N>
	struct Parameter<T[N]>
	{
		typedef T* Type;
	};

	template<class T>
	struct Parameter<T&>
	{
		typedef const T& Type;
	};

	template<class T>
	struct Parameter<const T&>
	{
		typedef const T& Type;
	};



	template<class R, class B>
	struct Unbind
	{
		static void Execute(B* d)
		{
			d->bind(d, &B::emptyCallback);
		}
	};

	template<class B>
	struct Unbind<void,B>
	{
		static void Execute(B* d)
		{
			d->bind(d, &B::emptyCallbackReturnsVoid);
		}
	};





	// class IPointer
<% (0..generator.argumentCount-1).each do |i| %>
	template<class R<%=generator.templateParameterList(i) %>>
	class IPointer<R(<%=generator.list(i)%>)>
	{
	public:
		//! Destructor
		virtual ~IPointer() {}
		//! Invoke the delegate
		virtual R invoke(<%=generator.variableList(i)%>) const = 0;

		//! Get the pointer to object
		virtual const void* object() const = 0;

		//! Get the pointer to object cast into IEventObserverBase
		virtual const IEventObserverBase* observerBaseObject() const = 0;

		//! Get if the attached class is a descendant of 'IEventObserverBase'
		virtual bool isDescendantOf(const IEventObserverBase* obj) const = 0;

		//! Get if the attached class is a descendant of 'IEventObserverBase'
		virtual bool isDescendantOfIEventObserverBase() const = 0;

		//! Compare with a mere pointer-to-function
		virtual bool compareWithPointerToFunction(R (*pointer)(<%=generator.list(i)%>)) const = 0;
		//! Compare with a pointer-to-object
		virtual bool compareWithPointerToObject(const void* object) const = 0;

	};
<% end %>





	// class BoundWithFunction

<% (0..generator.argumentCount-1).each do |i| %>
	/*!
	** \brief
	*/
	template<class R<%=generator.templateParameterList(i) %>>
	class BoundWithFunction<R (<%=generator.list(i)%>)> final :
		public IPointer<R (<%=generator.list(i)%>)>
	{
	public:
		//! Destructor
		virtual ~BoundWithFunction() {}

		BoundWithFunction(R(*pointer)(<%=generator.list(i)%>)) :
			pPointer(pointer)
		{}

		virtual R invoke(<%=generator.variableList(i)%>) const override
		{
			return (*pPointer)(<%=generator.list(i, 'a')%>);
		}

		virtual const void* object() const override
		{
			return NULL;
		}

		virtual const IEventObserverBase* observerBaseObject() const override
		{
			return NULL;
		}

		virtual bool isDescendantOf(const IEventObserverBase*) const override
		{
			return false;
		}

		virtual bool isDescendantOfIEventObserverBase() const override
		{
			return false;
		}

		virtual bool compareWithPointerToFunction(R (*pointer)(<%=generator.list(i)%>)) const override
		{
			return ((void*)pPointer == (void*)pointer);
		}

		virtual bool compareWithPointerToObject(const void*) const override
		{
			return false;
		}


	private:
		//! Pointer-to-function
		R (*pPointer)(<%=generator.list(i)%>);

	}; // class BoundWithFunction<R (<%=generator.list(i)%>)>


<% end %>





	// class BoundWithFunctionAndUserData

<% (1..generator.argumentCount).each do |i| %>
	template<class U, class R<%=generator.templateParameterList(i) %>>
	class BoundWithFunctionAndUserData<U, R(<%=generator.list(i)%>)> final
		:public IPointer<R (<%=generator.list(i - 1)%>)>
	{
	public:
		BoundWithFunctionAndUserData(R(*pointer)(<%=generator.list(i)%>), U userdata) :
			pPointer(pointer), pUserdata(userdata)
		{}

		virtual R invoke(<%=generator.variableList(i-1)%>) const override
		{
			return (*pPointer)(<%=generator.list(i-1, 'a', "", ", ")%>*const_cast<UserDataTypeByCopy*>(&pUserdata));
		}

		virtual const void* object() const override
		{
			return NULL;
		}

		virtual const IEventObserverBase* observerBaseObject() const override
		{
			return NULL;
		}

		virtual bool isDescendantOf(const IEventObserverBase*) const override
		{
			return false;
		}

		virtual bool isDescendantOfIEventObserverBase() const override
		{
			return false;
		}

		virtual bool compareWithPointerToFunction(R (*pointer)(<%=generator.list(i-1)%>)) const override
		{
			return ((void*)pPointer == (void*)pointer);
		}

		virtual bool compareWithPointerToObject(const void*) const override
		{
			return false;
		}


	private:
		//! Pointer-to-function
		R (*pPointer)(<%=generator.list(i)%>);
		//! Storage type
		typedef typename Static::Remove::RefOnly<A<%=i-1%>>::Type UserDataTypeByCopy;
		//! The user data
		UserDataTypeByCopy pUserdata;

	}; // class BoundWithFunctionAndUserData<U, R(<%=generator.list(i)%>)>


<% end %>







	// class BoundWithMember

<% (0..generator.argumentCount-1).each do |i| %>
	template<class C, class R<%=generator.templateParameterList(i) %>>
	class BoundWithMember<C, R(<%=generator.list(i)%>)> final
		:public IPointer<R(<%=generator.list(i)%>)>
	{
	public:
		//! \name Constructor
		//@{
		//! Constructor
		BoundWithMember(C* c, R(C::*member)(<%=generator.list(i)%>)) :
			pThis(c),
			pMember(member)
		{}
		//@}

		virtual R invoke(<%=generator.variableList(i)%>) const override
		{
			return (pThis->*pMember)(<%=generator.list(i, 'a')%>);
		}

		virtual const void* object() const override
		{
			return reinterpret_cast<void*>(pThis);
		}

		virtual const IEventObserverBase* observerBaseObject() const override
		{
			return Static::DynamicCastWhenInherits<C,IEventObserverBase>::PerformConst(pThis);
		}

		virtual bool isDescendantOf(const IEventObserverBase* obj) const override
		{
			return Static::DynamicCastWhenInherits<C,IEventObserverBase>::Equals(obj, pThis);
		}

		virtual bool isDescendantOfIEventObserverBase() const override
		{
			return Static::DynamicCastWhenInherits<C,IEventObserverBase>::Yes;
		}

		virtual bool compareWithPointerToFunction(R (*)(<%=generator.list(i)%>)) const override
		{
			return false;
		}

		virtual bool compareWithPointerToObject(const void* object) const override
		{
			return (reinterpret_cast<const C*>(object) == pThis);
		}


	private:
		//! Pointer to the object
		mutable C* pThis;
		//! Pointer-to-member
		mutable R (C::*pMember)(<%=generator.list(i)%>);

	}; // class BoundWithMember<C, R(<%=generator.list(i)%>)>


<% end %>







	// class BoundWithMemberAndUserData

<% (1..generator.argumentCount).each do |i| %>
	template<class U, class C, class R<%=generator.templateParameterList(i) %>>
	class BoundWithMemberAndUserData<U, C, R(<%=generator.list(i)%>)> final
		:public IPointer<R(<%=generator.list(i-1)%>)>
	{
	public:
		typedef typename Static::Remove::RefOnly<A<%=i-1%>>::Type UserDataTypeByCopy;

	public:
		BoundWithMemberAndUserData(C* c, R(C::*member)(<%=generator.list(i)%>), U userdata) :
			pThis(c),
			pMember(member),
			pUserdata(userdata)
		{}

		virtual R invoke(<%=generator.variableList(i-1)%>) const override
		{
			return (pThis->*pMember)(<%=generator.list(i-1, 'a', "", ", ")%>const_cast<UserDataTypeByCopy&>(pUserdata));
		}

		virtual const void* object() const override
		{
			return reinterpret_cast<void*>(pThis);
		}

		virtual const IEventObserverBase* observerBaseObject() const override
		{
			return Static::DynamicCastWhenInherits<C,IEventObserverBase>::PerformConst(pThis);
		}

		virtual bool isDescendantOf(const IEventObserverBase* obj) const override
		{
			return Static::DynamicCastWhenInherits<C,IEventObserverBase>::Equals(obj, pThis);
		}

		virtual bool isDescendantOfIEventObserverBase() const override
		{
			return Static::DynamicCastWhenInherits<C,IEventObserverBase>::Yes;
		}


		virtual bool compareWithPointerToFunction(R (*)(<%=generator.list(i-1)%>)) const override
		{
			return false;
		}

		virtual bool compareWithPointerToObject(const void* object) const override
		{
			return (reinterpret_cast<const C*>(object) == pThis);
		}


	private:
		//! Pointer to the object
		mutable C* pThis;
		//! Pointer-to-member
		mutable R (C::*pMember)(<%=generator.list(i)%>);
		//! Userdata
		UserDataTypeByCopy pUserdata;

	}; // class BoundWithMemberAndUserData<U, C, R(<%=generator.list(i)%>)>


<% end %>


<% (0..generator.argumentCount-1).each do |i| %>
	template<class PtrT, class R<%=generator.templateParameterList(i) %>>
	class BoundWithSmartPtrMember<PtrT, R(<%=generator.list(i)%>)> final
		:public IPointer<R(<%=generator.list(i)%>)>
	{
	public:
		typedef typename PtrT::Type  C;

	public:
		//! \name Constructor
		//@{
		//! Constructor
		BoundWithSmartPtrMember(const PtrT& c, R(C::*member)(<%=generator.list(i)%>)) :
			pThis(c),
			pMember(member)
		{}
		//@}

		virtual R invoke(<%=generator.variableList(i)%>) const override
		{
			return ((pThis.pointer())->*pMember)(<%=generator.list(i, 'a')%>);
		}

		virtual const void* object() const override
		{
			return reinterpret_cast<void*>(pThis.pointer());
		}

		virtual const IEventObserverBase* observerBaseObject() const override
		{
			return Static::DynamicCastWhenInherits<C,IEventObserverBase>::PerformConst(pThis.pointer());
		}

		virtual bool isDescendantOf(const IEventObserverBase* obj) const override
		{
			return Static::DynamicCastWhenInherits<C,IEventObserverBase>::Equals(obj, pThis.pointer());
		}

		virtual bool isDescendantOfIEventObserverBase() const override
		{
			return Static::DynamicCastWhenInherits<C,IEventObserverBase>::Yes;
		}

		virtual bool compareWithPointerToFunction(R (*)(<%=generator.list(i)%>)) const override
		{
			return false;
		}

		virtual bool compareWithPointerToObject(const void* object) const override
		{
			return (reinterpret_cast<const C*>(object) == pThis.pointer());
		}


	private:
		//! Pointer to the object
		mutable PtrT pThis;
		//! Pointer-to-member
		mutable R (C::*pMember)(<%=generator.list(i)%>);

	}; // class BoundWithSmartPtrMember<C, R(<%=generator.list(i)%>)>


<% end %>







	// class BoundWithMemberAndUserData

<% (1..generator.argumentCount).each do |i| %>
	template<class U, class PtrT, class R<%=generator.templateParameterList(i) %>>
	class BoundWithSmartPtrMemberAndUserData<U, PtrT, R(<%=generator.list(i)%>)> final
		:public IPointer<R(<%=generator.list(i-1)%>)>
	{
	public:
		typedef typename PtrT::Type  C;
		typedef typename Static::Remove::RefOnly<A<%=i-1%>>::Type UserDataTypeByCopy;

	public:
		BoundWithSmartPtrMemberAndUserData(const PtrT& c, R(C::*member)(<%=generator.list(i)%>), U userdata) :
			pThis(c),
			pMember(member),
			pUserdata(userdata)
		{}

		virtual R invoke(<%=generator.variableList(i-1)%>) const override
		{
			return ((pThis.pointer())->*pMember)(<%=generator.list(i-1, 'a', "", ", ")%>const_cast<UserDataTypeByCopy&>(pUserdata));
		}

		virtual const void* object() const override
		{
			return reinterpret_cast<void*>(pThis.pointer());
		}

		virtual const IEventObserverBase* observerBaseObject() const override
		{
			return Static::DynamicCastWhenInherits<C,IEventObserverBase>::PerformConst(pThis.pointer());
		}

		virtual bool isDescendantOf(const IEventObserverBase* obj) const override
		{
			return Static::DynamicCastWhenInherits<C,IEventObserverBase>::Equals(obj, pThis.pointer());
		}

		virtual bool isDescendantOfIEventObserverBase() const override
		{
			return Static::DynamicCastWhenInherits<C,IEventObserverBase>::Yes;
		}


		virtual bool compareWithPointerToFunction(R (*)(<%=generator.list(i-1)%>)) const override
		{
			return false;
		}

		virtual bool compareWithPointerToObject(const void* object) const override
		{
			return (reinterpret_cast<const C*>(object) == pThis.pointer());
		}


	private:
		//! Pointer to the object
		mutable PtrT pThis;
		//! Pointer-to-member
		mutable R (C::*pMember)(<%=generator.list(i)%>);
		//! Userdata
		UserDataTypeByCopy pUserdata;

	}; // class BoundWithSmartPtrMemberAndUserData<U, C, R(<%=generator.list(i)%>)>


<% end %>





} // namespace BindImpl
} // namespace Private
} // namespace Yuni

#endif // __YUNI_CORE_BIND_TRAITS_H__
