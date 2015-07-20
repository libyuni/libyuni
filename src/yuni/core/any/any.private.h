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
#include "../static/types.h"



namespace Yuni
{
namespace Private
{

/*!
** \brief Any implementation details
**
** This namespace stores a big part of the actual Any
** implementation, in particular the Static Function Pointer
** Table Polymorphism Pattern (SFPTPP) implementation.
*/
namespace Any
{


	/*!
	** \brief Static function pointer table
	**
	** All the operations done on Any storage objects are done via
	** the functions currently defined in pTable. Depending on the size of the object,
	** this member may use different approachs to store the data.
	** This function table defines the function pointers to the different operations.
	*/
	struct TypeManipulationTable
	{
		const std::type_info& (*type)();
		void (*staticDelete)(void**);
		void (*clone)(void* const*, void**);
		void (*move)(void* const*, void**);
	};



	/*!
	** \brief Static functions for small objects (smaller than a void *)
	**
	** The goal of this struct is to use pObject as a sort of union
	** to store object smaller than pointer size. Of course, this means that
	** bigger objects can be stored faster if you use 64-bit addresses.
	*/
	template <bool isSmall>
	struct Functions final
	{
		template <typename T>
		struct TypeManipulator
		{
			static const std::type_info& Type()
			{
				return typeid(T);
			}

			static void Delete(void** object)
			{
				reinterpret_cast<T*>(object)->~T();
			}

			static void Clone(void* const* source, void** dest)
			{
				new (dest) T(*reinterpret_cast<T const*>(source));
			}

			static void Move(void* const* source, void** dest)
			{
				reinterpret_cast<T*>(dest)->~T();
				*reinterpret_cast<T*>(dest) = *reinterpret_cast<T const*>(source);
			}
		};
	};


	/*!
	** \brief Static functions implementations for big objects (larger than a void *)
	**
	** This implementation stores big objects as normal objects (ie. a pointer to an alloc'ed
	** object in pObject.)
	*/
	template <>
	struct Functions<false> final
	{
		template <typename T>
		struct TypeManipulator
		{
			static const std::type_info& Type()
			{
				return typeid(T);
			}

			static void Delete(void** object)
			{
				delete(*reinterpret_cast<T**>(object));
			}

			static void Clone(void* const* source, void** dest)
			{
				*dest = new T(**reinterpret_cast<T* const*>(source));
			}

			static void Move(void* const* source, void** dest)
			{
				(*reinterpret_cast<T**>(dest))->~T();
				**reinterpret_cast<T**>(dest) = **reinterpret_cast<T* const*>(source);
			}
		};
	};


	/*!
	** \brief Structure providing function table for a type.
	**
	** This structure provides a function to get the appropriate function table for
	** a given type in an allegedly sexy way. (like Table<T>::get())
	*/
	template<typename T>
	struct Table final
	{
		/*!
		** \brief Returns a pointer on a static function table
		**
		** The static function table depends on the type and its size.
		** One different function table will be instanciated by type.
		*/
		static TypeManipulationTable * Get()
		{
			static TypeManipulationTable staticTable =
			{
				Functions<Static::Type::IsSmall<T>::Yes>::template TypeManipulator<T>::Type,
				Functions<Static::Type::IsSmall<T>::Yes>::template TypeManipulator<T>::Delete,
				Functions<Static::Type::IsSmall<T>::Yes>::template TypeManipulator<T>::Clone,
				Functions<Static::Type::IsSmall<T>::Yes>::template TypeManipulator<T>::Move
			};

			return &staticTable;
		}
	};



	/*!
	** \brief Any type for an empty Any.
	*/
	struct Empty {};



} // namespace Any
} // namespace Private
} // namespace Yuni
