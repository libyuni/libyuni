
#include "protocol.h"


namespace Yuni
{
namespace Messaging
{

	void Protocol::shrinkMemory()
	{
		// ask to reduce the memory consumption to all schemas
		Schema::Hash::iterator end = pSchemas.end();
		for (Schema::Hash::iterator i = pSchemas.begin(); i != end; ++i)
			i->second.shrinkMemory();

		// release
		pTmp.clear();
		pTmp.shrink();
	}




} // namespace Messaging
} // namespace Yuni


