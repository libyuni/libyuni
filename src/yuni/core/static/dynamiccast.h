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
#ifndef __YUNI_CORE_STATIC_DYNAMIC_CAST_H__
# define __YUNI_CORE_STATIC_DYNAMIC_CAST_H__

# include "inherit.h"


namespace Yuni
{
namespace Static
{


	// Forward declaration
	template<class C, class BaseT, int CInheritsFromBaseT = InheritsFrom<C,BaseT>::Yes>
	struct DynamicCastWhenInherits;



	/*!
	** \brief Perform dynamic_cast operations only when a class `C` inherits from another one `BaseT`
	**
	** \tparam C Any class (may be virtual)
	** \tparam BaseT The base class to compare with (may be virtual)
	*/
	template<class C, class BaseT, int CInheritsFromBaseT>
	struct DynamicCastWhenInherits
	{
		enum { Yes = 1, No = 0, };

		/*!
		** \brief Test if two pointer are equivalent (means "represents the same real object")
		**
		** \param ref A pointer to a base class
		** \param objPointer A pointer to any descendant of `ref`
		** \return True if C inherits from BaseT and that the two pointer are dynamically equal
		**   (represent the same object)
		*/
		template<class U, class V>
		static bool Equals(const U* ref, const V* objPointer)
		{
			return ref == dynamic_cast<const U*>(objPointer);
		}

		template<class U>
		static BaseT* Perform(U* object)
		{
			return dynamic_cast<BaseT*>(object);
		}

		template<class U>
		static const BaseT* PerformConst(const U* object)
		{
			return dynamic_cast<const BaseT*>(object);
		}

	};


	template<class C, class BaseT>
	struct DynamicCastWhenInherits<C,BaseT,0>
	{
		enum { Yes = 0, No = 1, };

		template<class U, class V>
		static bool Equals(const U*, const V*)
		{
			return false;
		}

		template<class U>
		static BaseT* Perform(U*)
		{
			return NULL;
		}

		template<class U>
		static const BaseT* PerformConst(const U*)
		{
			return NULL;
		}

	};





} // namespace Static
} // namespace Yuni

#endif // __YUNI_CORE_STATIC_DYNAMIC_CAST_H__
