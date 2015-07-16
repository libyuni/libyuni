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
#ifndef __YUNI_CORE_SLIST_SLIST_H__
# define __YUNI_CORE_SLIST_SLIST_H__

# include "../../yuni.h"
# include <list>
# include <vector>
# include "../static/remove.h"
# include "../static/types.h"
# include "../iterator.h"
# include "../noncopyable.h"


namespace Yuni
{

	/*!
	** \brief A singly linked list
	*/
	template<class T, class Alloc = None>
	class YUNI_DECL LinkedList final
	{
	public:
		//! Size type
		typedef uint Size;
		//! Size type (STL compliant)
		typedef Size size_type;

		//! Value type
		typedef T Type;
		//! Value type
		typedef T ValueType;
		//! Value type (STL compliant)
		typedef T value_type;
		//! Reference
		typedef T& reference_type;
		//! Reference (const)
		typedef const T& const_reference_type;
		//! Pointer
		typedef T* pointer_type;
		//! Pointer (const)
		typedef const T* const_pointer_type;

		// Forward declaration for iterators
		class Iterator;
		//! Iterator (STL compliant)
		typedef IIterator<Iterator, false> iterator;
		typedef IIterator<Iterator, true> const_iterator;

	protected:
		// Forward declaration
		class Item;

		enum
		{
			/*!
			** \brief Indicates whether to base the comparison between values on a dereferenced pointer
			**
			** \internal This check is used when the type T is a reference
			*/
			comparePointer = Static::Type::IsConst<T>::No && Static::Type::Compound<T>::IsReference,
		};

	public:
		# include "iterator.inc.hpp"

	public:
		//! \name Constructors & Destructor
		//@{
		//! Default Constructor
		LinkedList();
		//! Copy constructor
		LinkedList(const LinkedList& rhs);
		# ifdef YUNI_HAS_CPP_MOVE
		//! Move constructor
		LinkedList(LinkedList&& rhs);
		# endif
		//! Copy constructor from another linked list
		template<class U, class A> LinkedList(const LinkedList<U,A>& rhs);
		//! Copy constructor from a standard std::list
		template<class U, class A> explicit LinkedList(const std::list<U,A>& rhs);
		//! Copy constructor from a standard std::vector
		template<class U, class A> explicit LinkedList(const std::vector<U,A>& rhs);
		//! Destructor
		~LinkedList();
		//@}


		//! \name Iterators & Accessors
		//@{
		/*!
		** \brief Get an iterator at the beginning of the list
		*/
		iterator begin();
		const_iterator begin() const;

		/*!
		** \brief Get an iterator at the end of the list
		*/
		iterator end();
		const_iterator end() const;

		//! Get the first item
		reference_type front();
		const_reference_type front() const;

		//! Get the last item
		reference_type back();
		const_reference_type back() const;
		//@}


		//! \name Find
		//@{
		/*!
		** \brief Get the first item equal to a given value
		**
		** \param value The value to look for
		** \return An iterator to the item found
		*/
		template<class U> iterator find(const U& value);
		iterator find(const_reference_type value);

		/*!
		** \brief Get the first item equal to a given value
		**
		** \param value The value to look for
		** \return An iterator to the item found
		*/
		template<class U> const_iterator find(const U& value) const;
		const_iterator find(const_reference_type value) const;
		//@}


		//! \name Add
		//@{
		/*!
		** \brief Add a value at the end of the list (default constructor)
		*/
		void push_back();
		/*!
		** \brief Add a value at the end of the list
		*/
		template<class U> void push_back(const U& value);
		void push_back(reference_type value);
		/*!
		** \brief Add a list of values at the end of the list
		*/
		template<class U, class A> void push_back(const LinkedList<U,A>& rhs);
		/*!
		** \brief Add a list of values at the end of the list
		*/
		template<class U, class A> void push_back(const std::list<U,A>& rhs);
		/*!
		** \brief Add a list of values at the end of the list
		*/
		template<class U, class A> void push_back(const std::vector<U,A>& rhs);

		/*!
		** \brief Add a value at the beginning of the list (default constructor)
		*/
		void push_front();
		/*!
		** \brief Add a value at the begining of the list
		*/
		template<class U> void push_front(const U& value);
		void push_front(reference_type value);

		/*!
		** \brief Add a list of values at the beginning of the list
		*/
		template<class U, class A> void push_front(const LinkedList<U,A>& rhs);
		/*!
		** \brief Add a list of values at the beginning of the list
		*/
		template<class U, class A> void push_front(const std::list<U,A>& rhs);
		/*!
		** \brief Add a list of values at the beginning of the list
		*/
		template<class U, class A> void push_front(const std::vector<U,A>& rhs);

		/*!
		** \brief Remove the first element
		*/
		void pop_front();
		//@}


		//! \name Remove
		//@{
		/*!
		** \brief Remove the item pointed to by the iterator
		*/
		void erase(iterator& i);

		/*!
		** \brief Remove any item equal to a given value
		**
		** \code
		** Yuni::LinkedList<int> list;
		** list.push_back(10);
		** list.push_back(42);
		** list.remove(42);
		** \endcode
		**
		** \code
		** struct PredicateRemove
		** {
		**		bool operator == (const int& rhs) const
		**		{
		**			return !(rhs % 2);
		**		}
		** };
		**
		** int main(void)
		** {
		**	Yuni::LinkedList<int>  list;
		**	list << 10 << 11 << 2 << 42;
		**	list.remove(PredicateRemove());
		**	return 0;
		** }
		** \endcode
		**
		** \param value The value to look for
		** \return The count of deleted items
		*/
		template<class U> Size remove(const U& value);
		Size remove(reference_type value);

		/*!
		** \brief Remove any item equal to a value in the given list
		**
		** \param rhs A list of values to remove
		** \return The number of items that have been removed
		*/
		template<class U, class A> Size remove(const LinkedList<U,A>& rhs);

		/*!
		** \brief Remove any item equal to a value in the given list
		**
		** \param rhs A list of values to remove
		** \return The number of items that have been removed
		*/
		template<class U, class A> Size remove(const std::list<U,A>& rhs);

		/*!
		** \brief Remove any item equal to a value in the given list
		**
		** \param rhs A list of values to remove
		** \return The number of items that have been removed
		*/
		template<class U, class A> Size remove(const std::vector<U,A>& rhs);

		/*!
		** \brief Remove all items at once
		*/
		void clear();
		//@}


		//! \name Memory
		//@{
		/*!
		** \brief Get if the container is empty
		** \return True if the container is empty, false otherwise
		*/
		bool empty() const;

		//! Get the number of items in the container
		Size size() const;
		//@}


		//! \name Operators
		//@{
		/*!
		** \brief Clear then add the value
		**
		** \param value The value to copy
		** \return Always *this
		*/
		LinkedList& operator = (const LinkedList& value);

		# ifdef YUNI_HAS_CPP_MOVE
		//! Move assignement
		LinkedList& operator = (LinkedList&& rhs);
		# endif

		/*!
		** \brief Append a new item at the end of the list
		**
		** \param value The value to copy and add
		** \return Always *this
		*/
		template<class U> LinkedList& operator += (const U& value);

		/*!
		** \brief Append a new item at the end of the list
		**
		** \param value The value to copy and add
		** \return Always *this
		*/
		template<class U> LinkedList& operator << (const U& value);

		/*!
		** \brief Remove all items equal to 'value'
		**
		** \param value The value to look for
		** \return Always *this
		*/
		template<class U> LinkedList& operator -= (const U& value);

		/*!
		** \brief Check if the container is empty
		**
		** This operator is strictly equivalent to the method 'empty()'
		** \return True if the container is empty, false otherwise
		*/
		bool operator ! () const;
		//@}


	protected:
		class Item final : private NonCopyable<Item>
		{
		public:
			Item() :
				next(nullptr)
			{}
			inline explicit Item(Item* n) :
				next(n), data()
			{}
			inline explicit Item(value_type& value) :
				next(nullptr), data(value)
			{}
			template<class U> inline explicit Item(const U& value) :
				next(nullptr), data(value)
			{}
			inline Item(Item* n, reference_type value) :
				next(n), data(value)
			{}
			template<class U> inline Item(Item* n, const U& value) :
				next(n), data(value)
			{}

		public:
			//! The next item in the list
			Item* next;
			//! The real value
			T data;

		}; // class Item

	protected:
		//! Internal clear (without resetting values)
		void doClear();

	protected:
		//! The head of the list
		Item* pHead;
		//! The last item in the list
		Item* pLast;
		//! Item count
		uint pCount;

	}; // class LinkedList





} // namespace Yuni

# include "slist.hxx"

#endif // __YUNI_CORE_SLIST_SLIST_H__
