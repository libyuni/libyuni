Yuni's ChangeLOG
================


release version 1.0.0
---------------------

**Features**

 * [core] Introduced new convenient utility routines for Multithreading Programming :
   `spawn`, `every`, `async` and `sync` (see `yuni/thread/utility.h` for more details)
 * [core] `Yuni::Bind` now supports C++11 lambda
 * [core] `Yuni::String` now supports move semantic
 * [core] `Yuni::SmartPtr` is now able to detect if an ownership policy does not seem appropriate
 * [core] An helper class is now available to detect if a specific typedef is available within
   a given class and his ancestors (see `yuni/core/static/typedef.h`)
 * [core] `Yuni::IThread` is now able to set the internal thread stack size via a parameter
   from its constructor, mainly to reduce memory consumption when a lot of threads are involved.
   The default value has been set to 512K, which should be large enough for most scenarios.

**Changes**

* [core] `Yuni::Job::IJob` and `Yuni::Thread::IThread` now use intrusive smartptr
   to reduce memory allocation in high workload environments. Consequently it may not compile
   for some old program. Use `IJob::Ptr::Promote<T>::Ptr` or `Thread::IThread::Ptr::Promote<T>::Ptr`
   to get the appropriate smartptr type.
 * [core] `<yuni/job/queue/queue.h>` has been remamed to `<yuni/job/queue/service.h>` for consistency
 * [core] `<yuni/job/queue.h>` has been removed. Please use `<yuni/job/queue/service.h>` instead
 * [core] IIntrusiveSmartPtr now uses a non recursive mutex.
   `Yuni::SmartPtr` won't compile if the underlying object has inherited
   from IIntrusiveSmartPtr and a non-appropriate ownership is used
 * [core] msinttypes for Visual Studio has been updated to r29 (http://code.google.com/p/msinttypes/)
 * [core] `Yuni::Job::QueueService` is no longer a template class and has been
   internally revamped
 * [core] The method `SmartPtr::unique()` has been removed for thread-safety issues

**Fixes**

 * [core] Fixed Clang compiler detection
 * [core] Fixed a potential race condition in `Yuni::Timer`
 * [core] Math/Geometry : added missing `operator[] const` for `Point2D`, `Point3D`, and `Vector3D`.

