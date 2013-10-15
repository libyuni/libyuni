#ifndef __YUNI_CORE_STRING_WSTRING_HXX__
# define __YUNI_CORE_STRING_WSTRING_HXX__

# ifdef YUNI_OS_WINDOWS

namespace Yuni
{
namespace Private
{

	template<bool UNCPrefix, bool AppendSeparatorT>
	inline WString<UNCPrefix, AppendSeparatorT>::WString(const AnyString& string)
	{
		prepareWString(string.c_str(), string.size());
	}


	template<bool UNCPrefix, bool AppendSeparatorT>
	inline WString<UNCPrefix, AppendSeparatorT>::WString(const char* cstring, uint size)
	{
		prepareWString(cstring, size);
	}


	template<bool UNCPrefix, bool AppendSeparatorT>
	inline WString<UNCPrefix, AppendSeparatorT>::~WString()
	{
		delete[] pWString;
	}


	template<bool UNCPrefix, bool AppendSeparatorT>
	inline uint WString<UNCPrefix, AppendSeparatorT>::size() const
	{
		return pSize;
	}


	template<bool UNCPrefix, bool AppendSeparatorT>
	inline void WString<UNCPrefix, AppendSeparatorT>::replace(char from, char to)
	{
		for (uint i = 0; i != pSize; ++i)
		{
			if (pWString[i] == from)
				pWString[i] = to;
		}
	}


	template<bool UNCPrefix, bool AppendSeparatorT>
	inline bool WString<UNCPrefix, AppendSeparatorT>::empty() const
	{
		return !pSize;
	}


	template<bool UNCPrefix, bool AppendSeparatorT>
	inline const wchar_t* WString<UNCPrefix, AppendSeparatorT>::c_str() const
	{
		return pWString;
	}


	template<bool UNCPrefix, bool AppendSeparatorT>
	inline wchar_t* WString<UNCPrefix, AppendSeparatorT>::c_str()
	{
		return pWString;
	}


	template<bool UNCPrefix, bool AppendSeparatorT>
	inline WString<UNCPrefix, AppendSeparatorT>::operator const wchar_t* () const
	{
		return pWString;
	}


	template<bool UNCPrefix, bool AppendSeparatorT>
	void WString<UNCPrefix, AppendSeparatorT>::prepareWString(const char* const cstring, uint size)
	{
		if (!size)
		{
			if (UNCPrefix)
			{
				pSize = 4;
				pWString = new wchar_t[5];
				pWString[0] = L'\\';
				pWString[1] = L'\\';
				pWString[2] = L'?';
				pWString[3] = L'\\';
				pWString[4] = L'\0';
			}
			else
			{
				pSize = 0;
				pWString = NULL;
			}
			return;
		}

		// Offset according to the presence of the UNC prefix
		const uint offset = UNCPrefix ? 4 : 0;

		// Size of the wide string. This value may be modified if there is not
		// enough room for converting the C-String
		pSize = size + offset + (AppendSeparatorT ? 1 : 0);

		// Allocate and convert the C-String. Several iterations may be required
		// for allocating enough room for the conversion.
		do
		{
			pWString = new wchar_t[pSize + (AppendSeparatorT ? 1 : 0) + 1];
			if (!pWString)
			{
				// Impossible to allocate the buffer. Aborting.
				pWString = NULL;
				pSize = 0;
				return;
			}

			// Appending the Windows UNC prefix
			if (UNCPrefix)
			{
				pWString[0] = L'\\';
				pWString[1] = L'\\';
				pWString[2] = L'?';
				pWString[3] = L'\\';
			}
			// Converting into Wide String
			const int n = MultiByteToWideChar(CP_UTF8, 0, cstring, size, pWString + offset, pSize + 1);
			if (n <= 0)
			{
				// An error has occured
				if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				{
					// There is not enough room for the conversion
					// Trying again with more rooms
					pSize += (pSize > 2) ? pSize / 2 : 2;
					delete[] pWString;
					continue;
				}
				// This is a real error. Aborting.
				delete[] pWString;
				pWString = NULL;
				pSize = 0;
				return;
			}
			// Adding the final zero
			if (!AppendSeparatorT)
			{
				pWString[n + offset] = L'\0';
				pSize = n + offset;
			}
			else
			{
				pWString[n + offset] = L'\\';
				pWString[n + offset + 1] = L'\0';
				pSize = n + offset + 1;
			}
			return;
		}
		while (true);
	}





} // namespace Private
} // namespace Yuni

# endif // YUNI_OS_WINDOWS
#endif // __YUNI_CORE_STRING_WSTRING_HXX__
