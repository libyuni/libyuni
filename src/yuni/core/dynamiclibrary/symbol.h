#ifndef __YUNI_CORE_DYNAMICLIBRARY_SYMBOL_H__
# define __YUNI_CORE_DYNAMICLIBRARY_SYMBOL_H__
/*!
** Header for Yuni::DynamicLibrary::Symbol, a class for representing an exported
** symbol by a (shared) library
*/
# include "../../yuni.h"


namespace Yuni
{
namespace DynamicLibrary
{

	/*!
	** \brief Exported Symbol from a dynamic library
	*/
	class Symbol final
	{
	public:
		//! Handle for a symbol
		typedef void* Handle;

	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default constructor
		*/
		Symbol();
		/*!
		** \brief Constructor with a given handle
		*/
		Symbol(Handle p);
		/*!
		** \brief Copy constructor
		*/
		Symbol(const Symbol& rhs);
		//@}


		//! \name Validity
		//@{
		//! Get if the symbol is invalid
		bool null() const;
		//! Get if the symbol is valid
		bool valid() const;
		//@}


		//! \name Operator
		//@{
		//! Copy operator
		Symbol& operator = (const Symbol& rhs);
		//! Copy operator
		Symbol& operator = (Symbol::Handle hndl);
		//@}


		//! \name Handle
		//@{
		/*!
		** \brief Get the handle of the symbol
		*/
		Handle ptr() const;
		//@}


	private:
		//! Handle
		Handle pPtr;

	}; // class Symbol





} // namespace DynamicLibrary
} // namespace Yuni

# include "symbol.hxx"

#endif // __YUNI_CORE_DYNAMICLIBRARY_SYMBOL_H__
