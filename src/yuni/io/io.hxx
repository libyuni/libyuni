#ifndef __YUNI_IO_IO_HXX__
# define __YUNI_IO_IO_HXX__


namespace Yuni
{
namespace IO
{

	inline bool Exists(const AnyString& filename)
	{
		return (Yuni::IO::typeUnknown != Yuni::IO::TypeOf(filename));
	}



} // namespace IO
} // namespace Yuni

#endif // __YUNI_IO_IO_HXX__
