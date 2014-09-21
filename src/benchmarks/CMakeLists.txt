
# yuni sources
include_directories("..")

# Compilation Flags
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/../compiler-flags-debug-cc"    YN_FLAGS_C_DEBUG)
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/../compiler-flags-release-cc"  YN_FLAGS_C_RELEASE)
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/../compiler-flags-debug-cxx"   YN_FLAGS_CXX_DEBUG)
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/../compiler-flags-release-cxx" YN_FLAGS_CXX_RELEASE)

set(CMAKE_C_FLAGS_DEBUG     "${YN_FLAGS_C_DEBUG}")
set(CMAKE_C_FLAGS_RELEASE   "${YN_FLAGS_C_RELEASE}")
set(CMAKE_CXX_FLAGS_DEBUG   "${YN_FLAGS_CXX_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "${YN_FLAGS_CXX_RELEASE}")




add_subdirectory(jobs)

