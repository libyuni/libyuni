/*
** YUNI's default license is the GNU Lesser Public License (LGPL), with some
** exclusions (see below). This basically means that you can get the full source
** code for nothing, so long as you adhere to a few rules.
**
** Under the LGPL you may use YUNI for any purpose you wish, and modify it if you
** require, as long as you:
**
** Pass on the (modified) YUNI source code with your software, with original
** copyrights intact :
**  * If you distribute electronically, the source can be a separate download
**    (either from your own site if you modified YUNI, or to the official YUNI
**    website if you used an unmodified version) – just include a link in your
**    documentation
**  * If you distribute physical media, the YUNI source that you used to build
**    your application should be included on that media
** Make it clear where you have customised it.
**
** In addition to the LGPL license text, the following exceptions / clarifications
** to the LGPL conditions apply to YUNI:
**
**  * Making modifications to YUNI configuration files, build scripts and
**    configuration headers such as yuni/platform.h in order to create a
**    customised build setup of YUNI with the otherwise unmodified source code,
**    does not constitute a derived work
**  * Building against YUNI headers which have inlined code does not constitute a
**    derived work
**  * Code which subclasses YUNI classes outside of the YUNI libraries does not
**    form a derived work
**  * Statically linking the YUNI libraries into a user application does not make
**    the user application a derived work.
**  * Using source code obsfucation on the YUNI source code when distributing it
**    is not permitted.
** As per the terms of the LGPL, a "derived work" is one for which you have to
** distribute source code for, so when the clauses above define something as not
** a derived work, it means you don't have to distribute source code for it.
** However, the original YUNI source code with all modifications must always be
** made available.
*/
#include <iconv.h>
#include "charset.h"

namespace Yuni
{
namespace Charset
{

	Converter::Converter(Charset::Type fromCS, Charset::Type toCS, Charset::Unconvertable opts)
		:pContext((iconv_t)-1), pLastError(0)
	{
		// 64 Should be sufficient to hold ISO-8859-15//IGNORE//TRANSLIT
		char toBuf[64];

		strcpy(toBuf, Charset::TypeAsString[(int)toCS]);

		if (opts == Charset::uncTranslit || opts == Charset::uncTranslitIgnore)
			strcat(toBuf, "//TRANSLIT");

		if (opts == Charset::uncTranslitIgnore || opts == Charset::uncIgnore)
			strcat(toBuf, "//IGNORE");

		// Open the conversion context using our generated charset strings.
		open(Charset::TypeAsString[(int)fromCS], toBuf);
	}


	Converter::Converter(const char* fromCS, const char* toCS)
		: pContext((void*)-1), pLastError(0)
	{
		open(fromCS, toCS);
	}


	Converter::Converter()
		: pContext((void*)-1), pLastError(0)
	{}


	Converter::~Converter()
	{
		if (valid())
			iconv_close((iconv_t)pContext);
		// pContext = -1; This object will be destroyed anyways.
	}


	void Converter::close()
	{
		if (!valid())
			return;
		iconv_close((iconv_t)pContext);
		pContext = (iconv_t)-1;
	}


	void Converter::reset()
	{
		if (!valid())
			return;
		iconv((iconv_t)pContext, NULL, NULL, NULL, NULL);
	}


	const char * Converter::Name(Charset::Type type)
	{
		return Charset::TypeAsString[(int)type];
	}


	bool Converter::open(const char* from, const char* to)
	{
		pContext = (void *)iconv_open(to, from);
		bool isValid = valid();
		if (!isValid)
			pLastError = errno;
		return isValid;
	}


	size_t Converter::IconvWrapper(void* cd, char **inbuf, size_t* inbytesleft,
		char** outbuf, size_t* outbytesleft)
	{
		return iconv((iconv_t)cd, inbuf, inbytesleft, outbuf, outbytesleft);
	}





} // namespace Charset
} // namespace Yuni
