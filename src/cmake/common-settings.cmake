
#
# CMake Stuff
#
#set(CMAKE_VERBOSE_MAKEFILE ON)
# Policy
if(NOT CMAKE_MINOR_VERSION EQUAL 4 OR NOT CMAKE_MAJOR_VERSION EQUAL 2 )
	cmake_policy(SET CMP0004 OLD)
	cmake_policy(SET CMP0003 NEW)
endif()
include(CheckIncludeFile)
include(CheckCXXCompilerFlag)


if ("${CMAKE_BUILD_TYPE}" STREQUAL "release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
	set(YUNI_)
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG}")
endif()


#
# Clang Detection
#
if (CMAKE_CXX_COMPILER MATCHES ".*clang")
	set(CLANG 1)
endif ()

#
# Getting the folder where this file is located
#
set(CurrentFolder "${CMAKE_CURRENT_LIST_FILE}")
string(REPLACE "\\common-settings.cmake" "" CurrentFolder "${CurrentFolder}")
string(REPLACE "/common-settings.cmake" "" CurrentFolder "${CurrentFolder}")
# Current Folder : ${CurrentFolder}

#
# Detect Special Instructions Set (SSE, MMX...)
#
include("${CurrentFolder}/DetectInstructionsSets.cmake")


# Common options to all GCC-based compilers
set(YUNI_COMMON_CC_OPTIONS   "-Wall -Wextra -Wunused-parameter -Wconversion")
set(YUNI_COMMON_CXX_OPTIONS  "-Woverloaded-virtual")
if(NOT MSVC)
	check_cxx_compiler_flag("-Wunused-but-set-variable -Wunused-but-set-parameter"
		YUNI_HAS_GCC_UNUSED_BUT_PARAM)
	if (YUNI_HAS_GCC_UNUSED_BUT_PARAM)
		set(YUNI_COMMON_CC_OPTIONS  "${YUNI_COMMON_CC_OPTIONS} -Wunused-but-set-variable -Wunused-but-set-parameter")
	endif (YUNI_HAS_GCC_UNUSED_BUT_PARAM)
endif()

set(YUNI_COMMON_CC_OPTIONS  "${YUNI_COMMON_CC_OPTIONS} -Wmissing-noreturn -Wcast-align  -Wfloat-equal -Wundef")
set(YUNI_COMMON_CC_OPTIONS  "${YUNI_COMMON_CC_OPTIONS} -D_REENTRANT -DXUSE_MTSAFE_API")
set(YUNI_COMMON_CC_OPTIONS  "${YUNI_COMMON_CC_OPTIONS} -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64")


set(COMMON_MSVC_FLAGS "/W3 /MP4 /Zi")
set(COMMON_MSVC_FLAGS "${COMMON_MSVC_FLAGS} /DREENTRANT /D_LARGEFILE_SOURCE /D_LARGEFILE64_SOURCE /D_FILE_OFFSET_BITS=64")



# Common options for GCC on Unixes (mingw excluded)
# fPIC seems a good choice in most cases
set(YUNI_COMMON_CC_OPTIONS_UNIX   "${YUNI_COMMON_CC_OPTIONS} -fPIC")

include(CheckCXXCompilerFlag)
if(NOT MSVC)
	check_cxx_compiler_flag("-std=c++14"     YUNI_HAS_CPP14_SUPPORT)
	check_cxx_compiler_flag("-std=c++11"     YUNI_HAS_CPP11_SUPPORT)
	check_cxx_compiler_flag("-std=c++0x"     YUNI_HAS_GCC_CPP0X_SUPPORT)
	check_cxx_compiler_flag("-stdlib=libc++" YUNI_HAS_LIB_CPP11_SUPPORT) # clang, Apple gcc...
	check_cxx_compiler_flag("-g"             YUNI_HAS_COMPILER_DEBUG_INFORMATION)
	check_cxx_compiler_flag("-O3"            YUNI_HAS_COMPILER_OPTIMIZATION_LEVEL_3)
	if (CLANG)
		# currently, only clang really supports -O4
		check_cxx_compiler_flag("-O4"        YUNI_HAS_COMPILER_OPTIMIZATION_LEVEL_4)
	endif()

	if (YUNI_HAS_CPP14_SUPPORT)
		set(YUNI_COMMON_CXX_OPTIONS  "${YUNI_COMMON_CXX_OPTIONS} -std=c++14")
	else()
		if (YUNI_HAS_CPP11_SUPPORT)
			set(YUNI_COMMON_CXX_OPTIONS  "${YUNI_COMMON_CXX_OPTIONS} -std=c++11")
		else()
			if (YUNI_HAS_GCC_CPP0X_SUPPORT)
				set(YUNI_COMMON_CXX_OPTIONS  "${YUNI_COMMON_CXX_OPTIONS} -std=c++0x")
			endif()
		endif()
	endif()
	if (YUNI_HAS_LIB_CPP11_SUPPORT)
		set(YUNI_COMMON_CXX_OPTIONS  "${YUNI_COMMON_CXX_OPTIONS} -stdlib=libc++")
	endif()
endif()
if (NOT MSVC AND NOT CLANG)
	# clang produces the warning "argument unused during compilation"
	check_cxx_compiler_flag("-ggdb"  YUNI_HAS_GCC_FLAG_GGDB)
	# clang produces the warning "argument unused during compilation"
	check_cxx_compiler_flag("-ggdb3" YUNI_HAS_GCC_FLAG_GGDB3)
endif()

if (APPLE)
	# http://lists.cs.uiuc.edu/pipermail/cfe-dev/2011-January/012999.html
	# Temporary workaround for compiling with Clang on OS X
	set(YUNI_COMMON_CC_OPTIONS_UNIX  "${YUNI_COMMON_CC_OPTIONS_UNIX} -U__STRICT_ANSI__")
endif()

set(COMPILER_OPTIMIZATION_LEVEL "")
if (YUNI_HAS_COMPILER_OPTIMIZATION_LEVEL_3)
	set(COMPILER_OPTIMIZATION_LEVEL "-O3")
endif()
if (YUNI_HAS_COMPILER_OPTIMIZATION_LEVEL_4)
	set(COMPILER_OPTIMIZATION_LEVEL "-O4")
endif()




# Unicode on Windows
if(WIN32 OR WIN64)
	add_definitions("-DUNICODE")
	add_definitions("-D_UNICODE")
	add_definitions("-D__UNICODE")
endif()

# Thread safety
add_definitions("-D_REENTRANT -DXUSE_MTSAFE_API")




if(NOT WIN32)
	set(CMAKE_CXX_FLAGS_RELEASE         "${YUNI_COMMON_CC_OPTIONS_UNIX} ${YUNI_COMMON_CXX_OPTIONS} ${YUNI_HAS_COMPILER_OPTIMIZATION_LEVEL_4} -fomit-frame-pointer -fstrict-aliasing -momit-leaf-frame-pointer -fno-tree-pre -falign-loops -mfpmath=sse -msse -msse2 -Wuninitialized")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${YUNI_COMMON_CC_OPTIONS_UNIX} ${YUNI_COMMON_CXX_OPTIONS} ${YUNI_HAS_COMPILER_OPTIMIZATION_LEVEL_4} -fomit-frame-pointer -mfpmath=sse -msse -msse2")
	set(CMAKE_CXX_FLAGS_DEBUG           "${YUNI_COMMON_CC_OPTIONS_UNIX} ${YUNI_COMMON_CXX_OPTIONS} -g")

	set(CMAKE_C_FLAGS_RELEASE         "${YUNI_COMMON_CC_OPTIONS_UNIX} ${YUNI_HAS_COMPILER_OPTIMIZATION_LEVEL_4} -fomit-frame-pointer -fstrict-aliasing -momit-leaf-frame-pointer -fno-tree-pre -falign-loops -mfpmath=sse -msse -msse2 -Wuninitialized")
	set(CMAKE_C_FLAGS_RELWITHDEBINFO  "${YUNI_COMMON_CC_OPTIONS_UNIX} ${YUNI_HAS_COMPILER_OPTIMIZATION_LEVEL_4} -fomit-frame-pointer -mfpmath=sse -msse -msse2")
	set(CMAKE_C_FLAGS_DEBUG           "${YUNI_COMMON_CC_OPTIONS_UNIX} -g")
endif()

if(MINGW)
	set(CMAKE_CXX_FLAGS_RELEASE         "${YUNI_COMMON_CC_OPTIONS} ${YUNI_COMMON_CXX_OPTIONS} ${YUNI_HAS_COMPILER_OPTIMIZATION_LEVEL_4} -fomit-frame-pointer -fstrict-aliasing -momit-leaf-frame-pointer -fno-tree-pre -falign-loops -mthreads -Wuninitialized")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${YUNI_COMMON_CC_OPTIONS} ${YUNI_COMMON_CXX_OPTIONS} -mthreads")
	set(CMAKE_CXX_FLAGS_DEBUG           "${YUNI_COMMON_CC_OPTIONS} ${YUNI_COMMON_CXX_OPTIONS} -mthreads")

	set(CMAKE_C_FLAGS_RELEASE         "${YUNI_COMMON_CC_OPTIONS} ${YUNI_HAS_COMPILER_OPTIMIZATION_LEVEL_4} -fomit-frame-pointer -fstrict-aliasing -momit-leaf-frame-pointer -fno-tree-pre -falign-loops -mthreads -Wuninitialized")
	set(CMAKE_C_FLAGS_RELWITHDEBINFO  "${YUNI_COMMON_CC_OPTIONS} -mthreads")
	set(CMAKE_C_FLAGS_DEBUG           "${YUNI_COMMON_CC_OPTIONS} -mthreads")
endif()

if(MSVC)
	set(CMAKE_C_FLAGS_DEBUG   "${COMMON_MSVC_FLAGS} /MDd /GR /Ot /Od /EHsc /RTC1")
	set(CMAKE_CXX_FLAGS_DEBUG "${COMMON_MSVC_FLAGS} /MDd /GR /Ot /Od /EHsc /RTC1 /fp:except /RTCc")

	set(MSVC_RELEASE_FLAGS)

	# O2x: optimization
	set(MSVC_RELEASE_FLAGS "${MSVC_RELEASE_FLAGS} /O2")
	# Prefer speed instead of size
	set(MSVC_RELEASE_FLAGS "${MSVC_RELEASE_FLAGS} /Ot")
	# Omit frame pointer
	set(MSVC_RELEASE_FLAGS "${MSVC_RELEASE_FLAGS} /Oy")
	# Any suitable inlining
	set(MSVC_RELEASE_FLAGS "${MSVC_RELEASE_FLAGS} /Ob2")
	# Fiber-safe optimizations
	set(MSVC_RELEASE_FLAGS "${MSVC_RELEASE_FLAGS} /GT")
	# whole program / requires "Link time code generation"
	#set(MSVC_RELEASE_FLAGS "${MSVC_RELEASE_FLAGS} /GL")
	# No buffer security check
	set(MSVC_RELEASE_FLAGS "${MSVC_RELEASE_FLAGS} /GS-")
	# SSE2
	set(MSVC_RELEASE_FLAGS "${MSVC_RELEASE_FLAGS} /arch:SSE2")
	# Intrinsic functions
	set(MSVC_RELEASE_FLAGS "${MSVC_RELEASE_FLAGS} /Oi")
	# Multithreaded DLL
	set(MSVC_RELEASE_FLAGS "${MSVC_RELEASE_FLAGS} /MD")

	set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} ${MSVC_RELEASE_FLAGS}")
	set(CMAKE_C_FLAGS_RELEASE    "${CMAKE_C_FLAGS_RELEASE} ${MSVC_RELEASE_FLAGS}")
endif()


if(APPLE)
	# The SSE instruction set is mandatory
	set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_CXX_FLAGS_RELEASE} -msse2 -fvisibility=hidden")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${CMAKE_CXX_FLAGS_RELEASE} -msse2 -fvisibility=hidden")
	set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_CXX_FLAGS_DEBUG}   -msse2 -gfull -fvisibility=hidden")

	set(CMAKE_C_FLAGS_RELEASE           "${CMAKE_C_FLAGS_RELEASE} -msse2")
	set(CMAKE_C_FLAGS_RELWITHDEBINFO    "${CMAKE_C_FLAGS_RELEASE} -msse2")
	set(CMAKE_C_FLAGS_DEBUG             "${CMAKE_C_FLAGS_DEBUG}   -msse2 -gfull")
endif()
if (YUNI_HAS_COMPILER_DEBUG_INFORMATION)
	set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_C_FLAGS_DEBUG             "${CMAKE_C_FLAGS_DEBUG} -g")
endif()
if (YUNI_HAS_GCC_FLAG_GGDB)
	set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_CXX_FLAGS_DEBUG} -ggdb")
	set(CMAKE_C_FLAGS_DEBUG             "${CMAKE_C_FLAGS_DEBUG}   -ggdb")
endif()
if (YUNI_HAS_GCC_FLAG_GGDB3)
	set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3")
	set(CMAKE_C_FLAGS_DEBUG             "${CMAKE_C_FLAGS_DEBUG}   -ggdb3")
endif()

# NDEBUG
if(MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_CXX_FLAGS_RELEASE} /DNDEBUG")
	set(CMAKE_C_FLAGS_RELEASE           "${CMAKE_C_FLAGS_RELEASE} /DNDEBUG")
else()
	set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")
	set(CMAKE_C_FLAGS_RELEASE           "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")
endif()

set(CMAKE_CXX_FLAGS_RELEASE       "${CMAKE_CXX_FLAGS_RELEASE}      ${YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBUG  "${CMAKE_CXX_FLAGS_RELWITHDEBUG} ${YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS}")
set(CMAKE_CXX_FLAGS_DEBUG         "${CMAKE_CXX_FLAGS_DEBUG}        ${YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS}")

# Override
if (NOT "${YUNI_CXX_FLAGS_OVERRIDE}" STREQUAL "")
	set(CMAKE_CXX_FLAGS_RELEASE          "${YUNI_CXX_FLAGS_OVERRIDE}")
	set(CMAKE_CXX_FLAGS_RELWITHDEBUG     "${YUNI_CXX_FLAGS_OVERRIDE}")
	set(CMAKE_CXX_FLAGS_RELWITHDEBUGINFO "${YUNI_CXX_FLAGS_OVERRIDE}")
	set(CMAKE_CXX_FLAGS_DEBUG            "${YUNI_CXX_FLAGS_OVERRIDE}")
endif()

if(NOT "${YUNI_CXX_FLAGS_OVERRIDE_ADD_DEBUG}" STREQUAL "")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${YUNI_CXX_FLAGS_OVERRIDE_ADD_DEBUG}")
endif()
if(NOT "${YUNI_CXX_FLAGS_OVERRIDE_ADD_RELEASE}" STREQUAL "")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${YUNI_CXX_FLAGS_OVERRIDE_ADD_RELEASE}")
endif()
if(NOT "${YUNI_CXX_FLAGS_OVERRIDE_ADD_RELWITHDEBINFO}" STREQUAL "")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${YUNI_CXX_FLAGS_OVERRIDE_ADD_RELWITHDEBINFO}")
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG}")
endif()



#
# Extra - Mac OS X Bundles
#
if(APPLE)
	set(MACOSX_BUNDLE_COPYRIGHT "Yuni Framework - 2012")

	# The compiler flag -arch must be checked. The compiler might not have apple extensions
	include(CheckCXXCompilerFlag)
	check_cxx_compiler_flag("arch" YUNI_GCC_HAS_ARCH_FLAG)

	if (YUNI_GCC_HAS_ARCH_FLAG)
		if(YUNI_MACOX_UNIVERSAL_BINARIES)
			YMESSAGE("Enabled universal binaries (custom : ${YUNI_MACOX_UNIVERSAL_BINARIES})")
			set(CMAKE_OSX_ARCHITECTURES "${YUNI_MACOX_UNIVERSAL_BINARIES}") # ppc;i386;ppc64;x86_64
		else()
			YMESSAGE("Enabled universal binaries (i386, x86_64)")
			set(CMAKE_OSX_ARCHITECTURES "i686;x86_64") # ppc;i386;ppc64;x86_64
		endif()
	else()
		YWARNING("Universal binaries disabled. The compiler does not seem to support multiple platform architectures in a single binary)")
	endif()
endif()


