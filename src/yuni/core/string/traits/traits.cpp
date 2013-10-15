
#include "../../../yuni.h"
#include "traits.h"
#include <ctype.h>



namespace Yuni
{
namespace Private
{
namespace CStringImpl
{


	bool Equals(const char* const s1, const char* const s2, uint len)
	{
		for (uint i = 0; i != len; ++i)
		{
			if (s1[i] != s2[i])
				return false;
		}
		return true;
	}


	bool EqualsInsensitive(const char* const s1, const char* const s2, uint len)
	{
		for (uint i = 0; i != len; ++i)
		{
			if (tolower(s1[i]) != tolower(s2[i]))
				return false;
		}
		return true;
	}


	int Compare(const char* const s1, uint l1, const char* const s2, uint l2)
	{
		const uint l = (l1 < l2) ? l1 : l2;

		for (uint i = 0; i != l; ++i)
		{
			if (s1[i] != s2[i])
				return (((unsigned char)s1[i] < (unsigned char)s2[i]) ? -1 : +1);
		}
		return (l1 == l2) ? 0 : ((l1 < l2) ? -1 : +1);
	}


	int CompareInsensitive(const char* const s1, uint l1, const char* const s2, uint l2)
	{
		const uint l = (l1 < l2) ? l1 : l2;

		for (uint i = 0; i != l; ++i)
		{
			if (tolower(s1[i]) != tolower(s2[i]))
				return ((tolower((unsigned char)s1[i]) < tolower((unsigned char)s2[i])) ? -1 : +1);
		}
		return (l1 == l2) ? 0 : ((l1 < l2) ? -1 : +1);
	}


	bool Glob(const char* const s, uint l1, const char* const pattern, uint patternlen)
	{
		if (patternlen)
		{
			if (l1)
			{
				uint e = 0;
				uint prev = ((uint) -1);
				for (uint i = 0 ; i < l1; ++i)
				{
					if ('*' == pattern[e])
					{
						if (e + 1 == patternlen)
							return true;
						while (pattern[e+1] == '*')
							++e;
						if (e + 1 == patternlen)
							return true;

						prev = e;
						if (pattern[e + 1] == s[i])
							e += 2;
					}
					else
					{
						if (pattern[e] == s[i])
							++e;
						else
						{
							if (prev != ((uint) -1))
								e = prev;
							else
								return false;
						}
					}
				}
				return (e == patternlen);
			}
			return false;
		}
		return (l1 == 0);
	}





} // namespace CStringImpl
} // namespace Private
} // namespace Yuni

