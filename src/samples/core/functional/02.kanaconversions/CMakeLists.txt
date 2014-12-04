

Find_Package(Yuni COMPONENTS core)
IF(Yuni_FOUND)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Yuni_CXXFLAGS}")
	link_libraries("${Yuni_LIBS}")

	Message(STATUS "Sample: Core / Functional / Converting Japanese kana")
	ADD_EXECUTABLE(functional_02_kanaconversions main.cpp)
ENDIF(Yuni_FOUND)
