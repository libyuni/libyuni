
find_package(Yuni COMPONENTS core parser)
if(Yuni_FOUND)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Yuni_CXXFLAGS}")
	link_libraries("${Yuni_LIBS}")

	message(STATUS "Sample: Parser / Calculator")
	add_executable(parser_00_generator  generator.cpp)
	add_executable(parser_00_calculator main.cpp calculator-grammar.cpp calculator-grammar.h calculator-grammar.hxx calculator-grammar.nodes.h)
endif(Yuni_FOUND)

