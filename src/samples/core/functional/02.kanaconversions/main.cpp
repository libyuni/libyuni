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
#include <yuni/yuni.h>
#include <yuni/core/functional/view.h>
#include <yuni/core/string.h>
#include <iostream>

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
