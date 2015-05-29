#ifndef __YUNI_CORE_STRING_TRAITS_FILL_H__
# define __YUNI_CORE_STRING_TRAITS_FILL_H__

# include "../../traits/length.h"


namespace Yuni
{
namespace Extension
{
namespace CString
{


	template<class CStringT, class StringT>
	class Fill final
	{
	public:
		static void Perform(char* data, typename CStringT::Size size, const StringT& pattern)
		{
			const uint patternSize = Traits::Length<StringT,uint>::Value(pattern);
			if (0 == patternSize)
				return;

			const char* const cstr = Traits::CString<StringT>::Perform(pattern);
			// If equals to 1, it is merely a single char
			if (1 == patternSize)
			{
				for (typename CStringT::Size i = 0; i < size; ++i)
					data[i] = *cstr;
				return;
			}
			// We have to copy N times the pattern
			typename CStringT::Size p = 0;
			while (p + patternSize <= size)
			{
				YUNI_MEMCPY(data + p, patternSize * sizeof(char), cstr, patternSize * sizeof(char));
				p += patternSize;
			}
			for (; p < size; ++p)
				data[p] = ' ';
		}
	};


	template<class CStringT>
	class Fill<CStringT, char> final
	{
	public:
		static void Perform(char* data, typename CStringT::Size size, const char rhs)
		{
			for (typename CStringT::Size i = 0; i != size; ++i)
				data[i] = rhs;
		}
	};






} // namespace CString
} // namespace Extension
} // namespace Yuni

#endif // __YUNI_CORE_STRING_TRAITS_FILL_H__
