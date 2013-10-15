#ifndef __YUNI_IO_CONSTANTS_H__
# define __YUNI_IO_CONSTANTS_H__

# include "io.h"


namespace Yuni
{
namespace IO
{

	
	/*!
	** \brief Flow control used in the IO module
	*/
	enum Flow
	{
		//! Abort the whole process
		flowAbort = 0,
		//! Continue
		flowContinue,
		//! Skip the current item
		flowSkip,
	};


	/*!
	** \brief Type of a single node (bitmask)
	*/
	enum NodeType
	{
		//! The node doest not exist
		typeUnknown = 0,
		//! The node is a folder
		typeFolder = 1,
		//! The node is a file
		typeFile = 2,
	};




	//! \name System-dependant variables
	//@{
	/*!
	** \brief The path-separator character according to the platform (ex: `/`)
	*/
	extern const char Separator; // '/'

	/*!
	** \brief The path-separator character according to the platform (stored in a string instead of a char)
	*/
	extern const char* SeparatorAsString; // "/"

	/*!
	** \brief Constant acoording a type
	**
	** These variables are identical to SeparatorAsString and Separator
	** but are charactere-type dependant.
	*/
	template<typename C /* = char*/> struct Constant;
	template<> struct Constant<char>
	{
		// The complete specialization with wchar_t is in directory.hxx

		//! The path-separator character according to the platform (ex: `/`)
		static const char  Separator; // '/';
		//! The path-separator character according to the platform (stored in a string instead of a char)
		static const char* SeparatorAsString; // "/"
		//! All path-separator characters, for all platforms
		static const char* AllSeparators; // "\\/"

		//! Dot
		static const char  Dot; // '.';

	}; // class Constant<char>

	template<> struct Constant<wchar_t>
	{
		//! The path-separator character according to the platform (ex: `/`)
		static const wchar_t  Separator; // L'/';
		//! The path-separator character according to the platform (stored in a string instead of a char)
		static const wchar_t* SeparatorAsString; // = L"/";
		//! All path-separator characters, for all platforms
		static const wchar_t* AllSeparators; // = L"\\/";
		//! Dot
		static const wchar_t  Dot; // L'.';
	};
	//@}





} // namespace IO
} // namespace Yuni

#endif // __YUNI_IO_CONSTANTS_H__
