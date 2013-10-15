

find_package(Yuni COMPONENTS media)
if(Yuni_FOUND)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Yuni_CXXFLAGS}")
	link_libraries("${Yuni_LIBS}")

	message(STATUS "Sample: Audio / Play a sound on a moving emitter")
	add_executable(audio_01_movingemitter main.cpp)
endif(Yuni_FOUND)

