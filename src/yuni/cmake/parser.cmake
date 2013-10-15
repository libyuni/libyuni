
YMESSAGE_MODULE("Parser Generator")

LIBYUNI_CONFIG_LIB("both" "parser"        "yuni-static-parser")

include_directories("..")


# generate cpp file
file(READ "private/parser/peg/__parser.include.cpp.template" cpp_contents)
string(REPLACE "\\" "\\\\" cpp_contents "${cpp_contents}")
string(REPLACE "\"" "\\\"" cpp_contents "${cpp_contents}")
string(REPLACE "\n" "\\n\";\n\tout << \"" cpp_contents "${cpp_contents}")
set(cpp_contents "\ntemplate<class StreamT>\nstatic inline void PrepareCPPInclude(StreamT& out)\n{\n\tout << \"${cpp_contents}\";\n}\n")
string(REPLACE "<< \"\\n\";" "<< '\\n';" cpp_contents "${cpp_contents}")
file(WRITE "private/parser/peg/__parser.include.cpp.hxx" "${cpp_contents}")



set(SRC_PARSER
	parser/peg/grammar.h
	parser/peg/grammar.hxx
	parser/peg/grammar.cpp
	parser/peg/node.h
	parser/peg/node.hxx
	parser/peg/node.cpp
	parser/peg/export-cpp.cpp
	parser/peg/export-dot.cpp
	private/parser/peg/__parser.include.cpp.hxx
)
source_group("Parser\\Generator" FILES ${SRC_PARSER})



add_Library(yuni-static-parser STATIC
	${SRC_PARSER}
)

# Setting output path
SET_TARGET_PROPERTIES(yuni-static-parser PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${YUNI_OUTPUT_DIRECTORY}/lib")

# Installation
install(TARGETS yuni-static-parser ARCHIVE DESTINATION lib/${YUNI_VERSIONED_INST_PATH})

# Install net-related headers
install(
	DIRECTORY parser
	DESTINATION include/${YUNI_VERSIONED_INST_PATH}/yuni
	FILES_MATCHING
		PATTERN "*.h"
		PATTERN "*.hxx"
	PATTERN ".svn" EXCLUDE
	PATTERN "CMakeFiles" EXCLUDE
	PATTERN "cmake" EXCLUDE
)
