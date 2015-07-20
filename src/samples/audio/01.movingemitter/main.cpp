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
#include <vector>
#include <yuni/yuni.h>
#include <yuni/media/queueservice.h>
#include <yuni/core/system/suspend.h>
#include <yuni/core/math.h>

using namespace Yuni;





int main(int argc, char* argv[])
{
	// Trivial command line parsing
	if (argc < 2)
	{
		std::cout << "usage: " << argv[0] << " <filename>" << std::endl;
		return EXIT_FAILURE;
	}
	AnyString filename = argv[1];


	// Media manager
	Media::QueueService media;
	media.start();

	if (!media.running())
		return 1;

	static const float LIMIT = 150.0f;

	// adding a new emitter
	AnyString emitterName("Emitter");
	if (not media.emitter.add(emitterName))
	{
		std::cerr << "Emitter creation failed !" << std::endl;
		return EXIT_FAILURE;
	}

	// loading the sound in the sound library
	if (not media.library.load(filename))
	{
		std::cerr << "impossible to load " << filename << std::endl;
		return EXIT_FAILURE;
	}

	// attaching it to our emitter
	media.emitter.attach(emitterName, filename);
	media.emitter.play(emitterName);

	if (media.library.channels(filename) > 1)
		std::cerr << "Warning : 3D sound only works with mono sources !" << std::endl;

	// emitter position
	Point3D<> position; // [0, 0, 0]
	bool inverse = false;
	position.y = -LIMIT;
	media.emitter.move(emitterName, position);

	for (uint i = 0; i < 2000; ++i) // arbitrary
	{
		// pause
		SuspendMilliSeconds(50); // milliseconds (arbitrary)

		// Moving the emitter around us
		position.y += 1.0f * (inverse ? -1.0f : 1.0f);

		if (!inverse && position.y > LIMIT)
		{
			inverse = true;
		}
		else
		{
			if (inverse && position.y < -LIMIT)
				inverse = false;
		}
		media.emitter.move(emitterName, position);
	}

	return 0;
}

