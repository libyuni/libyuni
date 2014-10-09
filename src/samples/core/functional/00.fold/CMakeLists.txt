

Find_Package(Yuni COMPONENTS core)
IF(Yuni_FOUND)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Yuni_CXXFLAGS}")
	link_libraries("${Yuni_LIBS}")

	Message(STATUS "Sample: Core / Functional / Folding a collection")
	ADD_EXECUTABLE(functional_00_fold main.cpp)
ENDIF(Yuni_FOUND)
