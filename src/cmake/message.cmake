set(YUNI_ERROR_HAS_OCCURED   false)


macro(ynmessage msg)
	if(UNIX)
		message(STATUS "[1;30m{yuni}[0m  ${msg}")
	else()
		message(STATUS "{yuni}  ${msg}")
	endif()

endmacro()


macro(ynmessage_bold msg)
	if(UNIX)
		message(STATUS "[1;30m{yuni}[0m  [1;37m[1m${msg}[0m")
	else()
		message(STATUS "{yuni}  ${msg}")
	endif()
endmacro()


macro(ynmessage_title msg1 msg2)
	if(UNIX)
		message(STATUS "[1;30m{yuni}[0m  [1;32m${msg1}[0m[1;37m[1m${msg2}[0m")
	else()
		message(STATUS "{yuni}  ${msg1}${msg2}")
	endif()
endmacro()


macro(ynmessage_module msg)
	ynmessage_title("[module] " "${msg}")
endmacro()


macro(YWARNING msg)
	if(UNIX)
		message(STATUS "[1;33m{yuni}  [warning][0m ${msg}")
	else()
		message(STATUS "{yuni}  [WARNING] ${msg}")
	endif()
endmacro()


macro(ynerror msg)
	if(UNIX)
		message(STATUS "[1;31m{yuni}  [error][0m ${msg}")
	else()
		message(STATUS "{yuni}  [ERROR] ${msg}")
	endif()
	set(YUNI_ERROR_HAS_OCCURED  true)
endmacro()


macro(YFATAL msg)
	if(UNIX)
		message(FATAL_ERROR "[1;31m{yuni}  [error][0m ${msg}")
	else()
		message(FATAL_ERROR "{yuni}  [ERROR] ${msg}")
	endif()

	set(YUNI_ERROR_HAS_OCCURED  true)
endmacro()
