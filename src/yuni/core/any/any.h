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
#ifndef __YUNI_CORE_ANY_ANY_H__
#define __YUNI_CORE_ANY_ANY_H__

# include "../../yuni.h"
# include "../exceptions/badcast.h"
# include "../string.h"
# include "any.private.h"



namespace Yuni
{

	/*!
	** \brief Container for values of any type.
	** \ingroup Any
	**
	** This is a relatively classic, yet (i hope) fast implementation of
	** an Any type.
	**
	** How to use:
	** \code
	** Any v(3.14);
	**
	** if (v.is<double>())
	**   // Be sure to check the type before, otherwise Any will throw an exception.
	**   std::cout << v.cast<double>();
	** \endcode
	*/
	class YUNI_DECL Any final
	{
	public:
		/*!
		** \brief Swaps an any with another
		** \param[in,out] one Any 1
		** \param[in,out] other Any 2
		*/
		static void Swap(Any& one, Any& other);


	public:
		//! \name Constructors
		//@{
		//! Copy of an existing variable
		template<class T> Any(const T& source);

		//! Copy of an existing const C string
		Any(const char* source)
		{ initFromCString(source); }

		//! Copy of an existing const C string
		Any(char* source)
		{ initFromCString(source); }

		//! Empty constructor
		Any();

		//! Copy constructor
		Any(const Any& rhs);

		//! Destructor
		~Any();
		//@}


		//! \name Alteration methods
		//@{

		/*!
		** \brief Assignment from another Any
		** \param[in] rhs The Any to assign from
		** \return This Any
		*/
		Any& assign(const Any& rhs);

		/*!
		** \brief Assignment from any object
		** \param[in] rhs The object to assign from
		** \return This Any
		*/
		template<class T>
		Any& assign(const T& rhs);

		/*!
		** \brief Specialized assign for C Strings.
		*/
		Any& assign(const char* rhs)
		{ return assign<String>(rhs); }

		/*!
		** \brief Assignment operator for convenience
		*/
		template<class T>
		Any& operator = (T const& rhs) {return assign(rhs);}

		/*!
		** \brief Resets the Any to an empty state.
		*/
		void reset();

		//@}

		//! \name Information methods
		//@{

		/*!
		** \brief Returns the type_info of the held variable.
		**
		** Can be used to compare with typeid(MyType).
		*/
		const std::type_info& type() const {return pTable->type();}


		/*!
		** \brief Returns true if the object is of the specified type
		*/
		template<class T>
		bool is() const {return type() == typeid(T);}


		/*!
		** \brief Checks if the any has been assigned a value.
		** \return True if the any contains no value.
		*/
		bool empty() const
		{ return pTable == Private::Any::Table<Private::Any::Empty>::Get(); }

		//@}

		//! \name Retrieval methods
		//@{

		/*!
		** \brief Casts the Any to the T type. Throws a Yuni::Exceptions::BadCast
		** if not possible.
		** \return A T object.
		*/
		template<class T>
		const T& to() const;

		//@}

	private:
		//! Special initializer for C Strings copys
		template<class T> void initFromCString(T source);

	private:
		//! Static function pointer table storage
		Private::Any::TypeManipulationTable * pTable;

		//! Object storage.
		void* pObject;
	};




} // namespace Yuni

# include "any.hxx"

#endif /* !__YUNI_CORE_ANY_ANY_H__ */


