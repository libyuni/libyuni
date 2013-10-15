

find_package(Yuni COMPONENTS media core)
if(Yuni_FOUND)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Yuni_CXXFLAGS}")
	link_libraries("${Yuni_LIBS}")

	message(STATUS "Sample: Audio / Load sounds and play them")
	add_executable(audio_00_loadandplay 00.loadandplay.cpp)
endif(Yuni_FOUND)

