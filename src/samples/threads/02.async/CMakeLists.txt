

Find_Package(Yuni COMPONENTS core)
IF(Yuni_FOUND)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Yuni_CXXFLAGS}")
	link_libraries("${Yuni_LIBS}")

	Message(STATUS "Sample: Multi-Threading / Async")
	ADD_EXECUTABLE(threads_02_async main.cpp)
ENDIF(Yuni_FOUND)

