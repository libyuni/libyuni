

Find_Package(Yuni COMPONENTS core)
IF(Yuni_FOUND)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Yuni_CXXFLAGS}")
	link_libraries("${Yuni_LIBS}")

	Message(STATUS "Sample: Core / Functional / Working with views on collections")
	ADD_EXECUTABLE(functional_01_views main.cpp)
ENDIF(Yuni_FOUND)
