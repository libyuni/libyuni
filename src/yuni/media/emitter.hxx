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
#ifndef __YUNI_MEDIA_EMITTER_HXX__
# define __YUNI_MEDIA_EMITTER_HXX__

# include "../yuni.h"

namespace Yuni
{
namespace Media
{

	inline Emitter::Emitter(bool loop):
		pLoop(loop),
		pGain(DefaultGain),
		pSource(nullptr),
		pReady(false),
		pPlaying(false),
		pPaused(false),
		pModified(false)
	{}


	inline Emitter::Emitter(const Point3D<>& position, bool loop):
		pPosition(position),
		pLoop(loop),
		pGain(DefaultGain),
		pSource(nullptr),
		pReady(false),
		pPlaying(false),
		pPaused(false),
		pModified(false)
	{}


	inline Emitter::Emitter(const Point3D<>& position, const Vector3D<>& velocity,
		const Vector3D<>& direction, bool loop = false):
		pPosition(position),
		pVelocity(velocity),
		pDirection(direction),
		pLoop(loop),
		pGain(DefaultGain),
		pSource(nullptr),
		pReady(false),
		pPlaying(false),
		pPaused(false),
		pModified(false)
	{}


	inline bool Emitter::attached() const
	{
		ThreadingPolicy::MutexLocker locker(*this);
		return !(!pSource);
	}


	inline bool Emitter::playing() const
	{
		ThreadingPolicy::MutexLocker locker(*this);
		return pPlaying;
	}


	inline bool Emitter::paused() const
	{
		ThreadingPolicy::MutexLocker locker(*this);
		return pPaused;
	}


	inline void Emitter::position(const Point3D<>& position)
	{
		ThreadingPolicy::MutexLocker locker(*this);
		if (pPosition != position)
		{
			pPosition = position;
			pModified = true;
		}
	}


	inline Point3D<> Emitter::position() const
	{
		ThreadingPolicy::MutexLocker locker(*this);
		return pPosition;
	}


	inline void Emitter::velocity(const Vector3D<>& velocity)
	{
		ThreadingPolicy::MutexLocker locker(*this);
		if (pVelocity != velocity)
		{
			pVelocity = velocity;
			pModified = true;
		}
	}


	inline Vector3D<> Emitter::velocity() const
	{
		ThreadingPolicy::MutexLocker locker(*this);
		return pVelocity;
	}


	inline void Emitter::direction(const Vector3D<>& direction)
	{
		ThreadingPolicy::MutexLocker locker(*this);
		if (pDirection != direction)
		{
			pDirection = direction;
			pModified = true;
		}
	}


	inline Vector3D<> Emitter::direction() const
	{
		ThreadingPolicy::MutexLocker locker(*this);
		return pDirection;
	}


	inline void Emitter::loop(bool looping)
	{
		ThreadingPolicy::MutexLocker locker(*this);
		if (pLoop != looping)
		{
			pLoop = looping;
			pModified = true;
		}
	}

	inline bool Emitter::loop() const
	{
		ThreadingPolicy::MutexLocker locker(*this);
		return pLoop;
	}


	inline void Emitter::gain(float newGain)
	{
		ThreadingPolicy::MutexLocker locker(*this);
		if (!Math::Equals(pGain, newGain))
		{
			pGain = newGain;
			pModified = true;
		}
	}


	inline float Emitter::gain() const
	{
		ThreadingPolicy::MutexLocker locker(*this);
		return pGain;
	}


	inline unsigned int Emitter::id() const
	{
		ThreadingPolicy::MutexLocker locker(*this);
		return pID;
	}




} // namespace Media
} // namespace Yuni

#endif // __YUNI_MEDIA_EMITTER_HXX__
