
find_package(Yuni COMPONENTS core parser)
if(Yuni_FOUND)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Yuni_CXXFLAGS}")
	link_libraries("${Yuni_LIBS}")

	message(STATUS "Sample: Parser / Calculator")
	add_executable(parser_00_calculator
		main.cpp
		calculator.cpp calculator.h calculator.hxx)
endif(Yuni_FOUND)

