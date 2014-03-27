Yuni's ChangeLOG
================


Release version 1.0.0
---------------------

**Features**

 * [core] Multihreading:
   - Introduced new convenient utility routines for Multithreading Programming :
     `spawn`, `every`, `async` and `sync` (see `yuni/thread/utility.h` for more details)
   - `Yuni::IThread` is now able to set the internal thread stack size via a parameter
     from its constructor, mainly to reduce memory consumption when a lot of threads are involved.
     The default value has been set to 512K, which should be large enough for most scenarios.
 * [core] `Yuni::Bind` now supports C++11 lambda
 * [core] `Yuni::String` now supports move semantic
 * [core] `Yuni::SmartPtr` is now able to detect if an ownership policy does not seem appropriate
 * [core] An helper class is now available to detect at compile time if a specific typedef
   is available within a given class and his ancestors (see `yuni/core/static/typedef.h`)
 * [core] `System::Console`: new routines `IsStdoutTTY()` and `IsStderrTTY()` to determine
   whether the standard output (or standard error output) is a TTY or not
 * [core/functional] added `Functional::Fold()` routine
 * [dbi] The DBI/sqlite adapter is now ready for production
 * Multiple fixes related to C++11 standard
 * [io] `Yuni::IO::LoadFromFile` now accepts std::string as a destination

**Changes**

* [core] Smart pointers:
    - IIntrusiveSmartPtr now uses a non recursive mutex.
      `Yuni::SmartPtr` won't compile if the underlying object has inherited
      from IIntrusiveSmartPtr and a non-appropriate ownership is used
    - The method `SmartPtr::unique()` has been removed for thread-safety issues
    - IIntrusiveSmartPtr::onRelease is now const.
* [core] Jobs:
   - `Yuni::Job::IJob` and `Yuni::Thread::IThread` now use intrusive smartptr
     to reduce memory allocation in high workload environments. Consequently it may not compile
     for some old program. Use `IJob::Ptr::Promote<T>::Ptr` or `Thread::IThread::Ptr::Promote<T>::Ptr`
     to get the appropriate smartptr type.
   - `<yuni/job/queue/queue.h>` has been remamed to `<yuni/job/queue/service.h>` for consistency
   - `<yuni/job/queue.h>` has been removed. Please use `<yuni/job/queue/service.h>` instead
   - `Yuni::Job::QueueService` is no longer a template class and has been
     internally revamped
 * [core] Math/Geometry:
   - the methods `Point2D::closeTo()` have been renamed to `Point2D::isCloseTo()`
   - the methods `Point3D::closeTo()` have been renamed to `Point3D::isCloseTo()`
 * [core] Misc: msinttypes for Visual Studio has been updated to r29 (http://code.google.com/p/msinttypes/)
 * [core] Event::Loop: added the method `gracefulStop()`
 * [core] Point2D and Point3D : Added operator * and *=.
 * [dbi] Updated sqlite to v3.8.3
 * [gl] Updated glew to v1.10

**Fixes**

 * [core] Fixed Clang compiler detection
 * [core] Fixed compilation on Visual Studio 2013
 * [core] Fixed a potential race condition in `Yuni::Timer`
 * [core] Math/Geometry : added missing `operator[] const` for `Point2D`, `Point3D`, and `Vector3D`.
 * [core] Fixed the method `Yuni::String::split()` with list or vector of `AnyString`,
   which was producing unpredictable results (ex: calling `String("1,2,3").split(mylist, ",")`
   with `mylist` as `AnyString::Vector`)
 * [core] Fixed the method `IO::Directory::Info::clean()`, which was only deleting subfolders
 * [core] removed overload on std::vector/std::list with std::ostream
 * [io] Fixed directory and file iterators when given a path containing mixed slashes and backslashes.
