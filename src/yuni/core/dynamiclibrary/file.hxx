#ifndef __YUNI_CORE_DYNAMICLIBRARY_FILE_HXX__
# define __YUNI_CORE_DYNAMICLIBRARY_FILE_HXX__

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


	template<class StringT>
	inline Symbol File::operator [] (const StringT& name) const
	{
		return resolve(name);
	}




} // namespace DyanmicLibrary
} // namespace Yuni

#endif // __YUNI_CORE_DYNAMICLIBRARY_FILE_HXX__
