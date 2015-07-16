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
#include "iterator.h"
#include "../../../core/traits/cstring.h"
#include "../../../core/traits/length.h"
#include "../../../core/static/remove.h"
#include "../../io.h"




namespace Yuni
{
namespace IO
{
namespace Directory
{

	# ifndef YUNI_NO_THREAD_SAFE
	template<bool DetachedT>
	inline IIterator<DetachedT>::DetachedThread::DetachedThread()
	{}

	template<bool DetachedT>
	inline IIterator<DetachedT>::DetachedThread::~DetachedThread()
	{
		stop();
	}
	# endif



	template<bool DetachedT>
	inline IIterator<DetachedT>::IIterator()
		# ifndef YUNI_NO_THREAD_SAFE
		:pThread(NULL)
		# endif
	{
	}

	template<bool DetachedT>
	inline IIterator<DetachedT>::IIterator(const IIterator& rhs)
		# ifndef YUNI_NO_THREAD_SAFE
		:pThread(NULL)
		# endif
	{
		typename ThreadingPolicy::MutexLocker locker(rhs);
		pRootFolder = rhs.pRootFolder;
	}

	template<bool DetachedT>
	IIterator<DetachedT>& IIterator<DetachedT>::operator = (const IIterator<DetachedT>& rhs)
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		typename ThreadingPolicy::MutexLocker locker2(rhs);
		pRootFolder = rhs.pRootFolder;
		return *this;
	}


	template<bool DetachedT>
	inline IIterator<DetachedT>::~IIterator()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		stop();
		delete pThread;
		# endif
	}


	template<bool DetachedT>
	inline void IIterator<DetachedT>::clear()
	{
		typename ThreadingPolicy::MutexLocker locker(*this);
		pRootFolder.clear();
	}



	template<bool DetachedT>
	inline void IIterator<DetachedT>::add(const AnyString& folder)
	{
		if (not folder.empty())
		{
			// Pushing it into the list
			String* item = new String();
			IO::Canonicalize(*item, folder);

			typename ThreadingPolicy::MutexLocker locker(*this);
			pRootFolder.push_back(item);
		}
	}


	template<bool DetachedT>
	bool IIterator<DetachedT>::start()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		if (detached)
		{
			// Lock
			typename ThreadingPolicy::MutexLocker locker(*this);

			// Early detection of an invalid root folder
			if (pRootFolder.empty())
				return false;

			if (nullptr == pThread)
			{
				pThread = new DetachedThread();
			}
			else
			{
				// Do nothing if already started
				if (pThread->started())
					return false;
			}

			// Providing a reference to ourselves for events
			pThread->options.self = this;
			pThread->options.rootFolder = pRootFolder; // copy

			// Starting the thread
			return (Thread::errNone == pThread->start());
		}
		else
		# endif
		{
			using namespace Yuni::Private::IO::Directory::Iterator;
			Options opts;
			opts.self = this; // Providing a reference to ourselves for events
			{
				// Lock
				typename ThreadingPolicy::MutexLocker locker(*this);
				// Early detection of an invalid root folder
				if (pRootFolder.empty())
					return false;

				opts.rootFolder = pRootFolder;
			}

			// The calling thread will block until the traversing is complete
			Traverse(opts, NULL);
		}
		return true;
	}


	template<bool DetachedT>
	bool IIterator<DetachedT>::stop(uint timeout)
	{
		# ifndef YUNI_NO_THREAD_SAFE
		if (detached)
		{
			typename ThreadingPolicy::MutexLocker locker(*this);
			if (pThread)
			{
				bool success = (Yuni::Thread::errNone == pThread->stop(timeout));
				if (success)
				{
					delete pThread; // be resources friendly
					pThread = nullptr;
				}
				return success;
			}
		}
		# else
		(void) timeout; // to avoid compiler warning
		# endif
		return true;
	}


	template<bool DetachedT>
	inline void IIterator<DetachedT>::wait()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		if (detached)
		{
			// Lock
			typename ThreadingPolicy::MutexLocker locker(*this);
			if (pThread)
			{
				pThread->wait();
				delete pThread; // be resources friendly
				pThread = nullptr;
			}
		}
		# endif
	}


	template<bool DetachedT>
	inline void IIterator<DetachedT>::wait(uint timeout)
	{
		# ifndef YUNI_NO_THREAD_SAFE
		if (detached)
		{
			// Lock
			typename ThreadingPolicy::MutexLocker locker(*this);
			if (pThread)
				pThread->wait(timeout);
		}
		# else
		(void) timeout;
		# endif
	}



	template<bool DetachedT>
	inline Flow
	IIterator<DetachedT>::onBeginFolder(const String&, const String&, const String&)
	{
		return flowContinue; // Do nothing
	}

	template<bool DetachedT>
	inline void
	IIterator<DetachedT>::onEndFolder(const String&, const String&, const String&)
	{
		// Do nothing
	}

	template<bool DetachedT>
	inline Flow
	IIterator<DetachedT>::onFile(const String&, const String&, const String&, uint64)
	{
		return flowContinue; // Do nothing
	}

	template<bool DetachedT>
	inline Flow
	IIterator<DetachedT>::onError(const String&)
	{
		return flowContinue; // Do nothing
	}


	template<bool DetachedT>
	inline Flow
	IIterator<DetachedT>::onAccessError(const String&)
	{
		return flowContinue;
	}


	template<bool DetachedT>
	inline void IIterator<DetachedT>::onAbort()
	{
		// Do nothing
	}


	template<bool DetachedT>
	inline void IIterator<DetachedT>::onTerminate()
	{
		// Do nothing
	}


	template<bool DetachedT>
	inline bool IIterator<DetachedT>::onStart(const String&)
	{
		// Do nothing
		return true;
	}





} // namespace Directory
} // namespace IO
} // namespace Yuni

