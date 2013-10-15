/*!
** This sample demonstrates basic audio playing abilities.
** It takes file names as command line arguments and plays their audio streams one after the other.
** To demonstrate that the main loop is free to do something else, the sample displays elapsed time
** for the first file.
** This sample accepts audio streams from either audio or video files indifferently.
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
			Yuni::SuspendMilliSeconds(100);

		std::cout << "Playing \"" << argv[i] << "\"" << std::endl;

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

