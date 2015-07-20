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
#include <yuni/yuni.h>
#include <yuni/core/functional/view.h>
#include <yuni/core/string.h>


using namespace Yuni;



// Romanize Japanese kana from UTF8-Char
static void romanizeAndAppend(const UTF8::Char& kana, String& romanization)
{
	if (kana == "あ" || kana == "ア")
		romanization << "a";
	else if (kana == "い" || kana == "イ")
		romanization << "i";
	else if (kana == "う" || kana == "ウ")
		romanization << "u";
	else if (kana == "た" || kana == "タ")
		romanization << "ta";
	else if (kana == "と" || kana == "ト")
		romanization << "to";
	else if (kana == "ど" || kana == "ド")
		romanization << "do";
	else if (kana == "ま" || kana == "マ")
		romanization << "ma";
	else if (kana == "も" || kana == "モ")
		romanization << "mo";
	else if (kana == "り" || kana == "リ")
		romanization << "ri";
	else if (kana == "す" || kana == "ス")
		romanization << "su";
	else if (kana == "し" || kana == "シ")
		romanization << "shi";
	else if (kana == "ざ" || kana == "ザ")
		romanization << "za";
	else if (kana == "か" || kana == "カ")
		romanization << "ka";
	else if (kana == "が" || kana == "ガ")
		romanization << "ga";
	else if (kana == "こ" || kana == "コ")
		romanization << "ko";
	else if (kana == "ご" || kana == "ゴ")
		romanization << "go";
	else
		romanization << kana;
}



// Functional::View works on Strings and allows for nice UTF-8 management
int main()
{
	String str("どうもありがとうございました");

	String romanized;
	makeView(str.utf8begin(), str.utf8end()).each([&](const UTF8::Char& c) -> bool
	{
		romanizeAndAppend(c, romanized);
		return true;
	});
	std::cout << romanized << std::endl;

	return 0;
}
