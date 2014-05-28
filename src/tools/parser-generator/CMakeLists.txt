project(yuni-config CXX)
cmake_minimum_required(VERSION 2.8)

YMESSAGE_TITLE("[tool]" " yuni-parser-generator")


include("${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/common-settings.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/modules.cmake")


include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../..")



add_executable(yuni-parser-generator   main.cpp)

set(ADDITIONAL_LIBS "")
if (WIN32 OR WIN64)
	set(ADDITIONAL_LIBS rpcrt4.lib)
else()
	if (NOT APPLE)
		set(ADDITIONAL_LIBS uuid)
	endif()
endif()

target_link_libraries(yuni-parser-generator yuni-static-parser yuni-static-core yuni-static-uuid "${ADDITIONAL_LIBS}")
