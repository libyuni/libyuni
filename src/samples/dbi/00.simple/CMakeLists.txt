
find_package(Yuni COMPONENTS core dbi)
if(Yuni_FOUND)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Yuni_CXXFLAGS}")
	link_libraries("${Yuni_LIBS}")

	message(STATUS "Sample: DBI / Simple")
	add_executable(dbi_00_simple main.cpp)
endif(Yuni_FOUND)

