
find_package(Yuni COMPONENTS core parser)
if(Yuni_FOUND)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Yuni_CXXFLAGS}")
	link_libraries("${Yuni_LIBS}")

	message(STATUS "Sample: Parser / JSON")
	add_executable(parser_01_json
		main.cpp
		json.cpp json.h json.hxx)
endif(Yuni_FOUND)

