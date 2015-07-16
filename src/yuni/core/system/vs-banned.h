/*
** YUNI's default license is the GNU Lesser Public License (LGPL), with some
** exclusions (see below). This basically means that you can get the full source
** code for nothing, so long as you adhere to a few rules.
**
** Under the LGPL you may use YUNI for any purpose you wish, and modify it if you
** require, as long as you:
**
** Pass on the (modified) YUNI source code with your software, with original
** copyrights intact :
**  * If you distribute electronically, the source can be a separate download
**    (either from your own site if you modified YUNI, or to the official YUNI
**    website if you used an unmodified version) – just include a link in your
**    documentation
**  * If you distribute physical media, the YUNI source that you used to build
**    your application should be included on that media
** Make it clear where you have customised it.
**
** In addition to the LGPL license text, the following exceptions / clarifications
** to the LGPL conditions apply to YUNI:
**
**  * Making modifications to YUNI configuration files, build scripts and
**    configuration headers such as yuni/platform.h in order to create a
**    customised build setup of YUNI with the otherwise unmodified source code,
**    does not constitute a derived work
**  * Building against YUNI headers which have inlined code does not constitute a
**    derived work
**  * Code which subclasses YUNI classes outside of the YUNI libraries does not
**    form a derived work
**  * Statically linking the YUNI libraries into a user application does not make
**    the user application a derived work.
**  * Using source code obsfucation on the YUNI source code when distributing it
**    is not permitted.
** As per the terms of the LGPL, a "derived work" is one for which you have to
** distribute source code for, so when the clauses above define something as not
** a derived work, it means you don't have to distribute source code for it.
** However, the original YUNI source code with all modifications must always be
** made available.
*/
* banned.h - list of Microsoft Security Development Lifecycle (SDL) banned APIs
*
* Purpose:
*       This include file contains a list of banned APIs which should not be used in new code and
*       removed from legacy code over time.
*
* History
* 01-Jan-2006 - mikehow - Initial Version
* 22-Apr-2008 - mikehow	- Updated to SDL 4.1, commented out recommendations and added memcpy
* 26-Jan-2009 - mikehow - Updated to SDL 5.0, made the list sane, added SDL compliance levels
* 10-Feb-2009 - mikehow - Updated based on feedback from MS Office
* 12-May-2009 - jpardue - Added wmemcpy
* 08-Jul-2009 - mikehow - Fixed header #ifndef/#endif logic, made the SDL recommended compliance level name more obvious
* 05-Nov-2009 - mikehow	- Added vsnprintf (ANSI version of _vsnprintf)
* 01-Jan-2010 - mikehow - Added better strsafe integration, now the following works:
*							#include "strsafe.h"
*							#include "banned.h"
* 04-Jun-2010 - mikehow - Small "#if" bug fix
* 16-Jun-2011 - mikehow	- Added the two _CRT_SECURE_xxxxx macros
* 07-Jul-2011 - mikehow - Bugfix when using recommended banned functions and StrSafe. Locally surpressed C4005 warnings
*
*
***/

#ifndef _INC_BANNED
#	define _INC_BANNED

#	if defined(_MSC_VER)
#		pragma once

// Flip the 'auto-migrate' functionality in VC++
// Some functions, such as strcpy() are changed to safer functions by the compiler
// More info: http://blogs.msdn.com/b/sdl/archive/2010/02/16/vc-2010-and-memcpy.aspx
#ifndef _SDL_DONT_AUTO_FIX

#	pragma warning(push)
#	pragma warning(disable: 4005)

	// strcpy etc
#	define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES			(1)

	// memcpy etc
#	define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_MEMORY	(1)

#	pragma warning(pop)

#endif

		// SDL 5.0 and later Requirements
#		if defined(_STRSAFE_H_INCLUDED_) && !defined(STRSAFE_NO_DEPRECATE)

			// Only deprecate what's not already deprecated by StrSafe
#			pragma deprecated (_mbscpy, _mbccpy)
#			pragma deprecated (strcatA, strcatW, _mbscat, StrCatBuff, StrCatBuffA, StrCatBuffW, StrCatChainW, _tccat, _mbccat)
#			pragma deprecated (strncpy, wcsncpy, _tcsncpy, _mbsncpy, _mbsnbcpy, StrCpyN, StrCpyNA, StrCpyNW, StrNCpy, strcpynA, StrNCpyA, StrNCpyW, lstrcpyn, lstrcpynA, lstrcpynW)
#			pragma deprecated (strncat, wcsncat, _tcsncat, _mbsncat, _mbsnbcat, lstrncat, lstrcatnA, lstrcatnW, lstrcatn)
#			pragma deprecated (IsBadWritePtr, IsBadHugeWritePtr, IsBadReadPtr, IsBadHugeReadPtr, IsBadCodePtr, IsBadStringPtr)
#			pragma deprecated (memcpy, RtlCopyMemory, CopyMemory, wmemcpy)

#		else
			// StrSafe not loaded, so deprecate everything!
#			pragma deprecated (strcpy, strcpyA, strcpyW, wcscpy, _tcscpy, _mbscpy, StrCpy, StrCpyA, StrCpyW, lstrcpy, lstrcpyA, lstrcpyW, _tccpy, _mbccpy, _ftcscpy)
#			pragma deprecated (strcat, strcatA, strcatW, wcscat, _tcscat, _mbscat, StrCat, StrCatA, StrCatW, lstrcat, lstrcatA, lstrcatW, StrCatBuff, StrCatBuffA, StrCatBuffW, StrCatChainW, _tccat, _mbccat, _ftcscat)
#			pragma deprecated (sprintfW, sprintfA, wsprintf, wsprintfW, wsprintfA, sprintf, swprintf, _stprintf)
#			pragma deprecated (wvsprintf, wvsprintfA, wvsprintfW, vsprintf, _vstprintf, vswprintf)
#			pragma deprecated (strncpy, wcsncpy, _tcsncpy, _mbsncpy, _mbsnbcpy, StrCpyN, StrCpyNA, StrCpyNW, StrNCpy, strcpynA, StrNCpyA, StrNCpyW, lstrcpyn, lstrcpynA, lstrcpynW)
#			pragma deprecated (strncat, wcsncat, _tcsncat, _mbsncat, _mbsnbcat, StrCatN, StrCatNA, StrCatNW, StrNCat, StrNCatA, StrNCatW, lstrncat, lstrcatnA, lstrcatnW, lstrcatn)
#			pragma deprecated (gets, _getts, _gettws)
#			pragma deprecated (IsBadWritePtr, IsBadHugeWritePtr, IsBadReadPtr, IsBadHugeReadPtr, IsBadCodePtr, IsBadStringPtr)
#			pragma deprecated (memcpy, RtlCopyMemory, CopyMemory, wmemcpy)
#		endif //defined(_STRSAFE_H_INCLUDED_) && !defined(STRSAFE_NO_DEPRECATE)

// SDL 5.0 and later Recommendations
#		if defined(_SDL_BANNED_RECOMMENDED)
#			if defined(_STRSAFE_H_INCLUDED_) && !defined(STRSAFE_NO_DEPRECATE)
				// Only deprecate what's not already deprecated by StrSafe
#				pragma deprecated (wnsprintf, wnsprintfA, wnsprintfW)
#				pragma deprecated (vsnprintf, wvnsprintf, wvnsprintfA, wvnsprintfW)
#				pragma deprecated (strtok, _tcstok, wcstok, _mbstok)
#				pragma deprecated (makepath, _tmakepath,  _makepath, _wmakepath)
#				pragma deprecated (_splitpath, _tsplitpath, _wsplitpath)
#				pragma deprecated (scanf, wscanf, _tscanf, sscanf, swscanf, _stscanf, snscanf, snwscanf, _sntscanf)
#				pragma deprecated (_itoa, _itow, _i64toa, _i64tow, _ui64toa, _ui64tot, _ui64tow, _ultoa, _ultot, _ultow)
#				pragma deprecated (CharToOem, CharToOemA, CharToOemW, OemToChar, OemToCharA, OemToCharW, CharToOemBuffA, CharToOemBuffW)
#				pragma deprecated (alloca, _alloca)
#				pragma deprecated (strlen, wcslen, _mbslen, _mbstrlen, StrLen, lstrlen)
#				pragma deprecated (ChangeWindowMessageFilter)
#			else
				// StrSafe not loaded, so deprecate everything!
#				pragma deprecated (wnsprintf, wnsprintfA, wnsprintfW, _snwprintf, _snprintf, _sntprintf)
#				pragma deprecated (_vsnprintf, vsnprintf, _vsnwprintf, _vsntprintf, wvnsprintf, wvnsprintfA, wvnsprintfW)
#				pragma deprecated (strtok, _tcstok, wcstok, _mbstok)
#				pragma deprecated (makepath, _tmakepath,  _makepath, _wmakepath)
#				pragma deprecated (_splitpath, _tsplitpath, _wsplitpath)
#				pragma deprecated (scanf, wscanf, _tscanf, sscanf, swscanf, _stscanf, snscanf, snwscanf, _sntscanf)
#				pragma deprecated (_itoa, _itow, _i64toa, _i64tow, _ui64toa, _ui64tot, _ui64tow, _ultoa, _ultot, _ultow)
#				pragma deprecated (CharToOem, CharToOemA, CharToOemW, OemToChar, OemToCharA, OemToCharW, CharToOemBuffA, CharToOemBuffW)
#				pragma deprecated (alloca, _alloca)
#				pragma deprecated (strlen, wcslen, _mbslen, _mbstrlen, StrLen, lstrlen)
#				pragma deprecated (ChangeWindowMessageFilter)
#			endif // StrSafe
#		endif // SDL recommended

#	endif // _MSC_VER_

#endif  // _INC_BANNED


