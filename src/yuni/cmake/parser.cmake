
YMESSAGE_MODULE("Parser Generator")

LIBYUNI_CONFIG_LIB("both" "parser"        "yuni-static-parser")



include_directories("..")


set (SRC_PRIVATE_PARSER
	private/parser/info-charset.h
	private/parser/info-charset.cpp
)
source_group("Parser\\Private" FILES ${SRC_PRIVATE_PARSER})


set(SRC_PARSER
	parser/charset.h
	parser/charset.hxx
	parser/charsets.h
	parser/charsets.cpp
	parser/charsets.hxx
	parser/grammar.h
)
source_group("Parser" FILES ${SRC_PARSER})



add_Library(yuni-static-parser STATIC
	${SRC_PARSER}
	${SRC_PRIVATE_PARSER}
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
