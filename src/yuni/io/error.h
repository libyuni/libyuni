#ifndef __YUNI_IO_ERROR_H__
# define __YUNI_IO_ERROR_H__


namespace Yuni
{
namespace IO
{

	/*!
	** \brief Error list
	*/
	enum Error
	{
		//! No error
		errNone = 0,
		//! Generic error
		errUnknown,
		//! Bad filename
		errBadFilename,
		//! The file could not be loaded - not found or permission error
		errNotFound,
		//! A hard memory limit has been reached
		errMemoryLimit,
		//! It is impossible to overwrite an existing file
		errOverwriteNotAllowed,
		//! Impossible to write
		errWriteFailed,
		//! Impossible to read
		errReadFailed,
	};




} // namespace IO
} // namespace Yuni

#endif // __YUNI_IO_ERROR_H__
