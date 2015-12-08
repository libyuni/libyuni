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
#ifndef __YUNI_MEDIA_QUEUESERVICE_H__
# define __YUNI_MEDIA_QUEUESERVICE_H__

# include "../yuni.h"
# include "../core/string.h"
# include "emitter.h"
# include "loop.h"
# include "source.h"


namespace Yuni
{
namespace Media
{

	/*!
	** \brief The audio queue service is the service that manages everything sound-related
	**
	** It takes care of ffmpeg / openal inits.
	** It uses an event loop to solve MT problems.
	*/
	class QueueService: public Policy::ObjectLevelLockable<QueueService>
	{
	public:
		//! The threading policy
		typedef Policy::ObjectLevelLockable<QueueService>  ThreadingPolicy;

	public:
		//! Forward declaration
		class Library;

		/*!
		** \brief This is the entry point to all the emitters for this queue service
		**
		** All the emitters are managed here.
		** It can be accessed through: queueService.emitter
		*/
		class Emitters: public Policy::ObjectLevelLockable<Emitters>
		{
		public:
			typedef Policy::ObjectLevelLockable<Emitters>  ThreadingPolicy;

		private:
			Emitters()
			{}
			Emitters(const Emitters&);

		public:
			//! Get an emitter
			Emitter::Ptr get(const AnyString& name) const;

			//! Add an emitter
			bool add(const AnyString& name);

			//! Attach an emitter to a source
			bool attach(const AnyString& name, const AnyString& attachedSource);
			//! Attach an emitter to a source
			bool attach(Emitter::Ptr& emitter, const AnyString& attachedSource);
			//! Attach an emitter to a source
			bool attach(const AnyString& name, Source::Ptr attachedSource);
			//! Attach an emitter to a source
			bool attach(Emitter::Ptr& emitter, Source::Ptr attachedSource);

			//! Detach an emitter from any source
			void detach(const AnyString& name);
			//! Detach an emitter from any source
			void detach(Emitter::Ptr name);

			//! Move an emitter around
			bool move(const AnyString& name, const Point3D<>& position);
			//! Move an emitter around
			bool move(Emitter::Ptr emitter, const Point3D<>& position);
			//! Move an emitter around
			bool move(const AnyString& name, const Point3D<>& position,
				const Vector3D<>& velocity, const Vector3D<>& direction);
			//! Move an emitter around
			bool move(Emitter::Ptr emitter, const Point3D<>& position,
				const Vector3D<>& velocity, const Vector3D<>& direction);

			//! Get elapsed playback time on an emitter
			float elapsedTime(const AnyString& name);
			//! Get elapsed playback time on an emitter
			float elapsedTime(Emitter::Ptr emitter);

			//! Get whether an emitter is playing
			bool playing(const AnyString& name) const;
			//! Get whether an emitter is playing
			bool playing(Emitter::Ptr emitter) const;

			//! Get whether an emitter is paused
			bool paused(const AnyString& name) const;
			//! Get whether an emitter is paused
			bool paused(Emitter::Ptr emitter) const;

			//! Start playback on an emitter
			bool play(const AnyString& name);
			//! Start playback on an emitter
			bool play(Emitter::Ptr emitter);

			//! Pause playback on an emitter
			bool pause(const AnyString& name);
			//! Pause playback on an emitter
			bool pause(Emitter::Ptr emitter);

			//! Stop playback on an emitter
			bool stop(const AnyString& name);
			//! Stop playback on an emitter
			bool stop(const Emitter::Ptr& emitter);

			//! Remove an emitter
			bool remove(const AnyString& name);
			//! Remove an emitter
			bool remove(Emitter::Ptr name);

		private:
			//! Friend declaration
			friend class QueueService;

			//! Map of currently registered emitters, with string tags as keys
			Emitter::Map pEmitters;

			//! Associated queue service
			QueueService* pQueueService;
			//! Associated library
			Library* pLibrary;
		};




		/*!
		** \brief The Library contains the media sources currently loaded in the queue service
		**
		** It can be accessed through: queueService.library
		*/
		class Library: public Policy::ObjectLevelLockable<Library>
		{
		public:
			typedef Policy::ObjectLevelLockable<Library>  ThreadingPolicy;

		private:
			//! \name Constructors
			//@{
			//! Empty constructor
			Library()
			{}

			//! Copy constructor
			Library(const Library&);
			//@}

		public:
			//! Clear the library, free the loaded sources
			void clear();

			/*!
			** \brief Load media file from given path
			**
			** \param name Path to file, used from now on as an identifier for the source
			**
			** \note Automatic stream detection is performed, if video and audio are found, both streams are loaded
			*/
			bool load(const AnyString& name);

			/*!
			** \brief Load audio file from given path
			**
			** \param name Path to file, used from now on as an identifier for the source
			**
			** \note Only the first found audio stream is loaded, even on video files
			*/
			bool loadSound(const AnyString& name);

			/*!
			** \brief Load audio file from given path
			**
			** \param name Path to file, used from now on as an identifier for the source
			**
			** \note Only the first found video stream is loaded, not the audio if any
			*/
			bool loadVideo(const AnyString& name);

			/*!
			** \brief Load media file from given path
			**
			** \param name Identifier for the source
			*/
			bool unload(const AnyString& name);

			//! Get the duration of a source
			uint duration(const AnyString& name) const;

			//! Get the sampling rate of a source
			uint samplingRate(const AnyString& name) const;

			//! Get the number of channels of a source
			uint channels(const AnyString& name) const;

			//! Get the number of bits per sample of a source
			uint bitsPerSample(const AnyString& name) const;

			//! Get elapsed time (current playing time) in the source
			float elapsedTime(const AnyString& name) const;

			//! Get a source from the library
			Source::Ptr get(const AnyString& name);
			const Source::Ptr get(const AnyString& name) const;

		private:
			//! Friend declaration
			friend class QueueService;
			//! Friend declaration
			friend class QueueService::Emitters;

			//! Map of currently loaded sources, with string tags as keys
			Source::Map pSources;

			//! Associated queue service
			QueueService* pQueueService;
		};




	public:
		//! \name Constructor and destructor
		//@{
		//! Constructor
		QueueService();

		//! Destructor
		~QueueService();
		//@}

	public:
		/*!
		** \brief Start the audio service
		*/
		bool start();

		/*!
		** \brief Stop the audio service
		*/
		void stop();

		/*!
		** \brief Is the audio service running ?
		*/
		bool running() const;

		/*!
		** \brief Is there at least one emitter playing ?
		*/
		bool playing() const;

	public:
		//! Control block for emitters
		Emitters emitter;
		//! Control block for audio sources
		Library library;

	private:
		//! \name Private operations
		//@{
		/*!
		** \brief Copy constructor
		*/
		QueueService(const QueueService&);

		/*!
		** \brief Assignment
		*/
		QueueService& operator = (const QueueService&);
		//@}


	private:
		//! This is meant to aggregate a condition and a boolean for dispatching
		struct InitData
		{
			InitData(Thread::Signal& s, bool& r): signal(s), ready(r) {}

			Thread::Signal& signal;
			bool& ready;
		};

		//! This is meant to aggregate a condition with the emitter
		struct EmitterPositionData
		{
			EmitterPositionData(Thread::Signal& s, float& f): signal(s), data(f) {}

			Thread::Signal& signal;
			float& data;
		};


	private:
		/*!
		** \brief Media initialization
		**
		** \note Dispatched in the media loop
		*/
		bool initDispatched(InitData& initData);

		/*!
		** \brief Source loading
		**
		** \note Dispatched in the media loop
		*/
		bool loadSource(const String& filePath, bool video, bool audio, bool strict);

		/*!
		** \brief Emitter and source update
		**
		** \note Called in the Media::Loop::onLoop()
		*/
		bool updateDispatched();

	private:
		//! Has the manager been properly started ?
		bool pReady;
		//! Event loop for media events
		Loop pMediaLoop;

	private:
		//! Friend declaration
		friend class Loop;
		//! Friend declaration
		friend class Emitters;
		//! Friend declaration
		friend class Library;

	}; // class QueueService




} // namespace Media
} // namespace Yuni

#include "queueservice.hxx"

#endif // __YUNI_MEDIA_QUEUESERVICE_H__
