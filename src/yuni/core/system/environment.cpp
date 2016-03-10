/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** github: https://github.com/libyuni/libyuni/
** gitlab: https://gitlab.com/libyuni/libyuni/ (mirror)
*/
#include "environment.h"
#include "windows.hdr.h"
#include "../../string.h"
#ifdef YUNI_HAS_STDLIB_H
#	include <stdlib.h>
#endif
#include "../string/wstring.h"



namespace Yuni
{
namespace System
{
namespace Environment
{

	namespace // anonymous
	{

		template<class StringT>
		inline bool ReadImpl(const AnyString& name, StringT& out)
		{
			#ifdef YUNI_OS_WINDOWS
			{
				WString nameUTF16(name);

				// Getting the size of the content
				// (in characters, *INCLUDING* the null-character since input buffer is null)
				DWORD size = GetEnvironmentVariableW(nameUTF16.c_str(), nullptr, 0);
				if (size > 1 and size <= 32767) // windows hard-coded value
				{
					// allocating a new buffer for receiving the value
					// TODO use a buffer on the stack for very small values to reduce memory allocations
					// (especially for ReadAsBool, ReadAsInt64 and ReadAsUInt64)
					wchar_t* buffer = (wchar_t*)::malloc(sizeof(wchar_t) * size);
					if (buffer)
					{
						// retrieving the content of the var into 'buffer'
						// since the buffer will be large enough, the returned size
						// will be the number of characters *NOT INCLUDING* the null-character
						size = GetEnvironmentVariableW(nameUTF16.c_str(), buffer, size);
						if (size != 0)
						{
							int sizeRequired = WideCharToMultiByte(CP_UTF8, 0, buffer, (int) size, NULL, 0, NULL, NULL);
							if (sizeRequired > 0)
							{
								out.reserve(out.size() + sizeRequired);
								int sizeRequired = WideCharToMultiByte(CP_UTF8, 0, buffer, (int) size, out.data() + out.size(), size, NULL, NULL);
								::free(buffer);
								out.resize(out.size() + (uint) sizeRequired);
								return true;
							}
						}
						::free(buffer);
					}
				}
			}
			#else
			{
				# ifdef YUNI_HAS_STDLIB_H
				const char* e = ::getenv(name.c_str());
				if (e)
				{
					out += e;
					return true;
				}
				return false;

				# else
				# error missing implementation
				assert(false and "missing implementation");
				# endif
			}
			#endif // windows

			return false; // fallback
		}

	} // anonymous namespace




	bool ReadAsBool(const AnyString& name, bool defvalue)
	{
		if (not name.empty())
		{
			#ifdef YUNI_OS_WINDOWS
			{
				String out;
				if (ReadImpl(name, out) and not out.empty())
					return out.to<bool>();
			}
			#else
			{
				# ifdef YUNI_HAS_STDLIB_H
				AnyString value = ::getenv(name.c_str());
				if (not value.empty())
				{
					bool result;
					if (value.to<bool>(result))
						return result;
				}
				# else
				#error not implemented
				# endif
			}
			#endif
		}
		return defvalue;
	}


	yint64 ReadAsInt64(const AnyString& name, yint64 defvalue)
	{
		if (not name.empty())
		{
			#ifdef YUNI_OS_WINDOWS
			{
				String out;
				ReadImpl(name, out);
				if (ReadImpl(name, out) and not out.empty())
				{
					yint64 result;
					if (out.to<yint64>(result))
						return result;
				}
			}
			#else
			{
				# ifdef YUNI_HAS_STDLIB_H
				AnyString value = ::getenv(name.c_str());
				if (not value.empty())
				{
					yint64 result;
					if (value.to<yint64>(result))
						return result;
				}
				# else
				#error not implemented
				# endif
			}
			#endif
		}
		return defvalue;
	}


	yuint64 ReadAsUInt64(const AnyString& name, yuint64 defvalue)
	{
		if (not name.empty())
		{
			#ifdef YUNI_OS_WINDOWS
			{
				String out;
				if (ReadImpl(name, out) and not out.empty())
				{
					yuint64 result;
					if (out.to<yuint64>(result))
						return result;
				}
			}
			#else
			{
				# ifdef YUNI_HAS_STDLIB_H
				AnyString value = ::getenv(name.c_str());
				if (not value.empty())
				{
					yuint64 result;
					if (value.to<yuint64>(result))
						return result;
				}
				# else
				#error not implemented
				# endif
			}
			#endif
		}
		return defvalue;
	}


	String Read(const AnyString& name)
	{
		if (not name.empty())
		{
			#ifdef YUNI_OS_WINDOWS
			{
				String out;
				ReadImpl(name, out);
				return out;
			}
			#else
			{
				# ifdef YUNI_HAS_STDLIB_H
				return ::getenv(name.c_str());
				# else
				#error not implemented
				# endif
			}
			#endif
		}
		return String(); // fallback
	}


	bool Read(const AnyString& name, Clob& out, bool emptyBefore)
	{
		if (emptyBefore)
			out.clear();
		return (not name.empty()) ? ReadImpl(name, out) : false;
	}


	bool Read(const AnyString& name, String& out, bool emptyBefore)
	{
		if (emptyBefore)
			out.clear();
		return (not name.empty()) ? ReadImpl(name, out) : false;
	}


	bool Exists(const AnyString& name)
	{
		if (not name.empty())
		{
			#ifdef YUNI_OS_WINDOWS
			{
				WString nameUTF16(name);
				DWORD size = GetEnvironmentVariableW(nameUTF16.c_str(), nullptr, 0);
				return (size >= 0);
			}
			#else
			# ifdef YUNI_HAS_STDLIB_H
			return (nullptr != ::getenv(name.c_str()));
			# else
			# error not implemented
			# endif
			#endif
		}
		return false;
	}


	void Set(const AnyString& name, const AnyString& value)
	{
		if (not name.empty())
		{
			#ifdef YUNI_OS_WINDOWS
			{
				WString nameUTF16(name);
				if (not value.empty())
				{
					WString valueUTF16(value);
					const wchar_t* w = (not valueUTF16.empty()) ? valueUTF16.c_str() : L"";
					SetEnvironmentVariable(nameUTF16.c_str(), w);
				}
				else
					SetEnvironmentVariable(nameUTF16.c_str(), L"");
			}
			#else
			# ifdef YUNI_HAS_STDLIB_H
			::setenv(name.c_str(), (not value.empty()) ? value.c_str() : "", 1);
			# else
			# error not implemented
			# endif
			#endif
		}
	}


	void Unset(const AnyString& name)
	{
		if (not name.empty())
		{
			#ifdef YUNI_OS_WINDOWS
			WString nameUTF16(name);
			SetEnvironmentVariable(nameUTF16.c_str(), nullptr);
			#else
			# ifdef YUNI_HAS_STDLIB_H
			::unsetenv(name.c_str());
			# else
			# error not implemented
			# endif
			#endif
		}
	}






} // namespace Environment
} // namespace System
} // namespace Yuni

