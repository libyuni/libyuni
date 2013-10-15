#ifndef __YUNI_IO_DIRECTORY_INFO_PLATFORM_H__
# define __YUNI_IO_DIRECTORY_INFO_PLATFORM_H__

# include "../../../core/string.h"


namespace Yuni
{
namespace Private
{
namespace IO
{
namespace Directory
{

	//! Platform-dependant data implementation
	class IteratorData;



	IteratorData* IteratorDataCreate(const AnyString& folder, uint flags);

	IteratorData* IteratorDataCopy(const IteratorData*);

	void IteratorDataFree(const IteratorData*);

	bool IteratorDataValid(const IteratorData*);

	IteratorData* IteratorDataNext(IteratorData*);

	const String& IteratorDataFilename(const IteratorData*);

	const String& IteratorDataParentName(const IteratorData*);

	const String& IteratorDataName(const IteratorData*);

	uint64 IteratorDataSize(const IteratorData*);

	sint64 IteratorDataModified(const IteratorData*);

	bool IteratorDataIsFolder(const IteratorData*);

	bool IteratorDataIsFile(const IteratorData*);




} // namespace Directory
} // namespace IO
} // namespace Private
} // namespace Yuni

#endif // __YUNI_IO_DIRECTORY_INFO_PLATFORM_H__
