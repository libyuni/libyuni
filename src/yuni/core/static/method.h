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
#ifndef __YUNI_CORE_STATIC_HAS_METHOD_H__
# define __YUNI_CORE_STATIC_HAS_METHOD_H__

# include "../../yuni.h"
# include "remove.h"


/*!
** \def YUNI_IMPL_STATIC_HAS_METHOD
** \brief Macro for implementing a check for Determining whether a class has a specific typedef or not
**
** \bug The signature is not checked in C++11
*/
# if !defined(YUNI_HAS_CPP_KEYWORD_FINAL)

// No keyword final, thus we always can inherit from the given Type

# define YUNI_IMPL_STATIC_HAS_METHOD(METHOD, RETURN_TYPE, ...)  \
		template<class T> \
		class METHOD final \
		{ \
			typedef struct {char a[2];}   TrueType; \
			typedef char  FalseType; \
			typedef typename ::Yuni::Static::Remove::All<T>::Type  Type; \
			\
			typedef RETURN_TYPE  ReturnType; \
			struct BaseMixin { ReturnType  METHOD __VA_ARGS__ { return RETURN_TYPE(); } }; \
			struct Base : public Type, public BaseMixin {}; \
			template<class C, C> struct Identity ;\
			\
			template<class C, class U> static TrueType deduce(C*, U C::* = 0); \
			template<class C> static FalseType deduce(...); \
			\
			template<class C> static FalseType  deduce(C*, Identity<RETURN_TYPE (BaseMixin::*)__VA_ARGS__, &C::METHOD>* = 0); \
			static TrueType deduce(...); \
			\
		public: \
			enum \
			{ \
				yes = sizeof(deduce((Base*) 0)) == sizeof(TrueType), \
				no = !yes, \
			}; \
		}


# else

# include <type_traits>

# define YUNI_IMPL_STATIC_HAS_METHOD(METHOD, RETURN_TYPE, ...)  \
		template<class T> \
		class METHOD final \
		{ \
			typedef struct {char a[2];}   TrueType; \
			typedef char  FalseType; \
			typedef typename ::Yuni::Static::Remove::All<T>::Type  Type; \
			template<int> struct Helper; \
			template<class C, C> struct Identity ;\
			\
			template<class C> static TrueType  deduce(C*, Helper<std::is_member_function_pointer<decltype(&C::METHOD)>::value>* = 0); \
			template<class C> static FalseType deduce(...); \
			\
		public: \
			enum \
			{ \
				yes = sizeof(deduce<Type>(0)) == sizeof(TrueType), \
				no =  ! yes, \
			}; \
		}


# endif



namespace Yuni
{
namespace Static
{
namespace HasMethod
{

	/*!
	** \brief Determine whether a class has the method `addRef`
	*/
	YUNI_IMPL_STATIC_HAS_METHOD(addRef, void, ());


	/*!
	** \brief Determine whether a class has the method `release`
	*/
	YUNI_IMPL_STATIC_HAS_METHOD(release, bool, ());


	/*!
	** \brief Determine whether a class has the method `hasIntrusiveSmartPtr`
	*/
	YUNI_IMPL_STATIC_HAS_METHOD(hasIntrusiveSmartPtr, bool, ());






} // namespace HasMethod
} // namespace Static
} // namespace Yuni

#endif // __YUNI_CORE_STATIC_HAS_METHOD_H__
