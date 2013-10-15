

YMESSAGE_MODULE("Core")

# Paths for yuni-config, embedded version.
LIBYUNI_CONFIG_INCLUDE_PATH("intree" "core" "${CMAKE_CURRENT_SOURCE_DIR}/..")
LIBYUNI_CONFIG_LIB_PATH("intree" "core" "${LIBRARY_OUTPUT_PATH}")

# Paths for yuni-config, installed version.
LIBYUNI_CONFIG_INCLUDE_PATH("target" "core" "${CMAKE_INSTALL_PREFIX}/include/${YUNI_VERSIONED_INST_PATH}/")
LIBYUNI_CONFIG_LIB_PATH("target" "core" "${CMAKE_INSTALL_PREFIX}/lib/${YUNI_VERSIONED_INST_PATH}/")

LIBYUNI_CONFIG_LIB("both" "core"       "yuni-static-core")


if(APPLE)
	LIBYUNI_CONFIG_CFLAG("both" "core"	"-fvisibility=hidden")
endif(APPLE)

if (NOT WIN32 AND NOT WIN64)
	LIBYUNI_CONFIG_DEFINITION("both" "core" "_FILE_OFFSET_BITS=64")
else()
	LIBYUNI_CONFIG_DEFINITION("both" "core" "UNICODE")
	LIBYUNI_CONFIG_DEFINITION("both" "core" "_UNICODE")
	if (YUNI_TARGET MATCHES "debug")
		LIBYUNI_CONFIG_CFLAG("both" "core" "-ggdb3")
	endif()
endif()
LIBYUNI_CONFIG_DEFINITION("both" "core" "_REENTRANT")

if (MINGW)
	LIBYUNI_CONFIG_CFLAG("both" "core" "-mthreads")
	if (YUNI_TARGET MATCHES "debug")
		LIBYUNI_CONFIG_CFLAG("both" "core" "-ggdb3")
	endif()
endif()



include(CheckCSourceCompiles)
include(CheckCXXSourceCompiles)
include(CheckIncludeFiles)
include(CheckIncludeFileCXX)


set(CMAKE_REQUIRED_FLAGS_PREVIOUS ${CMAKE_REQUIRED_FLAGS})
set(CMAKE_REQUIRED_FLAGS "")

include(cmake/core/check-includes.cmake)
include(cmake/core/check-c++11.cmake)
include(cmake/core/check-stdint.cmake)
include(cmake/core/check-extras.cmake)

set(CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS_PREVIOUS})













if (NOT WIN32)
	find_package(Threads REQUIRED)
	link_libraries(${CMAKE_THREAD_LIBS_INIT})
	LIBYUNI_CONFIG_LIB_RAW_COMMAND("both" "core" "${CMAKE_THREAD_LIBS_INIT}")
endif()

#
# Generating informations about the platform
#
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/platform.cmake ${CMAKE_CURRENT_SOURCE_DIR}/platform.h)



set(SRC_CORE_ATOMIC
		core/atomic/int.h
		core/atomic/int.hxx
		core/atomic/traits.h
		core/atomic/string.hxx
		core/atomic/bool.h
)
source_group(core\\atomic FILES ${SRC_CORE_ATOMIC})



set(SRC_CORE_PREPROCESSOR
		core/preprocessor/std.h
		core/preprocessor/capabilities.h
		core/preprocessor/unixes.h
		core/preprocessor/windows.h
		core/preprocessor/enum.h
)
source_group(core\\preprocessor FILES ${SRC_CORE_PREPROCESSOR})

set(SRC_CORE_FOREACH
		core/foreach.h
)
source_group(core\\foreach FILES ${SRC_CORE_FOREACH})


set(SRC_CORE_ITERATOR
		core/iterator.h
		core/iterator/iterator.h
		core/iterator/iterator.hxx
)
source_group(core\\iterator FILES ${SRC_CORE_ITERATOR})


set(SRC_CORE_STATIC
		core/static/if.h
		core/static/remove.h
		core/static/assert.h
		core/static/types.h
		core/static/inherit.h
		core/static/dynamiccast.h
		core/static/moveconstructor.h
)
source_group(core\\static FILES ${SRC_CORE_STATIC})

set(SRC_CORE_TRAITS
		core/traits/cstring.h
		core/traits/extension/into-cstring.h
		core/traits/extension/length.h
		core/traits/length.h
)
source_group(core\\traits FILES ${SRC_CORE_TRAITS})


set(SRC_CORE_NULLABLE
		core/nullable.h
		core/nullable/nullable.h
		core/nullable/nullable.hxx
		core/nullable/extension.hxx
		core/nullable/holder.h
)
source_group(core\\nullable FILES ${SRC_CORE_NULLABLE})


set(SRC_CORE_EXCEPTIONS
		core/exceptions.h
		core/exceptions/badcast.h
)
source_group(core\\exceptions FILES ${SRC_CORE_EXCEPTIONS})


set(SRC_CORE_VALIDATOR
		core/validator/text/default.h
		core/validator/text/default.hxx
		core/validator/text/default.private.h
		core/validator/text/validator.h
		core/validator/validator.h
)
source_group(core\\validator FILES ${SRC_CORE_VALIDATOR})

set(SRC_CORE_SLIST
		core/slist/iterator.inc.hpp
		core/slist/slist.h
		core/slist/slist.hxx
		core/slist.h
)
source_group(core\\linkedlist FILES ${SRC_CORE_SLIST})



set(SRC_CORE_STRING
		core/string/string.h
		core/string/string.cpp
		core/string/string.hxx
		core/string/operators.hxx
		core/string/iterator.inc.hpp
		core/string/iterator.hxx
		core/string/utf8char.h
		core/string/utf8char.hxx
		core/string/traits/append.h
		core/string/traits/assign.h
		core/string/traits/fill.h
		core/string/traits/integer.h
		core/string/traits/vnsprintf.h
		core/string/traits/traits.h
		core/string/traits/traits.cpp
		core/string/traits/traits.hxx
		core/string/traits/into.h

		# Wide String
		core/string/wstring.h
		core/string/wstring.hxx

		# misc
		core/string/escape.h
		core/string/escape.hxx

		core/string.h
)
source_group(core\\string FILES ${SRC_CORE_STRING})


set(SRC_CORE_STL
		core/stl/algorithm.h
)
source_group(core\\Stl FILES ${SRC_CORE_STL})


set(SRC_CORE_LOGS
		core/logs/logs.h
		core/logs/logs.hxx
		core/logs/verbosity.h
		core/logs/null.h
		core/logs/buffer.h
		# Handlers
		core/logs/handler/stdcout.h
		core/logs/handler/file.h
		core/logs/handler/file.hxx
		core/logs/handler/callback.h
		# Decorators
		core/logs/decorators/verbositylevel.h
		core/logs/decorators/time.h
		core/logs/decorators/time.cpp
		core/logs/decorators/applicationname.h
		core/logs/decorators/message.h
		core/logs.h
)
source_group(core\\logs FILES ${SRC_CORE_LOGS})



set(SRC_CORE_SMARTPTR
		core/smartptr/policies/checking.h
		core/smartptr/policies/ownership.h
		core/smartptr/policies/storage.h
		core/smartptr/policies/constness.h
		core/smartptr/smartptr.h
		core/smartptr/smartptr.hxx
		core/smartptr/intrusive.h
		core/smartptr/intrusive.hxx
)
source_group(core\\smartptr FILES ${SRC_CORE_SMARTPTR})


set(SRC_CORE_ANY
		core/any.h
		core/any/any.h
		core/any/any.private.h
		core/any/any.hxx
		core/any/any.cpp
)
source_group(core\\any FILES ${SRC_CORE_ANY})

set(SRC_CORE_VARIANT
		core/variant.h
		core/variant/variant.h
		core/variant/variant.hxx
		core/variant/variant.cpp
		core/variant/fwd.h
		core/variant/dataholder/dataholder.cpp
		core/variant/dataholder/dataholder.h
		core/variant/dataholder/dataholder.hxx
		core/variant/dataholder/string.h
		core/variant/dataholder/array.h
)
source_group(core\\variant FILES ${SRC_CORE_VARIANT})



set(SRC_CORE_DYNAMICLIBRARY
		core/dynamiclibrary/file.h
		core/dynamiclibrary/file.hxx
		core/dynamiclibrary/file.cpp
		core/dynamiclibrary/symbol.h
		core/dynamiclibrary/symbol.hxx
		core/dynamiclibrary.h
)
source_group(core\\dynamiclibrary FILES ${SRC_CORE_DYNAMICLIBRARY})


set(SRC_CORE_BIND
		core/bind/bind.h
		core/bind/bind.hxx
		core/bind/traits.h
		core/bind.h
)
source_group(core\\bind FILES ${SRC_CORE_BIND})

set(SRC_CORE_CHARSET
		core/charset/charset.h
		core/charset/charset.hxx
		core/charset/charset.cpp
)
source_group(core\\charset FILES ${SRC_CORE_CHARSET})


set(SRC_CORE_EVENT
		core/event/event.h
		core/event/event.hxx
		core/event/interfaces.h
		core/event/interfaces.hxx
		core/event/traits.h
		core/event/observer/observer.h
		core/event/observer/observer.hxx
		core/event.h

		# Event loop
		core/event/loop.h
		core/event/loop.hxx
		core/event/loop.fwd.h
		core/event/flow
		core/event/flow/continuous.h
		core/event/flow/timer.h
		core/event/statistics
		core/event/statistics/none.h
)
source_group(core\\event FILES ${SRC_CORE_EVENT})


set(SRC_CORE_MATH
		# Standard functions
		core/math/math.h
		core/math.h
		core/math/math.hxx
		core/math/msvc.hxx
		core/math/base.h
		# Log(N)
		core/math/log.h
		core/math/log.hxx
		# Trigonometric functions
		core/math/trigonometric.h
		core/math/trigonometric.hxx
		# Exponential functions
		core/math/exponential.h
		core/math/exponential.hxx
		core/math/complex.h
		core/math/complex.hxx
		# Random number generators
		core/math/random/distribution.h
		core/math/random/distribution.hxx
		core/math/random/default.h
		core/math/random/default.hxx
		core/math/random/range.h
		core/math/random/range.hxx
		core/math/random/constant.h

		core/rect2D.h
		core/math/geometry/rect2D.h
		core/math/geometry/rect2D.hxx
		core/vector3D.h
		core/math/geometry/vector3D.h
		core/math/geometry/vector3D.hxx
		core/point2D.h
		core/math/geometry/point2D.h
		core/point3D.h
		core/math/geometry/point3D.h
		core/quaternion.h
		core/math/geometry/quaternion.h
		core/math/geometry/quaternion.hxx
		core/boundingbox.h
		core/math/geometry/boundingbox.h
		core/math/geometry/boundingbox.hxx
		core/math/geometry/edge.h
		core/math/geometry/triangle.h
		core/math/geometry/vertex.h
)
source_group(core\\math FILES ${SRC_CORE_MATH})

set(SRC_CORE_BIT
		core/bit.h
		core/bit/bit.h core/bit/bit.hxx
		core/bit/array.h
		core/bit/array.hxx
		core/bit/array.cpp
)
source_group("core\\bit" FILES ${SRC_CORE_BIT})


# Singleton
set(SRC_CORE_SINGLETON
		core/singleton.h
		core/singleton/singleton.h
		core/singleton/singleton.hxx
		core/singleton/policies/creation.h
		core/singleton/policies/lifetime.h
)
source_group(core\\singleton FILES ${SRC_CORE_SINGLETON})



# Hash - Checksums
set(SRC_CORE_HASH_CHECKSUM
		core/hash/checksum/checksum.h
		core/hash/checksum/checksum.hxx
		core/hash/checksum/md5.hxx
		core/hash/checksum/md5.h
		core/hash/checksum/md5.cpp
)
source_group(core\\hash\\checksum FILES ${SRC_CORE_HASH_CHECKSUM})


# System
set(SRC_CORE_SYSTEM
		core/system/stdint.h
		core/system/nullptr.h
		core/system/capabilities.h
		core/system/suspend.h
		core/system/suspend.cpp
		core/suspend.h
		core/fwd.h
		core/system/cpu.h
		core/system/cpu.cpp
		core/system/memory.h
		core/system/memory.hxx
		core/system/memory.cpp
		core/system/environment.h
		core/system/environment.hxx
		core/system/username.h
		core/system/username.hxx
		core/system/endian.h
		core/system/process.h
		core/system/process.cpp
)
if(WIN32 OR WIN64)
	set(SRC_CORE_SYSTEM ${SRC_CORE_SYSTEM} core/system/username.cpp)
endif()
source_group(core\\system FILES ${SRC_CORE_SYSTEM})


set(SRC_CORE_TREE_N
		core/tree/treeN.h
		core/tree/n/treeN.h
		core/tree/n/treeN.hxx
		core/tree/n/iterator/iterator.h
		core/tree/n/iterator/childiterator.h
		core/tree/n/iterator/childiterator.hxx
		core/tree/n/iterator/depthprefixiterator.h
		core/tree/n/iterator/depthprefixiterator.hxx
		core/tree/n/iterator/depthinfixiterator.h
		core/tree/n/iterator/depthinfixiterator.hxx
		core/tree/n/iterator/depthsuffixiterator.h
		core/tree/n/iterator/depthsuffixiterator.hxx
)
source_group(core\\tree FILES ${SRC_CORE_TREE_N})


set(SRC_CORE_UNIT
		core/unit/define.h
		core/unit/undef.h
		core/unit/unit.h
		core/unit/unit.hxx
		core/unit.h
		core/unit/angle.h
		core/unit/length.h
		core/unit/length/length.h
		core/unit/length/metric.h
		core/unit/length/extra.h
		core/unit/luminance.h
)
source_group(core\\unit FILES ${SRC_CORE_UNIT})

set(SRC_CORE_UTILS
		core/utils/utils.h
		core/utils/hexdump.h core/utils/hexdump.hxx core/utils/hexdump.cpp
)
source_group(core\\utils FILES ${SRC_CORE_UTILS})

set(SRC_CORE_URI
		core/uri/uri.private.h  core/uri/uri.private.cpp
		core/uri/uri.h core/uri/uri.hxx core/uri/uri.cpp
		core/uri.h
)
source_group(core\\uri FILES ${SRC_CORE_URI})

set(SRC_CORE_PROCESS
		core/process.h
		core/process/fwd.h
		core/process/process.h
		core/process/process.hxx
		core/process/process.cpp
		core/process/rename.h
		core/process/rename.cpp
)
source_group(core\\process FILES ${SRC_CORE_PROCESS})


set(SRC_IO_FILENAME
		io/io.h
		io/constants.cpp
		io/constants.h
		io/exists.cpp
		io/io.hxx
		io/error.h
		io/filename-manipulation.h
		io/filename-manipulation.cpp
		io/filename-manipulation.hxx
)
source_group(io FILES ${SRC_IO_FILENAME})



set(SRC_IO_DIRECTORY
		io/directory.h
		io/directory/directory.hxx
		io/directory/commons.h
		io/directory/remove.cpp
		io/directory/current.cpp
		io/directory/create.cpp
		io/directory/copy.cpp
		io/directory/info.h
		io/directory/iterator.h
		io/directory/system.h
		io/directory/system.hxx
)
source_group(io\\directory FILES ${SRC_IO_DIRECTORY})

set(SRC_IO_DIRECTORY_ITERATOR
		io/directory/iterator/iterator.cpp
		io/directory/iterator/iterator.h
		io/directory/iterator/iterator.hxx
		io/directory/iterator/detachedthread.h
)
source_group(io\\directory\\iterator FILES ${SRC_IO_DIRECTORY_ITERATOR})


set(SRC_IO_DIRECTORY_INFO
		io/directory/info/iterator.inc.hpp
		io/directory/info/info.h
		io/directory/info/info.hxx
		io/directory/info/info.cpp
		io/directory/info/platform.cpp
		io/directory/info/platform.h
)
source_group(io\\directory\\Info FILES ${SRC_IO_DIRECTORY_INFO})



set(SRC_IO_FILE
		io/file/openmode.h
		io/file/openmode.cpp
		io/file/stream.h
		io/file/stream.hxx
		io/file/stream.cpp
		io/file.h
		io/file/file.hxx
		io/file/file.cpp
		io/file/copy.cpp
)
source_group(io\\file FILES ${SRC_IO_FILE})

set(SRC_IO_SEARCHPATH
		io/searchpath.h
		io/searchpath/searchpath.h
		io/searchpath/searchpath.hxx
		io/searchpath/searchpath.cpp
)
source_group(io\\searchpath FILES ${SRC_IO_SEARCHPATH})



set(SRC_CORE_GETOPT
		core/getopt.h
		core/getopt/option.h
		core/getopt/option.cpp
		core/getopt/parser.h
		core/getopt/parser.hxx
		core/getopt/parser.cpp
)
source_group(core\\getopt FILES ${SRC_CORE_GETOPT})


set(SRC_CORE_VERSION
		core/version/version.h
		core/version/version.cpp
		core/version/version.hxx
)
source_group(core\\version FILES ${SRC_CORE_VERSION})




# System - Microsoft Windows
set(SRC_CORE_SYSTEM_WINDOWS core/system/windows.hdr.h  core/system/gettimeofday.h)
if(WIN32 AND MSVC)
	set(SRC_CORE_SYSTEM_WINDOWS ${SRC_CORE_SYSTEM_WINDOWS} core/system/gettimeofday.cpp)
endif(WIN32 AND MSVC)
source_group(core\\system\\windows FILES ${SRC_CORE_SYSTEM_WINDOWS})




# Threads
set(SRC_THREADS
		thread/policy.h
		thread/fwd.h
		thread/pthread.h
		thread/mutex.h
		thread/mutex.cpp
		thread/mutex.hxx
		thread/thread.h
		thread/thread.hxx
		thread/semaphore.h
		thread/semaphore.hxx
		thread/semaphore.cpp
		thread/rwmutex.h
		thread/rwmutex.hxx
		thread/rwmutex.cpp
		thread/id.h
		thread/id.cpp
		thread/thread.cpp
		thread/signal.h
		thread/signal.hxx
		thread/signal.cpp
		thread/timer.h
		thread/timer.hxx
		thread/timer.cpp
		thread/array.h
		thread/array.hxx)
source_group(threads FILES ${SRC_THREADS})

# Jobs
set(SRC_JOBS
		job/enum.h
		job/forward.h
		job/job.h
		job/job.hxx
		job/job.cpp
		job/queue.h
		job/queue/queue.h
		job/queue/queue.hxx
		job/queue/thread.h
		job/queue/thread.hxx
		job/queue/waitingroom.h
		job/queue/waitingroom.hxx
		job/queue/waitingroom.cpp

		# Scheduler
		job/scheduler/highestpriorityfirst.h
		job/scheduler/highestpriorityfirst.cpp
		job/scheduler/highestpriorityfirst.hxx
		)
source_group(jobs FILES ${SRC_JOBS})


set(SRC_CORE_COLOR
		# Color
		core/color/rgba.h
		core/color/rgba.hxx
		core/color/rgb.h
		core/color/rgb.hxx
		core/color/fwd.h
	)
source_group(core\\color FILES ${SRC_CORE_COLOR})


set(SRC_CORE_PREDICATES_RESULTS
		core/predicate/result/sum.h
		core/predicate/result/and.h
		core/predicate/result/or.h
		)
source_group(core\\predicate\\Result FILES ${SRC_CORE_PREDICATES_RESULTS})


set(SRC_CORE_SYSTEM_CONSOLE
		core/system/console.h
		core/system/console/console.h
		core/system/console/console.hxx
		)
source_group(core\\system\\Console FILES ${SRC_CORE_SYSTEM_CONSOLE})



set(SRC_CORE_MISC
		core/noncopyable.h
		core/dictionary.h
)
source_group(core\\any FILES ${SRC_CORE_ANY})

set(SRC_DATETIME
		datetime/timestamp.h
		datetime/timestamp.hxx
		datetime/timestamp.cpp
)
source_group(datetime\\ FILES ${SRC_DATETIME})



add_library(yuni-static-core STATIC
		yuni.h
		${SRC_CORE_MISC}
		${SRC_CORE_PREDICATES_RESULTS}
		${SRC_CORE_STATIC}
		${SRC_CORE_ITERATOR}
		${SRC_CORE_TRAITS}
		${SRC_CORE_PREPROCESSOR}
		${SRC_CORE_FOREACH}
		${SRC_CORE_BIT}
		${SRC_CORE_EXCEPTIONS}
		${SRC_CORE_SYSTEM}
		${SRC_CORE_SYSTEM_WINDOWS}
		${SRC_CORE_SYSTEM_CONSOLE}
		${SRC_CORE_STRING}
		${SRC_CORE_NULLABLE}
		${SRC_CORE_STL} ${SRC_CORE_VALIDATOR}
		${SRC_DATETIME}
		${SRC_IO_FILENAME}
		${SRC_IO_DIRECTORY}
		${SRC_IO_DIRECTORY_INFO}
		${SRC_IO_DIRECTORY_ITERATOR}
		${SRC_IO_FILE}
		${SRC_IO_SEARCHPATH}
		${SRC_CORE_PROCESS}
		${SRC_CORE_LOGS}
		${SRC_CORE_ATOMIC}
		${SRC_CORE_SMARTPTR}
		${SRC_CORE_ANY}
		${SRC_CORE_COLOR}
		${SRC_CORE_SLIST}
		${SRC_CORE_VARIANT}
		${SRC_CORE_DYNAMICLIBRARY}
		${SRC_CORE_BIND}
		#${SRC_CORE_CHARSET}
		${SRC_CORE_EVENT}
		${SRC_CORE_MATH}
		${SRC_CORE_SINGLETON}
		${SRC_CORE_HASH_CHECKSUM}
		${SRC_CORE_TREE_N}
		${SRC_CORE_VERSION}
		${SRC_CORE_UNIT}
		${SRC_CORE_UTILS}
		${SRC_CORE_URI}
		${SRC_CORE_GETOPT}
		${SRC_PREDICATES_RESULTS}
		${SRC_THREADS} ${SRC_JOBS}
	)

if (WIN32 OR WIN64)
	add_definitions("-DUNICODE")
	add_definitions("-D_UNICODE")
endif ()

# Setting output path
set_target_properties(yuni-static-core PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${YUNI_OUTPUT_DIRECTORY}/lib")

# Installation
install(TARGETS yuni-static-core COMPONENT ${YUNICOMPONENT_CORE}
	ARCHIVE DESTINATION lib/${YUNI_VERSIONED_INST_PATH})

# Install Core-related headers
install(
	DIRECTORY core job thread io
	COMPONENT ${YUNICOMPONENT_CORE}
	DESTINATION include/${YUNI_VERSIONED_INST_PATH}/yuni
	FILES_MATCHING
		PATTERN "*.h"
		PATTERN "*.hxx"
	PATTERN ".svn" EXCLUDE
	PATTERN "CMakeFiles" EXCLUDE
	PATTERN "cmake" EXCLUDE
)
# Install other core-related base files
install(FILES
	Doxygen.txt
	doxygen.h
	platform.h
	yuni.h
	COMPONENT ${YUNICOMPONENT_CORE}
	DESTINATION include/${YUNI_VERSIONED_INST_PATH}/yuni
)

install(FILES yuni.version
	COMPONENT ${YUNICOMPONENT_CORE}
	DESTINATION include/${YUNI_VERSIONED_INST_PATH}/ RENAME "yuni.version")

install(FILES "${YUNI_LIBYUNI_CONFIG_TARGET_INIFILE}"
	COMPONENT ${YUNICOMPONENT_CORE}
	DESTINATION include/${YUNI_VERSIONED_INST_PATH}/ RENAME "yuni.config.${YUNI_LIBYUNI_CONFIG_COMPILER}")


