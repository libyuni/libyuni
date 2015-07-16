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
/*!
** \file
** \brief Storage policies
*/


namespace Yuni
{
namespace Policy
{

/*!
** \brief Storage policies
** \ingroup Policies
*/
namespace Storage
{

	/*!
	** \brief Implementation of the default storage policy
	** \ingroup Policies
	**
	** \tparam T The data type
	*/
	template<class T>
	class Pointer
	{
	public:
		//! The type stored
		typedef T* StoredType;
		//! The type used by the operator ->
		typedef T* PointerType;
		//! The type used by the operator *
		typedef T& ReferenceType;

		//! The default value for this type
		static StoredType DefaultValue() {return nullptr;}

	public:
		//! \name Constructors
		//@{
		Pointer() :pData(DefaultValue()) {}
		Pointer(const Pointer&) :pData(0) {}
		template<class U> Pointer(const Pointer<U>&) :pData(0) {}
		Pointer(const StoredType& p) :pData(p) {}
		//@}

		//! Swap the data
		void swapPointer(Pointer& rhs) {std::swap(pData, rhs.pData);}

		//! \name Get the data
		//@{
		friend PointerType storagePointer(const Pointer& rhs) {return rhs.pData;}
		friend StoredType& storageReference(Pointer& rhs) {return rhs.pData;}
		friend const StoredType& storageReference(const Pointer& rhs) {return rhs.pData;}
		//@}

		//! \name Operators
		//@{
		//! The operator ->
		PointerType   operator -> () const {return pData;}
		//! The operator *
		ReferenceType operator * () const {return *pData;}
		//@}

	protected:
		//! Destroy the inner data
		inline void destroy()
		{
			// [from the standard]
			// If the object being deleted has incomplete class type at the point of deletion
			// and the complete class has a non-trivial destructor or a deallocation function,
			// the behavior is undefined.
			if (0 < sizeof (T)) // won't compile for incomplete type
				delete pData;
		}

	private:
		//! The data
		StoredType pData;

	}; // class Pointer





	/*!
	** \brief Implementation of the Array storage policy
	** \ingroup Policies
	**
	** \tparam T The data type
	*/
	template<class T>
	class Array
	{
	public:
		//! The type stored
		typedef T* StoredType;
		//! The type used by the operator ->
		typedef T* PointerType;
		//! The type used by the operator *
		typedef T& ReferenceType;

		//! The default value for this type
		static StoredType DefaultValue() {return nullptr;}

	public:
		//! \name Constructors
		//@{
		Array() :pData(DefaultValue()) {}
		Array(const Array&) :pData(0) {}
		template<class U> Array(const Pointer<U>&) :pData(0) {}
		Array(const StoredType& p) :pData(p) {}
		//@}

		//! Swap the data
		void swapPointer(Array& rhs) {std::swap(pData, rhs.pData);}

		//! \name Get the data
		//@{
		friend PointerType storagePointer(const Array& rhs) {return rhs.pData;}
		friend StoredType& storageReference(Array& rhs) {return rhs.pData;}
		friend const StoredType& storageReference(const Array& rhs) {return rhs.pData;}
		//@}

		//! \name Operators
		//@{
		//! The operator ->
		PointerType   operator -> () const {return pData;}
		//! The operator *
		ReferenceType operator * () const {return *pData;}
		//@}

	protected:
		//! Destroy the inner data
		inline void destroy()
		{
			// [from the standard]
			// If the object being deleted has incomplete class type at the point of deletion
			// and the complete class has a non-trivial destructor or a deallocation function,
			// the behavior is undefined.
			if (0 < sizeof (T)) // won't compile for incomplete type
				delete[] pData;
		}

	private:
		//! The data
		StoredType pData;

	}; // class Array





} // namespace Storage
} // namespace Policy
} // namespace Yuni

