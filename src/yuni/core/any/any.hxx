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
#ifndef __YUNI_CORE_ANY_ANY_HXX__
#define __YUNI_CORE_ANY_ANY_HXX__

namespace Yuni
{

	template <typename T>
	Any::Any(const T& source)
	{
		pTable = Private::Any::Table<T>::Get();
		if (sizeof(T) <= sizeof(void*))
			new (&pObject) T(source);
		else
			pObject = new T(source);
	}

	template <typename T>
	Any& Any::assign(const T& rhs)
	{
		// Are we copying between the same type of variants ?
		Private::Any::TypeManipulationTable* rhsTable = Private::Any::Table<T>::Get();

		if (pTable == rhsTable)
		{
			// Yes, so we can avoid reallocating, and re-use memory.
			if (sizeof(T) <= sizeof(void*))
			{
				// Call the destructor on the object to clean up.
				reinterpret_cast<T*>(&pObject)->~T();
				// Create copy on-top of object pointer itself
				new (&pObject) T(rhs);
			}
			else
			{
				// Call the destructor on the old object, but do not deallocate memory !
				reinterpret_cast<T*>(pObject)->~T();
				// Create copy on-top of the old version
				new (pObject) T(rhs);
			}
		}
		else
		{
			// No, this was not the same type.
			reset();
			if (sizeof(T) <= sizeof(void*))
			{
				// Create copy on-top of object pointer itself
				new (&pObject) T(rhs);
				// Update table pointer
				pTable = rhsTable;
			}
			else
			{
				pObject = new T(rhs);
				pTable = rhsTable;
			}
		}
		return *this;
	}

	template <typename T>
	const T& Any::to() const
	{
		if (type() != typeid(T))
			throw Exceptions::BadCast(type(), typeid(T));

		if (sizeof(T) <= sizeof(void*))
			return *reinterpret_cast<T const*>(&pObject);

		return *reinterpret_cast<T const*>(pObject);
	}

	template <typename T>
	void Any::initFromCString(T source)
	{
		pTable = Private::Any::Table<String>::Get();
		if (sizeof(String) <= sizeof(void*))
			new (&pObject) String(source);
		else
			pObject = new String(source);
	}

}

#endif /* !__YUNI_CORE_ANY_ANY_HXX__ */
