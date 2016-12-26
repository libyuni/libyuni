
ynmessage("      Using the OpenAL DevPack")

DEVPACK_IMPORT_OPENAL()
# We get OPENAL_INCLUDE_DIR from here.
# We get OPENAL_LIBRARY from here.

LIBYUNI_CONFIG_INCLUDE_PATH("both" "media" "${OPENAL_INCLUDE_DIR}")
LIBYUNI_CONFIG_LIB_RAW_COMMAND("both" "media" "${OPENAL_LIBRARY}")
include_directories(${OPENAL_INCLUDE_DIR})

