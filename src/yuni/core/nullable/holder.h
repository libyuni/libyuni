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



namespace Yuni
{

	template<typename T, class Alloc> class YUNI_DECL Nullable;


namespace Private
{
namespace NullableImpl
{

	template<class T>
	struct IsNullable final
	{
		enum { Yes = 0, No = 1 };
	};

	template<class U, class Alloc>
	struct IsNullable< Nullable<U, Alloc> > final
	{
		enum { Yes = 1, No = 0 };
	};





	// Forward declaration
	template<class T, int S>
	class YUNI_DECL Holder final
	{
	public:
		Holder() :
			pHasData(false)
		{}
		Holder(const Holder& rhs) :
			pData(rhs.pData), pHasData(rhs.pHasData)
		{}
		template<typename U> Holder(const U& rhs) :
			pData(rhs), pHasData(true)
		{}

		void clear() {pHasData = false;}

		bool empty() const {return !pHasData;}

		T& reference()
		{
			if (!pHasData)
			{
				pHasData = true;
				pData = T();
			}
			return pData;
		}

		const T& data() const {return pData;}
		T& data() {return pData;}

		template<class U>
		void assign(const U& v)
		{
			pData = v;
			pHasData = true;
		}

		void assign(const NullPtr&)
		{
			pHasData = false;
		}

		void assign(const Holder& rhs)
		{
			pHasData = rhs.pHasData;
			pData = rhs.pData;
		}

	private:
		//! The data
		T pData;
		//
		bool pHasData;

	};


	template<class T>
	class YUNI_DECL Holder<T,0> final
	{
	public:
		Holder()
		{}
		Holder(const Holder& rhs) :
			pData(rhs.pData)
		{}
		template<typename U> Holder(const U& rhs) :
			pData(new T(rhs))
		{}

		void clear()
		{
			pData = nullptr;
		}

		bool empty() const {return !pData;}

		T& reference()
		{
			if (!pData)
				pData = new T();
			return *pData;
		}

		const T& data() const {return *pData;}
		T& data() {return *pData;}

		template<class U>
		void assign(const U& v)
		{
			if (!pData)
				pData = new T(v);
			else
				*pData = v;
		}

		void assign(const NullPtr&)
		{
			pData = nullptr;
		}


		void assign(const Holder& rhs)
		{
			pData = rhs.pData;
		}

		void swap(Holder& rhs)
		{
			pData.swap(rhs.pData);
		}

	private:
		/*!
		** \brief Pointer to the data
		**
		** The pointer is actually a smartptr to handle the copy constructor efficiently
		** for the class `Holder`.
		*/
		typename T::Ptr pData;

	}; // class Holder





} // namespace NullableImpl
} // namespace Private
} // namespace Yuni

