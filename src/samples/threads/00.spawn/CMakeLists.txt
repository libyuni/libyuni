

Find_Package(Yuni COMPONENTS core)
IF(Yuni_FOUND)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Yuni_CXXFLAGS}")
	link_libraries("${Yuni_LIBS}")

	Message(STATUS "Sample: Multi-Threading / Spawn a new thread")
	ADD_EXECUTABLE(threads_00_spawn main.cpp)
ENDIF(Yuni_FOUND)

