/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** gitlab: https://gitlab.com/libyuni/libyuni/
** github: https://github.com/libyuni/libyuni/ {mirror}
*/
#include "uuid.h"
#ifdef YUNI_OS_MACOS
#include <CoreFoundation/CFUUID.h>
#else
#ifndef YUNI_OS_WINDOWS
# include <uuid/uuid.h>
#else
# include <objbase.h>
#endif
#endif
#include "../core/string.h"
#include <cassert>
#include <iostream>


namespace Yuni
{

	namespace // anonymous
	{

		// converts a single hex char to a number (0 - 15)
		inline uchar hexDigitToChar(char ch)
		{
			if (ch > 47 && ch < 58)
				return static_cast<uchar>(ch - 48);
			if (ch > 96 && ch < 103)
				return static_cast<uchar>(ch - 87);
			if (ch > 64 && ch < 71)
				return static_cast<uchar>(ch - 55);
			return 0;
		}

		// converts the two hexadecimal characters to an unsigned char (a byte)
		inline uchar hexPairToChar(char a, char b)
		{
			  return static_cast<uchar>(hexDigitToChar(a) * 16 + hexDigitToChar(b));
		}

	} // anonymous


	void UUID::generate()
	{
		assert(sizeof(StorageType) == 16 and "Invalid storage size for uuid");

		#ifdef YUNI_OS_MACOS
		{
			CFUUIDRef uuid = CFUUIDCreate(nullptr);
			if (uuid)
			{
				CFUUIDBytes bytes = CFUUIDGetUUIDBytes(uuid);
				CFRelease(uuid);

				m_value.ubytes[0] =  bytes.byte0;
				m_value.ubytes[1] =  bytes.byte1;
				m_value.ubytes[2] =  bytes.byte2;
				m_value.ubytes[3] =  bytes.byte3;
				m_value.ubytes[4] =  bytes.byte4;
				m_value.ubytes[5] =  bytes.byte5;
				m_value.ubytes[6] =  bytes.byte6;
				m_value.ubytes[7] =  bytes.byte7;
				m_value.ubytes[8] =  bytes.byte8;
				m_value.ubytes[9] =  bytes.byte9;
				m_value.ubytes[10] = bytes.byte10;
				m_value.ubytes[11] = bytes.byte11;
				m_value.ubytes[12] = bytes.byte12;
				m_value.ubytes[13] = bytes.byte13;
				m_value.ubytes[14] = bytes.byte14;
				m_value.ubytes[15] = bytes.byte15;
				return;
			}
		}
		#else
		{
			#ifdef YUNI_OS_WINDOWS
			{
				GUID winguid;
				if (S_OK == ::CoCreateGuid(&winguid))
				{
					m_value.ubytes[0] = static_cast<uchar>((winguid.Data1 >> 24) & 0xFF);
					m_value.ubytes[1] = (winguid.Data1 >> 16) & 0xFF;
					m_value.ubytes[2] = (winguid.Data1 >> 8) & 0xFF;
					m_value.ubytes[3] = (winguid.Data1) & 0xFF;
					m_value.ubytes[4] = static_cast<uchar>((winguid.Data2 >> 8) & 0xFF);
					m_value.ubytes[5] = static_cast<uchar>((winguid.Data2) & 0xFF);
					m_value.ubytes[6] = static_cast<uchar>((winguid.Data3 >> 8) & 0xFF);
					m_value.ubytes[7] = static_cast<uchar>((winguid.Data3) & 0xFF);
					m_value.ubytes[8] = winguid.Data4[0];
					m_value.ubytes[9] = winguid.Data4[1];
					m_value.ubytes[10] = winguid.Data4[2];
					m_value.ubytes[11] = winguid.Data4[3];
					m_value.ubytes[12] = winguid.Data4[4];
					m_value.ubytes[13] = winguid.Data4[5];
					m_value.ubytes[14] = winguid.Data4[6];
					m_value.ubytes[15] = winguid.Data4[7];
					return;
				}
			}
			#else
			{
				assert(sizeof(uuid_t) == 16);
				return uuid_generate(m_value.ubytes);
			}
			#endif
		}
		#endif

		// fallback
		clear();
	}


	void UUID::writeToCString(char* out) const
	{
		constexpr const char* hex = "0123456789abcdef";

		const uchar* in = m_value.ubytes;
		for (uint32_t i = 0; i != 4; ++i)
		{
			*out++ = hex[(*in >> 4) & 0xF];
			*out++ = hex[(*in++) & 0xF];
		}
		*out++ = '-';

		for (uint32_t i = 4; i != 6; ++i)
		{
			*out++ = hex[(*in >> 4) & 0xF];
			*out++ = hex[(*in++) & 0xF];
		}
		*out++ = '-';

		for (uint32_t i = 6; i != 8; ++i)
		{
			*out++ = hex[(*in >> 4) & 0xF];
			*out++ = hex[(*in++) & 0xF];
		}
		*out++ = '-';

		for (uint32_t i = 8; i != 10; ++i)
		{
			*out++ = hex[(*in >> 4) & 0xF];
			*out++ = hex[(*in++) & 0xF];
		}
		*out++ = '-';

		for (uint32_t i = 10; i != 16; ++i)
		{
			*out++ = hex[(*in >> 4) & 0xF];
			*out++ = hex[(*in++) & 0xF];
		}
	}


	void UUID::assign(AnyString string)
	{
		string.trim();
		char charOne, charTwo;
		bool lookingForFirstChar = true;
		uint32_t bindex = 0;
		for (uint32_t i = 0; i != string.size(); ++i)
		{
			char ch = string[i];
			if (ch == '-' or ch == '{')
				continue;
			if (lookingForFirstChar)
			{
				charOne = ch;
				lookingForFirstChar = false;
			}
			else
			{
				charTwo = ch;
				m_value.ubytes[bindex++] = hexPairToChar(charOne, charTwo);
				lookingForFirstChar = true;
				if (bindex == 16)
					break;
			}
		}
	}


} // namespace Yuni


std::ostream& operator << (std::ostream& out, const Yuni::UUID& rhs)
{
	// WriteToubytes is guarantee to have 42 chars
	char ubytes[42];
	Yuni::Private::UUID::Helper::WriteToCString(ubytes, rhs);
	out.write(ubytes, 36);
	return out;
}
