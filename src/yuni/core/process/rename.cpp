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
#include "rename.h"
#include "../../thread/mutex.h"

#ifdef YUNI_OS_LINUX
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <limits.h>
# include <sys/user.h>
# include <sys/prctl.h>
#endif




namespace Yuni
{
namespace Process
{


	# ifdef YUNI_OS_LINUX

	namespace // anonymous
	{

		static char **Argv    = NULL;   // pointer to argument vector
		static char *LastArgv = NULL;   // end of argv
		//static char Argv0[128];       // program name


		static inline void LinuxInitializeRenameProcess(int argc, char **argv, char **envp)
		{
			uint i = 0;
			for (; envp[i] != NULL; ++i)
				continue;
			__environ = (char **) malloc(sizeof (char *) * (i + 1));

			for (i = 0; envp[i] != NULL; ++i)
				__environ[i] = strdup(envp[i]);
			__environ[i] = nullptr;

			Argv = argv;
			LastArgv = (i > 0)
				? envp[i - 1] + strlen(envp[i - 1])
				: argv[argc - 1] + strlen(argv[argc - 1]);

			char* tmp = strrchr(argv[0], '/');
			if (!tmp)
				tmp = argv[0];
			else
				++tmp;

			//strncpy(Argv0, tmp, sizeof(Argv0));
			//Argv0[sizeof(Argv0) - 1] = 0;
		}


		static inline void LinuxRenameProcess(const char* text, uint size)
		{
			// TODO Remove this static variable into a dynamicaly allocated one
			enum
			{
				initialBufferSize = 2048,
			};
			static char buffer[initialBufferSize];

			memset(buffer, '\0', sizeof(buffer));
			char* p = buffer;

			uint buffersize = (uint) (sizeof(buffer) - (size_t)(p - buffer) - 1);
			if (size > buffersize)
				size = buffersize;
			YUNI_MEMCPY(p, initialBufferSize, text, size);
			p[size] = '\0';

			for (uint i = 0; Argv[i]; ++i)
				memset(Argv[i], '\0', strlen(Argv[i]));

			(void) strcpy(Argv[0], buffer);
			Argv[1] = nullptr;
		}

	} // anonymous namespace








	char** Rename(int argc, char** argv, const AnyString& newprocessname)
	{
		// Global locking for multithreading
		static Yuni::Mutex globalMutex;
		Yuni::MutexLocker locker(globalMutex);

		static bool initialized = false;
		if (not initialized)
		{
			LinuxInitializeRenameProcess(argc, argv, environ);
			initialized = true;
		}
		LinuxRenameProcess(newprocessname.c_str(), newprocessname.size());
		return argv;
	}



	# else




	char** Rename(int /*argc*/, char** argv, const AnyString& /*newprocessname*/)
	{
		// does nothing
		return argv;
	}

	# endif






} // namespace Process
} // namespace Yuni

