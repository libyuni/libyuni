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
#include "../../yuni.h"
#include "../static/remove.h"



namespace Yuni
{
namespace Extension
{

	/*!
	** \brief Extension: Get the length of the inner buffer
	*/
	template<class C, class SizeT>
	class Length final
	{
	public:
		typedef SizeT SizeType;
		enum { valid = 0, isFixed = 0, fixedLength = 0, };

	public:
		template<class U> static SizeT Value(const U&) {return 0;}
	};


} // namespace Extension
} // namespace Yuni


namespace Yuni
{
namespace Traits
{

	/*!
	** \brief Traits: Length (number of items) of an arbitrary container
	**
	** The length is the number of items contained in the container.
	**
	** \tparam U Any class (Static::Remove::Const<> should be used with this parameter)
	** \tparam SizeT The type to use for the returned length
	*/
	template<class U, class SizeT = size_t>
	class Length final
	{
	public:
		//! The original type without its const qualifier
		typedef typename Static::Remove::Const<U>::Type Type;
		//! The type to use for the returned length
		typedef SizeT SizeType;
		//! Extension
		typedef Extension::Length<Type,SizeT>  ExtensionType;
		enum
		{
			//! A non-zero value if the specialization is valid
			valid = ExtensionType::valid,
			//! A non-zero value when the size is known at compile time
			isFixed = ExtensionType::isFixed,
			//! The fixed length value when it can be known at compile time (can be 0), 0 otherwise
			fixedLength = ExtensionType::fixedLength,
		};

	public:
		/*!
		** \brief Get the length of the container
		**
		** \internal The template T is here to manage some special cases with
		**   the const qualifier, especially when U = const char* const. Your compiler
		**   may complain about the following error :
		**   "invalid conversion from ‘const char* const’ to ‘char*’"
		** \param value A arbitrary variable
		** \return The length of the container
		*/
		template<class T>
		static SizeT Value(const T& value) { return ExtensionType::Value(value); }

	}; // class Length<U>





} // namespace Traits
} // namespace Yuni

#include "extension/length.h"
