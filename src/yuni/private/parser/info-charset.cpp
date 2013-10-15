
#include "info-charset.h"
#include <string.h>

#define CHARSER_ADD_SINGLE_CHAR(C) \
		do \
		{ \
			if (C.size() == 1) \
				Bit::Set(ascii, (unsigned char) C); \
			else \
				utf8chars.insert(C); \
		} \
		while (0)



namespace Yuni
{
namespace Private
{
namespace Parser
{

	CharsetInfo::CharsetInfo() :
		excluded(false)
	{
		::memset(ascii, '\0', sizeof(ascii));
	}


	CharsetInfo::CharsetInfo(const CharsetInfo& rhs) :
		excluded(rhs.excluded),
		utf8chars(rhs.utf8chars),
		utf8ranges(rhs.utf8ranges)
	{
		::memcpy(ascii, rhs.ascii, sizeof(ascii));
	}


	CharsetInfo& CharsetInfo::operator = (const CharsetInfo& rhs)
	{
		excluded = rhs.excluded;
		utf8chars = rhs.utf8chars;
		utf8ranges = rhs.utf8ranges;
		::memcpy(ascii, rhs.ascii, sizeof(ascii));
		return *this;
	}


	void CharsetInfo::add(UTF8::Char c)
	{
		CHARSER_ADD_SINGLE_CHAR(c);
	}



	void CharsetInfo::add(const AnyString& listofchars)
	{
		AnyString::const_utf8iterator end = listofchars.utf8end();
		for (AnyString::const_utf8iterator i = listofchars.utf8begin(); i != end; ++i)
			add(*i);
	}


	void CharsetInfo::addRange(UTF8::Char from, UTF8::Char to)
	{
		if (from.value() <= to.value())
		{
			if (from.value() != to.value())
			{
				if (from.value() < UTF8::Char::asciiLimit)
				{
					if (to.value() < UTF8::Char::asciiLimit)
					{
						for (uint i = from.value(); i <= to.value(); ++i)
							Bit::Set(ascii, i);
					}
					else
					{
						for (uint i = from.value(); i <= UTF8::Char::asciiLimit - 1; ++i)
							Bit::Set(ascii, i);
						utf8ranges.push_back(UTF8Range(UTF8::Char(UTF8::Char::asciiLimit), to));
					}
				}
				else
					utf8ranges.push_back(UTF8Range(from, to));
			}
			else
				CHARSER_ADD_SINGLE_CHAR(from);
		}
		else
		{
			// error - invalid range
		}
	}



	void CharsetInfo::addWhitespaces()
	{
		Bit::Set(ascii, (uint)' ');
		Bit::Set(ascii, (uint)'\t');
		Bit::Set(ascii, (uint)'\r');
		Bit::Set(ascii, (uint)'\n');
		Bit::Set(ascii, (uint)'\v');
	}


	void CharsetInfo::addLetters()
	{
		for (uint i = (uint)'a'; i <= (uint)'z'; ++i)
			Bit::Set(ascii, i);

		for (uint i = (uint)'A'; i <= (uint)'Z'; ++i)
			Bit::Set(ascii, i);
	}


	void CharsetInfo::addNumbers()
	{
		for (uint i = (uint)'0'; i <= (uint)'9'; ++i)
			Bit::Set(ascii, i);
	}


	bool CharsetInfo::contains(const UTF8::Char& c) const
	{
		if (c.isAscii()) // < asciiLimit, aka 0x80
		{
			return Bit::Get(ascii, c.value()) ? not excluded : excluded;
		}
		else
		{
			if (utf8chars.count(c) > 0)
			{
				return not excluded;
			}
			else
			{
				for (uint i = 0; i != utf8ranges.size(); ++i)
				{
					if (utf8ranges[i].first <= c)
					{
						if (utf8ranges[i].second <= c)
							return not excluded;
					}
				}
				return excluded;
			}
		}
	}



	void CharsetInfo::addAlphaNumeric()
	{
		addLetters();
		addNumbers();
	}


	void CharsetInfo::clear()
	{
		excluded = false;
		::memset(ascii, '\0', sizeof(ascii));
		utf8chars.clear();
		utf8ranges.clear();
	}



} // namespace Parser
} // namespace Private
} // namespace Yuni
