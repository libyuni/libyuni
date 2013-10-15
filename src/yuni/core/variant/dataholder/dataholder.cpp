
#include "../variant.h"


namespace Yuni
{
namespace Private
{
namespace Variant
{


	IDataHolder* IDataHolder::invoke(const String& /*name*/)
	{
		return nullptr;
	}


	IDataHolder* IDataHolder::invoke(const String& /*name*/, IDataHolder* /*a1*/)
	{
		return nullptr;
	}


	IDataHolder* IDataHolder::invoke(const String& /*name*/, IDataHolder* /*a1*/, IDataHolder* /*a2*/)
	{
		return nullptr;
	}


	IDataHolder* IDataHolder::invoke(const String& /*name*/, IDataHolder* /*a1*/, IDataHolder* /*a2*/, IDataHolder* /*a3*/)
	{
		return nullptr;
	}


	IDataHolder* IDataHolder::invoke(const String& /*name*/, IDataHolder* /*a1*/, IDataHolder* /*a2*/, IDataHolder* /*a3*/, IDataHolder* /*a4*/)
	{
		return nullptr;
	}




} // namespace Variant
} // namespace Private
} // namespace Yuni

