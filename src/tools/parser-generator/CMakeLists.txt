project(yuni-config CXX)
cmake_minimum_required(VERSION 2.8)

YMESSAGE_TITLE("[tool]" " yuni-parser-generator")


include("${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/common-settings.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/modules.cmake")


include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../..")



add_executable(yuni-parser-generator   main.cpp)

target_link_libraries(yuni-parser-generator yuni-static-parser yuni-static-core yuni-static-uuid uuid rpcrt4.lib)
