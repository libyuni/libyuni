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
#include "job.h"



namespace Yuni
{
namespace Job
{

	inline enum Job::State IJob::state() const
	{
		return (enum Job::State) ((sint32) pState);
	}


	inline bool IJob::idle() const
	{
		return (0 != (pState & stateIdle));
	}


	inline bool IJob::waiting() const
	{
		return (0 != (pState & stateWaiting));
	}


	inline bool IJob::running() const
	{
		return (0 != (pState & stateRunning));
	}


	inline void IJob::cancel()
	{
		pCanceling = true;
	}


	inline bool IJob::canceling() const
	{
		return (0 != pCanceling);
	}


	inline void IJob::progression(const int p)
	{
		pProgression = ((p < 0) ? 0 : (p > 100 ? 100 : p));
	}


	inline bool IJob::finished() const
	{
		// The state must be at the very end
		return (pProgression >= 100 and pState == stateIdle);
	}


	inline bool IJob::shouldAbort() const
	{
		assert(pThread != NULL and "Job: The pointer to the attached thread must not be NULL");
		return (pCanceling or pThread->shouldAbort());
	}


	template<class T>
	void IJob::fillInformation(T& info)
	{
		// The first important value is the state
		info.state = (Yuni::Job::State) ((int) (pState));
		// Then, if the job is canceling its work
		info.canceling = (0 != pCanceling);

		info.progression = pProgression;
		info.name = caption();
	}


	inline String IJob::caption() const
	{
		return nullptr;
	}





} // namespace Job
} // namespace Yuni
