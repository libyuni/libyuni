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
#pragma once

#include "../../yuni.h"
#include "../../core/noncopyable.h"
#include "easing.h"


namespace Yuni
{
namespace UI
{
namespace Animation
{


	//! A timeline stores key values and transitions to animate a given variable in the program
	template<class T = float>
	class Transition final : private NonCopyable<Transition<T> >
	{
	public:
		//! Smart pointer
		typedef SmartPtr<Transition<T> >  Ptr;

		//! Prototype for an interpolation / easing function (cf Robert Penner's equations)
		typedef Easing::Func EasingFunc;

		//! Key frame map : key = frame time in ms, value = state at the given time
		typedef std::map<uint, T> FrameMap;

		//! Easing map : key = frame time in ms, value = easing used for this frame to the next
		typedef std::map<uint, EasingFunc> EasingMap;

	public:
		//! Constructor
		Transition(uint durationMS, bool loop = false):
			pDuration(durationMS),
			pLoop(loop)
		{}

		void addKeyFrame(const EasingFunc& easing, uint timeMS, const T& state)
		{
			pKeyFrames[timeMS] = state;
			pEasings[timeMS] = easing;
		}

		void removeKeyFrame(uint timeMS)
		{
			auto frameIt = pKeyFrames.find(timeMS);
			if (pKeyFrames.end() != frameIt)
				pKeyFrames.erase(frameIt);
			auto easingIt = pEasings.find(timeMS);
			if (pEasings.end() != frameIt)
				pEasings.erase(frameIt);
		}

		void clear()
		{
			pKeyFrames.clear();
			pEasings.clear();
		}

		//! Duration of the full animation
		uint duration() const { return pDuration; }
		//! Change animation duration
		void duration(uint newDurationMS) { pDuration = newDurationMS; }

		//! Should the animation loop ?
		bool loop() const { return pLoop; }
		//! Set whether the animation should loop
		void loop(bool shouldLoop) { pLoop = shouldLoop; }

		/*!
		** \brief Current interpolated value given the time in milliseconds along the timeline
		**
		** \remark
		**   It is legal to provide timeMS > duration(). Behaviour depends on the value of loop()
		*/
		T currentValue(uint timeMS) const;
		T operator () (uint timeMS) const;

	private:
		//! Apply a given easing at a given ratio ([0,1])
		T apply(const EasingFunc& easing, const T& start, const T& end, float ratio) const;

	private:
		//! Duration in milliseconds of the timeline
		uint pDuration;

		//! Must the animation loop ?
		bool pLoop;

		//! Key frames
		FrameMap pKeyFrames;

		//! Easings
		EasingMap pEasings;

	}; // class Transition



} // namespace Animation
} // namespace UI
} // namespace Yuni


#include "transition.hxx"

