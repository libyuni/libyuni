

Find_Package(Yuni COMPONENTS core)
IF(Yuni_FOUND)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Yuni_CXXFLAGS}")
	link_libraries("${Yuni_LIBS}")

	message(STATUS "Sample: Multi-Threading / Using Condition Variable for Thread Synchronization")
	add_executable(threads_03_condition main.cpp)
endif(Yuni_FOUND)

