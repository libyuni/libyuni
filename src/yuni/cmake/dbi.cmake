
YMESSAGE_MODULE("DBI (Database independant)  (EXPERIMENTAL)")

LIBYUNI_CONFIG_LIB("both"   "dbi"  "yuni-static-dbi")

# only for sqlite
LIBYUNI_CONFIG_LIB("both" "dbi"  "dl")


# Sources for parsing JSON are imported from nany and no relative paths
# is currently provided
include_directories("..")

set (SRC_PRIVATE_DBI
	private/dbi/connector-data.h
	private/dbi/connector-data.hxx
	private/dbi/connector-data.cpp
	private/dbi/channel.h
	private/dbi/channel.hxx
	private/dbi/channel.cpp

	private/dbi/adapter/sqlite/sqlite3.c
	private/dbi/adapter/sqlite/sqlite3.h
	private/dbi/adapter/sqlite/sqlite3ext.h
)
source_group("DBI\\Private" FILES ${SRC_PRIVATE_DBI})


set (SRC_PRIVATE_DBI_ADAPTER_SQLITE
	private/dbi/adapter/sqlite/sqlite3.c
	private/dbi/adapter/sqlite/sqlite3.h
	private/dbi/adapter/sqlite/sqlite3ext.h
)
source_group("DBI\\Adapter\\SQLite" FILES ${SRC_PRIVATE_DBI_ADAPTER_SQLITE})


if (NOT MSVC)
	check_c_compiler_flag("-w" YUNI_HAS_GCC_NOWARNING)
	if (YUNI_HAS_GCC_NOWARNING)
		set_source_files_properties(private/dbi/adapter/sqlite/sqlite3.c PROPERTIES COMPILE_FLAGS -w)
	endif()
else()
	check_c_compiler_flag("/nowarn" YUNI_HAS_VS_NOWARNING)
	if (YUNI_HAS_VS_NOWARNING)
		set_source_files_properties(private/dbi/adapter/sqlite/sqlite3.c PROPERTIES COMPILE_FLAGS /nowarn)
	endif()
endif()


set(SRC_DBI
	dbi/dbi.h
	dbi/connector-pool.h
	dbi/connector-pool.hxx
	dbi/connector-pool.cpp
	dbi/error.h
	dbi/fwd.h
	dbi/transaction.h
	dbi/transaction.hxx
	dbi/transaction.cpp
	dbi/auto-commit.h
	dbi/row.h
	dbi/row.hxx
	dbi/column.h
	dbi/column.hxx
	dbi/settings.h
	dbi/settings.hxx
	dbi/cursor.h
	dbi/cursor.hxx
	dbi/cursor.cpp
	dbi/utils.h
	dbi/utils.cpp

	dbi/adapter.h
	dbi/adapter/entries.h
	dbi/adapter/adapter.h
	dbi/adapter/sqlite.h
	dbi/adapter/sqlite.cpp
)
source_group("DBI" FILES ${SRC_DBI})



add_Library(yuni-static-dbi STATIC
	${SRC_DBI}
	${SRC_PRIVATE_DBI_ADAPTER_SQLITE}
	${SRC_PRIVATE_DBI}
)

# Setting output path
SET_TARGET_PROPERTIES(yuni-static-dbi PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${YUNI_OUTPUT_DIRECTORY}/lib")

# Installation
install(TARGETS yuni-static-dbi ARCHIVE DESTINATION lib/${YUNI_VERSIONED_INST_PATH})

# Install net-related headers
install(
	DIRECTORY dbi
	DESTINATION include/${YUNI_VERSIONED_INST_PATH}/yuni
	FILES_MATCHING
		PATTERN "*.h"
		PATTERN "*.hxx"
	PATTERN ".svn" EXCLUDE
	PATTERN "CMakeFiles" EXCLUDE
	PATTERN "cmake" EXCLUDE
)
