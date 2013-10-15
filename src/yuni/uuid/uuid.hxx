#ifndef __YUNI_UUID_UUID_HXX__
# define __YUNI_UUID_UUID_HXX__


namespace Yuni
{

	template<class StringT>
	inline UUID::UUID(const StringT& string)
	{
		if (not assign(string))
			clear();
	}


	inline bool UUID::operator ! () const
	{
		return null();
	}


	template<class StringT>
	UUID& UUID::operator = (const StringT& string)
	{
		if (not assign(string))
			clear();
		return *this;
	}


} // namespace Yuni







namespace Yuni
{
namespace Private
{
namespace UUID
{

	class Helper final
	{
	public:
		static void WriteToCString(char* cstr, const Yuni::UUID& uuid)
		{
			uuid.writeToCString(cstr);
		}
	};


} // namespace UUID
} // namespace Private
} // namespace Yuni







namespace Yuni
{
namespace Extension
{
namespace CString
{

	template<class CStringT>
	class Append<CStringT, Yuni::UUID> final
	{
	public:
		static void Perform(CStringT& s, const Yuni::UUID& rhs)
		{
			const uint currentLength = s.size();
			// writeToCString is guarantee to have 42 chars
			s.reserve(currentLength + 42); // at least 36 + 1 zero-terminated
			Yuni::Private::UUID::Helper::WriteToCString((char*)s.data() + currentLength, rhs);
			s.resize(currentLength + 36); // guid is 36-bytes length
			//s.removeLast();
		}

	}; // class Append


	template<>
	class Into<Yuni::UUID> final
	{
	public:
		typedef Yuni::UUID TargetType;
		enum { valid = 1 };

		template<class StringT> static bool Perform(const StringT& s, TargetType& out)
		{
			return out.assign(s);
		}

		template<class StringT> static TargetType Perform(const StringT& s)
		{
			return TargetType(s);
		}

	}; // class Into



} // namespace CString
} // namespace Extension
} // namespace Yuni


// ostream
std::ostream& operator << (std::ostream& out, const Yuni::UUID& rhs);

#endif // __YUNI_UUID_UUID_HXX__
