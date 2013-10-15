
YMESSAGE("      Using OpenAL from system")

# Use find_package for automatic detection
find_package(OpenAL)
if (OPENAL_FOUND)
	set(CMAKE_REQUIRED_INCLUDES "${OPENAL_INCLUDE_DIR}")
	YMESSAGE("OpenAL found : ${OPENAL_INCLUDE_DIR}")
	LIBYUNI_CONFIG_INCLUDE_PATH("both" "audio" "${OPENAL_INCLUDE_DIR}")
	LIBYUNI_CONFIG_LIB("both" "audio" "${OPENAL_LIBRARY}")
else()
	set(YUNI_CMAKE_ERROR 1)
	YMESSAGE(    "[!!] Impossible to find OpenAL. Please check your profile.")
	YMESSAGE(    " * Packages needed on Debian: libopenal-dev")
	YMESSAGE(    " * Packages needed on redhat: openal-devel")
endif()

# Mac OS X
if(APPLE)

	# Frameworks
	LIBYUNI_CONFIG_FRAMEWORK("both" "audio" OpenAL)

endif()


LIBYUNI_CONFIG_INCLUDE_PATH("both" "audio" "${OPENAL_INCLUDE_DIR}")
LIBYUNI_CONFIG_LIB("both" "audio" "${OPENAL_LIBRARY}")
include_directories("${OPENAL_INCLUDE_DIR}")

