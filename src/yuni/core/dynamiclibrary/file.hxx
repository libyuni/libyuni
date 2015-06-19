#pragma once
#include "file.h"



namespace Yuni
{
namespace DynamicLibrary
{

	inline bool File::loaded() const
	{
		return (pHandle != NullHandle);
	}


	inline const String& File::filename() const
	{
		return pFilename;
	}


	inline File::Handle File::handle() const
	{
		return pHandle;
	}


	inline Symbol File::operator [] (const AnyString& name) const
	{
		return resolve(name);
	}




} // namespace DyanmicLibrary
} // namespace Yuni
