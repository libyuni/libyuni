

ynmessage("      Building Lua from source using the Unix DevPack [ Windows FIXME !!! ]")

if(UNIX AND NOT APPLE)
	check_include_files("stdio.h;readline/readline.h" YUNI_HAS_READLINE_H)
	if(NOT YUNI_HAS_READLINE_H)
		set(YUNI_CMAKE_ERROR 1)
		ynmessage("[!!] Impossible to find readline/readline.h")
		ynmessage(" * Packages needed on Debian: libreadline-dev")
		ynmessage(" * Packages needed on Fedora: readline-devel.i686")
	endif()
endif()

# Headers for Lua
DEVPACK_IMPORT_LUA()
# We get YUNI_EXT_LUA_INCLUDE from here.
# We get YUNI_EXT_LUA_LIB from here.

LIBYUNI_CONFIG_LIB_RAW_COMMAND("both" "lua" "${YUNI_EXT_LUA_LIB}")

