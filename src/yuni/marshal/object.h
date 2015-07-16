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
#include "../yuni.h"
#include "../core/string.h"



namespace Yuni
{
namespace Marshal
{

	/*!
	** \brief Base object for data serialization
	**
	** This class is a Variant-like structure, but less scalable and faster
	** for small data types.
	** Additionally, this class remains a POD type (no smart pointer or C++ advanced techniques)
	** for being used with malloc / calloc.
	*/
	class Object final
	{
	public:

		//! Internal type
		enum Type
		{
			// builtin types
			//! Nil object
			otNil = 0,
			//! String
			otString,
			//! Boolean
			otBool,
			//! Integer
			otInteger,
			//! Double,
			otDouble,
			// complex datatypes
			//! Array
			otArray,
			//! Dictionary
			otDictionary
		};

		// \internal for some reasons, the compiler requires to have this type public
		// (does not compile otherwise in object.cpp)
		union InternalDatatype
		{
			//! Value as integer
			sint64 integer;
			//! Value as bool
			bool boolean;
			//! Decimal
			double decimal;
			//! String
			String* string;
			//! Array
			void* array;
			//! Dictionary
			void* dictionary;
			//! blob
			void* blob;
		};


	public:
		//! \name Constructors & Destructor
		//@{
		Object();
		//! Copy constructor
		Object(const Object& rhs);
		#ifdef YUNI_HAS_CPP_MOVE
		//! Move constructor
		Object(Object&&);
		#endif
		//! Constructor for internal operations
		Object(Type type, InternalDatatype value);
		//! Destructor
		~Object();
		//@}

		//! \name Clean
		//@{
		//! Clear internal variable
		void clear();
		//@}


		//! \name Assign
		//@{
		//! Copy operator
		void assign(const Object& rhs);
		//! assign boolean
		void assign(bool boolean);
		//! assign double
		void assign(double decimal);
		//! assign int
		void assign(int integer);
		//! assign int64
		void assign(sint64 integer);
		//! assign string
		void assign(const AnyString& string);
		//! assign cstring
		template<uint N> void assign(const char string[N]);
		//! assign cstring
		void assign(const char* string);
		//@}


		//! \name Append
		//@{
		//! Append an object
		void append(const Object& rhs);

		//! see append()
		template<class T> void push_back(const T& value);
		//@}


		//! \name Informations about internal data
		//@{
		//! Get the internal type
		Type type() const;

		/*!
		** \brief Get the number of item
		**
		** \return The number of items if an array or a dictionary, 0 if nil, 1 otherwise
		*/
		size_t size() const;

		/*!
		** \brief Swap the content of two object
		*/
		void swap(Object& second);
		//@}


		//! \name Import & Export
		//@{
		/*!
		** \brief Dump the content into a JSON structure
		**
		** \param out Stream output
		** \param pretty True to export in a pretty format (with spaces and indentation)
		*/
		void toJSON(Clob& out, bool pretty = true) const;
		//@}


		//! \name Operators
		//@{
		//! Copy operator
		Object& operator = (const Object& rhs);
		#ifdef YUNI_HAS_CPP_MOVE
		//! Move opertor
		Object& operator = (Object&&);
		#endif
		//! assign string
		template<uint N> Object& operator = (const char string[N]);
		//! assign something else
		template<class T> Object& operator = (const T& value);
		//! append
		template<class T> Object& operator += (const T& value);
		//! read/write the value of a given key
		Object& operator [] (const String& key);
		//@}


	private:
		template<bool PrettyT, class StreamT>
		void valueToJSON(StreamT& out, uint depth) const;

	private:
		//! Internal data type
		Type pType;
		//! Internal value
		InternalDatatype pValue;

	}; // class Object





} // namespace Marshal
} // namespace Yuni

#include "object.hxx"
