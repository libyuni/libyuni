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
#ifndef __YUNI_MEDIA_EMITTER_H__
# define __YUNI_MEDIA_EMITTER_H__

# include "../yuni.h"
# include <map>
# include "../core/string.h"
# include "../core/point3D.h"
# include "../core/vector3D.h"
# include "source.h"


namespace Yuni
{
namespace Media
{

	/*!
	** \brief An emitter is a virtual object from which a source is played
	**
	** Emitters can be placed in space, and be moved around.
	*/
	class Emitter: public Policy::ObjectLevelLockable<Emitter>
	{
	public:
		//! The most suitable smart pointer for the class
		typedef SmartPtr<Emitter> Ptr;
		//! Threading Policy
		typedef Policy::ObjectLevelLockable<Emitter> ThreadingPolicy;
		//! Map
		typedef std::map<String, Ptr> Map;

	public:
		//! Default value for pitch (1.0)
		static const float DefaultPitch;
		//! Default value for gain (1.0)
		static const float DefaultGain;
		enum
		{
			//! Default value for attenuation (enabled)
			DefaultAttenuation = true,
			//! Default value for looping (false)
			DefaultLooping = false
		};

	public:
		//! \name Constructors & Destructor
		//@{
		/*!
		** \brief Shortest constructor
		**
		** Position, speed and direction default to (0,0,0)
		*/
		Emitter(bool loop = DefaultLooping);

		/*!
		** \brief Constructor with 3D position
		**
		** Speed and velocity default to (0,0,0)
		*/
		Emitter(const Point3D<>& position, bool loop);

		/*!
		** \brief Constructor with position, velocity and direction
		*/
		Emitter(const Point3D<>& position, const Vector3D<>& velocity,
			const Vector3D<>& direction, bool loop);

		/*!
		** \brief Destructor
		*/
		~Emitter() {}
		//@}


		//! \name Methods
		//@{
		//! Attach a source to the emitter
		bool attachSourceDispatched(Source::Ptr& source);

		//! Detach the emitter from any source
		bool detachSourceDispatched();

		//! Prepare the emitter for playing
		bool prepareDispatched();

		//! Play the source
		bool playSourceDispatched();
		//! Play the source
		bool playSourceDispatched(Source::Ptr& source);

		//! Stop the source
		bool stopSourceDispatched();

		//! Pause the source
		bool pauseSourceDispatched();

		//! Update sources if necessary
		bool updateDispatched();
		//@}


		//! \name Accessors
		//@{
		//! Is the emitter attached to a source ?
		bool attached() const;

		//! Is the emitter currently playing ?
		bool playing() const;

		//! Is the emitter currently playing ?
		bool paused() const;

		//! Set the 3D position of the emitter
		void position(const Point3D<>& position);
		//! Get the 3D position of the emitter
		Point3D<> position() const;

		//! Set the velocity of the emitter
		void velocity(const Vector3D<>& position);
		//! Get the velocity of the emitter
		Vector3D<> velocity() const;

		//! Set the direction of the emitter
		void direction(const Vector3D<>& position);
		//! Get the direction of the emitter
		Vector3D<> direction() const;

		//! Set whether the emitter should loop
		void loop(bool looping);
		//! Get whether the emitter loops
		bool loop() const;

		/*!
		** \brief Set the volume modifier on the emitter
		** \param newGain 0.0f for no sound, 1.0f to keep sound as is, > 1.0f to amplify sound
		*/
		//! Set the volume modifier
		void gain(float newGain);
		//! Get the current volume modifier
		float gain() const;

		/*!
		** \brief Get the elapsed playback time (in seconds)
		** \returns Time elapsed since the emitter started playback. 0 if not playing.
		*/
		float elapsedTime() const;

		//! Get the identifier for the emitter
		uint id() const;
		//@}


	private:
		Emitter(const Emitter&);
		Emitter& operator= (const Emitter&);

	private:
		//! String identifier for the emitter
		String pName;
		//! OpenAL identifier for the emitter
		unsigned int pID;
		//! Position of the emitter in space
		Point3D<> pPosition;
		//! Speed of the emitter
		Vector3D<> pVelocity;
		//! Direction of the movement of the emitter
		Vector3D<> pDirection;
		//! Should the emitter loop on itself when finished playing?
		bool pLoop;
		//! Volume modifier, 1.0 means no modification
		float pGain;
		//! Current playback position
		sint64 pStartTime;
		//! Sound to play. nullptr if none
		Source::Ptr pSource;

		//! Is the emitter ready for use?
		bool pReady;
		//! Is the emitter currently playing?
		bool pPlaying;
		//! Is the emitter currently paused?
		bool pPaused;
		//! Has the emitter's values been modified ?
		bool pModified;

	}; // class Emitter




} // namespace Media
} // namespace Yuni

# include "emitter.hxx"

#endif // __YUNI_MEDIA_EMITTER_H__
