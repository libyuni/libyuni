/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** gitlab: https://gitlab.com/libyuni/libyuni/
** github: https://github.com/libyuni/libyuni/ {mirror}
*/
#pragma once
<%
require File.dirname(__FILE__) + '/../../../tools/generators/commons.rb'
generator = Generator.new()
%>
<%=generator.thisHeaderHasBeenGenerated("traits.h.generator.hpp")%>

#include "../slist/slist.h"
#include "../atomic/bool.h"



namespace Yuni
{
namespace Private
{
namespace EventImpl
{

	template<int N, class BindT> class WithNArguments;


	template<typename T>
	class EmptyPredicate final
	{
	public:
		typedef void ResultType;
	public:
		inline void operator () (T) {}
		static void result() {}
	};


	template<class BindT>
	class PredicateRemoveObject final
	{
	public:
		PredicateRemoveObject(const void* object) :
			pObject(object)
		{}

		bool operator == (const BindT& rhs) const
		{
			return  (pObject == reinterpret_cast<const void*>(rhs.object()));
		}
	private:
		const void* pObject;
	};


	template<class BindT>
	class PredicateRemoveObserverBase final
	{
	public:
		PredicateRemoveObserverBase(IEvent* event, const IEventObserverBase* object) :
			pEvent(event), pObject(object)
		{}

		bool operator == (const BindT& rhs) const
		{
			if (pObject == rhs.observerBaseObject())
			{
				pObject->boundEventDecrementReference(pEvent);
				return true;
			}
			return false;
		}
	private:
		IEvent* pEvent;
		const IEventObserverBase* pObject;
	};


	template<class BindT>
	class PredicateRemoveWithoutChecks final
	{
	public:
		PredicateRemoveWithoutChecks(const IEventObserverBase* object) :
			pObject(object)
		{}

		inline bool operator == (const BindT& rhs) const
		{
			return (rhs.isDescendantOf(pObject));
		}
	private:
		const IEventObserverBase* pObject;
	};



	struct FoldTypeVoid {};
	template<class T> struct FoldType { typedef T value_type; };
	template<> struct FoldType<void> { typedef FoldTypeVoid value_type; };




<% (0..generator.argumentCount).each do |i| %>
	template<class BindT>
	class WithNArguments<<%=i%>, BindT> : public Policy::ObjectLevelLockable<WithNArguments<<%=i%>,BindT> >
	{
	public:
		//! The Threading Policy
		typedef Policy::ObjectLevelLockable<WithNArguments<<%=i%>,BindT> > ThreadingPolicy;
		//! Bind
		typedef BindT BindType;
		//! The Return type
		typedef typename BindType::ReturnType R;<% (0..i-1).each do |j| %>
		//! Type of the argument <%=j%>
		typedef typename BindType::template Argument<<%=j%>>::Type A<%=j%>;<% end %>


	public:
		//! \name Constructors
		//@{
		//! Default constructor
		WithNArguments() :
			pEmpty(true)
		{}
		//! Copy constructor
		WithNArguments(const WithNArguments& rhs)
		{
			typename ThreadingPolicy::MutexLocker locker(*this);
			pEmpty = rhs.pEmpty;
			pBindList = rhs.pBindList;
		}
		//@}

		//! \name Invoke
		//@{
		/*!
		** \brief Invoke the delegate
		*/
		void invoke(<%=generator.variableList(i)%>) const
		{
			if (not pEmpty)
			{
				typename ThreadingPolicy::MutexLocker locker(*this);
				const typename BindList::const_iterator end = pBindList.end();
				for (typename BindList::const_iterator i = pBindList.begin(); i != end; ++i)
					(*i).invoke(<%=generator.list(i,'a')%>);
			}
		}

		template<class CallbackT>
		R fold(typename FoldType<R>::value_type initval, const CallbackT& accumulator<%=generator.variableList(i, "A", "a", ", ")%>) const
		{
			if (not pEmpty)
			{
				typename FoldType<R>::value_type value = initval;
				typename ThreadingPolicy::MutexLocker locker(*this);
				const typename BindList::const_iterator end = pBindList.end();
				for (typename BindList::const_iterator i = pBindList.begin(); i != end; ++i)
					accumulator(value, (*i).invoke(<%=generator.list(i,'a')%>));
				return value;
			}
			return initval;
		}


		template<template<class> class PredicateT>
		typename PredicateT<R>::ResultType invoke(<%=generator.variableList(i)%>) const
		{
			PredicateT<R> predicate;
			if (not pEmpty)
			{
				typename ThreadingPolicy::MutexLocker locker(*this);
				const typename BindList::const_iterator end = pBindList.end();
				for (typename BindList::const_iterator i = pBindList.begin(); i != end; ++i)
					predicate((*i).invoke(<%=generator.list(i,'a')%>));
			}
			return predicate.result();
		}


		template<template<class> class PredicateT>
		typename PredicateT<R>::ResultType invoke(PredicateT<R>& predicate<%=generator.variableList(i, "A", "a", ", ")%>) const
		{
			if (not pEmpty)
			{
				typename ThreadingPolicy::MutexLocker locker(*this);
				const typename BindList::const_iterator end = pBindList.end();
				for (typename BindList::const_iterator i = pBindList.begin(); i != end; ++i)
					predicate((*i).invoke(<%=generator.list(i,'a')%>));
			}
			return predicate.result();
		}

		template<class EventT> void assign(EventT& rhs)
		{
			typename ThreadingPolicy::MutexLocker locker(*this);
			typename ThreadingPolicy::MutexLocker lockerRHS(rhs);
			pBindList = rhs.pBindList;
			pEmpty = pBindList.empty();
		}

		/*!
		** \brief Invoke the delegate
		*/
		void operator () (<%=generator.variableList(i)%>) const
		{
			if (not pEmpty)
			{
				typename ThreadingPolicy::MutexLocker locker(*this);
				const typename BindList::const_iterator end = pBindList.end();
				for (typename BindList::const_iterator i = pBindList.begin(); i != end; ++i)
					(*i).invoke(<%=generator.list(i,'a')%>);
			}
		}
		//@}


	protected:
		//! Binding list (type)
		typedef LinkedList<BindType> BindList;
		//! A flag to know if the event is empty or not
		// This value must only set when the mutex is locked
		volatile bool pEmpty;
		//! Binding list
		BindList pBindList;
		// friend !
		template<class P> friend class Event;

	}; // class WithNArguments


<% end %>




} // namespace EventImpl
} // namespace Private
} // namespace Yuni
