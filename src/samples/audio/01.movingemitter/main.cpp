
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

	static const float LIMIT = 300.0f;

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

