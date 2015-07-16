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
#include "../../../thread/thread.h"



namespace Yuni
{
namespace Private
{
namespace IO
{
namespace Directory
{
namespace Iterator
{

	// Forward declarations
	class Interface;
	class Options;
	class IDetachedThread;

	typedef Yuni::IO::Flow Flow;

	/*!
	** \brief Platform specific iplementation for traversing a folder
	*/
	void Traverse(Options& options, IDetachedThread* thread);

	Flow TraverseUnixFolder(const String&, Options& options, IDetachedThread* thread, bool files);
	Flow TraverseWindowsFolder(const String&, Options& options, IDetachedThread* thread, bool files);




	class Interface
	{
	public:
		Interface() {}
		virtual ~Interface() {}

	protected:
		virtual bool onStart(const String& root) = 0;

		virtual void onTerminate() = 0;

		virtual void onAbort() = 0;

		virtual Flow onBeginFolder(const String& filename, const String& parent, const String& name) = 0;
		virtual void onEndFolder(const String& filename, const String& parent, const String& name) = 0;

		virtual Flow onFile(const String& filename, const String& parent, const String& name, uint64 size) = 0;

		virtual Flow onError(const String& filename) = 0;

		virtual Flow onAccessError(const String& filename) = 0;

	public:
		friend void Traverse(Options&, IDetachedThread*);
		friend Flow TraverseUnixFolder(const String&, Options&, IDetachedThread*, bool);
		friend Flow TraverseWindowsFolder(const String&, Options&, IDetachedThread*, bool);
	}; // class Interface



	class Options final
	{
	public:
		//! Default constructor
		Options()
			: self(nullptr)
			# ifdef YUNI_OS_WINDOWS
			, wbuffer(nullptr)
			# endif
			, counter(0)
		{}

	public:
		/*!
		** \brief The root folder
		** \internal The shared access to this variable is already guaranted
		**   by the class IIterator
		*/
		String::VectorPtr rootFolder;
		//! Pointer to the parent class
		Interface* self;

		# ifdef YUNI_OS_WINDOWS
		wchar_t* wbuffer;
		# endif

		/*!
		** \brief Arbitrary counter to reduce the number of calls to suspend()
		*/
		uint counter;
	};


	# ifndef YUNI_NO_THREAD_SAFE
	class IDetachedThread : public Yuni::Thread::IThread
	{
	public:
		IDetachedThread() {}
		virtual ~IDetachedThread()
		{
			# ifdef YUNI_OS_WINDOWS
			delete[] options.wbuffer;
			options.wbuffer = nullptr;
			# endif
		}

		bool suspend()
		{
			return Yuni::Thread::IThread::suspend();
		}

	public:
		Options options;

	protected:
		virtual bool onExecute() override
		{
			Traverse(options, this);
			return false;
		}

		virtual void onStop() override
		{
			# ifdef YUNI_OS_WINDOWS
			delete[] options.wbuffer;
			options.wbuffer = nullptr;
			# endif
		}

		virtual void onKill() override
		{
			# ifdef YUNI_OS_WINDOWS
			delete[] options.wbuffer;
			options.wbuffer = nullptr;
			# endif
		}

	}; // class IDetachedThread

	# else
	class IDetachedThread {};
	# endif




} // namespace Iterator
} // namespace Directory
} // namespace IO
} // namespace Private
} // namespace Yuni

