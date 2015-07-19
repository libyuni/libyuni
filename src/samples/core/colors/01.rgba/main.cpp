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
#include <yuni/core/color/rgba.h>


using namespace Yuni;

int main(void)
{
	// Only two types should be used : uint8 and float. The other ones are not recommended

	// Standard colors when using an uint8.
	// The limits are : 0..255 (`0` means 0%, `255` means 100%).
	Color::RGBA<uint8> b_red(   255, 0,   0,   255);
	Color::RGBA<uint8> b_green( 0,   255, 0); // By default, a color is opaque
	Color::RGBA<uint8> b_blue(  0,   0,   255, 255);

	// The same colors when using a float (OpenGL uses float for the colors)
	// The limits are : 0..1 (`0.0f` means 0%, `1.0f` means 100%)
	Color::RGBA<float> f_red(   1.0f,  0.0f, 0.0f, 1.0f);
	Color::RGBA<float> f_green( 0.0f,  1.0f, 0.0f); // By default, a color is opaque
	Color::RGBA<float> f_blue(  0.0f,  0.0f, 1.0f, 1.0f);

	// A color completely transparent
	Color::RGBA<float> f_transparent(0.3f, 0.72f, 0.2345f, 0.3f);
	std::cout << "Transparent : " << f_transparent << std::endl;

	// Conversions
	Color::RGBA<float> f_convert (Color::RGBA<uint8>(242, 24, 183, 255));
	std::cout << "Convert 1 : " << f_convert << std::endl;

	Color::RGBA<uint8> f_convert2 (Color::RGBA<float>(0.1f, 0.9f, 0.24f, 0.3f));
	std::cout << "Convert 2 : " << f_convert2 << std::endl;
	Color::RGBA<uint8> f_convert3 (Color::RGBA<float>(0.1f, 0.9f, 0.24f, 0.3f));
	std::cout << "Convert 3 : " << f_convert3 << std::endl;

	// Compare 2 color models
//	std::cout << (f_red == f_green) << std::endl;
#warning Fix RGB<uint8> <> RGB<float> comparisons
	//std::cout << (f_convert2 == f_convert) << std::endl;

	return 0;
}
