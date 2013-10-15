
find_package(Yuni COMPONENTS core marshal)
if(Yuni_FOUND)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Yuni_CXXFLAGS}")
	link_libraries("${Yuni_LIBS}")

	message(STATUS "Sample: Marshal / JSON Export")
	add_executable(marshal_00_json-export  main.cpp)
endif(Yuni_FOUND)

