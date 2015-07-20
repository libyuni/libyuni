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
#include <yuni/core/string.h>
#include <yuni/core/system/suspend.h>
#include <yuni/media/queueservice.h>

using namespace Yuni;


// Display the time in seconds using an MM:SS format
template<typename T>
static void writeTime(T timeInSeconds)
{
	std::cout << (timeInSeconds / 60) << ":";
	if (timeInSeconds % 60 < 10)
		std::cout << 0;
	std::cout << (timeInSeconds % 60);
}

template<>
void writeTime<float>(float timeInSeconds)
{
	std::cout << ((int)timeInSeconds / 60) << ":";
	if ((int)timeInSeconds % 60 < 10)
		std::cout << 0;
	std::cout << ((int)timeInSeconds % 60);
	int decimals = (int)((timeInSeconds - (uint)timeInSeconds) * 100);
	if (decimals >= 10)
		std::cout << '.' << decimals;
	else
		std::cout << ".0" << decimals;
}


int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " <file1> [ <file2>, ... ]" << std::endl;
		return 1;
	}

	// The media queue service is the center point for media manipulation
	Media::QueueService media;
	media.start();

	if (!media.running())
		return 1;

	// Generate a name for the emitter ...
	AnyString emitterName = "Emitter 1";
	// ... and create it
	if (!media.emitter.add(emitterName))
	{
		std::cerr << "Emitter creation failed !" << std::endl;
		return 1;
	}

	for (int i = 1; i < argc; ++i)
	{
		// Load sound file : using `loadSound` specifies we do not care about possible video content
		if (!media.library.loadSound(argv[i]))
		{
			std::cerr << "Failed to load \"" << argv[i] << "\"" << std::endl;
			// Play the next song rather than just quitting
			continue;
		}

		// Attach the emitter to the buffer
		media.emitter.attach(emitterName, argv[i]);
		// Start playback on the emitter
		media.emitter.play(emitterName);

		while (!media.playing())
			Yuni::SuspendMilliSeconds(50);

		std::cout << "Playing \"" << argv[i] << "\"" << std::endl;
		std::cout << media.library.samplingRate(argv[i]) << "Hz, "
				  << media.library.bitsPerSample(argv[i]) << " bits, "
				  << (media.library.channels(argv[i]) < 2 ? "mono" : "stereo") << std::endl;

		// Get stream duration
		uint duration = media.library.duration(argv[i]);

		float elapsed = 0;
		// Keep playing while at least one emitter is playing
		while (media.playing())
		{
			// Every 100ms
			Yuni::SuspendMilliSeconds(100);

			// Get elapsed playback time
			float newTime = media.library.elapsedTime(argv[i]);
			// Only update if different
			if (newTime > elapsed)
			{
				// Display elapsed time
				elapsed = newTime;
				std::cout << "\rTime : ";
				writeTime(elapsed);
				std::cout << " / ";
				writeTime(duration);
			}
		}
		media.emitter.detach(emitterName);
		media.library.unload(argv[i]);
	}

	// Properly stop the queue service and unload everything
	media.stop();
	return 0;
}

