#ifndef __YUNI_PARSER_CHARSET_HXX__
# define __YUNI_PARSER_CHARSET_HXX__


namespace Yuni
{
namespace Parser
{

	inline Charset::Charset() :
		pList(nullptr),
		pIndex((uint) -1)
	{}

	inline Charset::Charset(Charsets* list, uint index) :
		pList(list),
		pIndex(index)
	{}


	inline void Charset::add(UTF8::Char c)
	{
		assert(pList and "invalid pointer to charsets");
		assert(pIndex < pList->size() and "invalid index");
		pList->pCharsets[pIndex].add(c);
	}


	inline void Charset::add(const AnyString& listofchars)
	{
		assert(pList and "invalid pointer to charsets");
		assert(pIndex < pList->size() and "invalid index");
		pList->pCharsets[pIndex].add(listofchars);
	}


	inline void Charset::addRange(UTF8::Char from, UTF8::Char to)
	{
		assert(pList and "invalid pointer to charsets");
		assert(pIndex < pList->size() and "invalid index");
		pList->pCharsets[pIndex].addRange(from, to);
	}


	inline bool Charset::contains(const UTF8::Char& c) const
	{
		assert(pList and "invalid pointer to charsets");
		assert(pIndex < pList->size() and "invalid index");
		return pList->pCharsets[pIndex].contains(c);
	}


	inline void Charset::addWhitespaces()
	{
		assert(pList and "invalid pointer to charsets");
		assert(pIndex < pList->size() and "invalid index");
		pList->pCharsets[pIndex].addWhitespaces();
	}


	inline void Charset::addLetters()
	{
		assert(pList and "invalid pointer to charsets");
		assert(pIndex < pList->size() and "invalid index");
		pList->pCharsets[pIndex].addLetters();
	}


	inline void Charset::addNumbers()
	{
		assert(pList and "invalid pointer to charsets");
		assert(pIndex < pList->size() and "invalid index");
		pList->pCharsets[pIndex].addNumbers();
	}


	inline void Charset::addAlphaNumeric()
	{
		assert(pList and "invalid pointer to charsets");
		assert(pIndex < pList->size() and "invalid index");
		pList->pCharsets[pIndex].addAlphaNumeric();
	}


	inline Charset& Charset::operator = (const AnyString& listofchars)
	{
		assert(pList and "invalid pointer to charsets");
		assert(pIndex < pList->size() and "invalid index");
		Yuni::Private::Parser::CharsetInfo& info = pList->pCharsets[pIndex];
		info.clear();
		info.add(listofchars);
		return *this;
	}


	inline Charset& Charset::operator ^= (const AnyString& listofchars)
	{
		assert(pList and "invalid pointer to charsets");
		assert(pIndex < pList->size() and "invalid index");
		Yuni::Private::Parser::CharsetInfo& info = pList->pCharsets[pIndex];
		info.clear();
		info.add(listofchars);
		info.excluded = true;
		return *this;
	}


	inline Charset& Charset::operator += (const AnyString& listofchars)
	{
		assert(pList and "invalid pointer to charsets");
		assert(pIndex < pList->size() and "invalid index");
		pList->pCharsets[pIndex].add(listofchars);
		return *this;
	}


	inline Charset& Charset::operator |= (const AnyString& listofchars)
	{
		assert(pList and "invalid pointer to charsets");
		assert(pIndex < pList->size() and "invalid index");
		pList->pCharsets[pIndex].add(listofchars);
		return *this;
	}





} // namespace Parser
} // namespace Yuni

#endif // __YUNI_PARSER_CHARSET_H__
