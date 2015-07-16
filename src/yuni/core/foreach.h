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
#pragma once
#include "preprocessor/std.h"



/*!
** \brief foreach..else statement
** \ingroup Core
**
** \code
** std::vector<int> list;
** list.push_back(2);
** list.push_back(3);
**
** foreach (int i, list)
**	std::cout << i << std::endl;
** \endcode
**
** \code
** std::vector<int> list; // empty list
**
** foreach (int i, list)
**	std::cout << "got " << i << std::endl;
** else
**	std::cout << "got nothing" << std::endl;
** \endcode
**
** \note C++11 is required (auto keyword)
*/
# define YUNI_FOREACH(var, container) \
		for (bool YUNI_JOIN(yn_continue_, __LINE__) = true; YUNI_JOIN(yn_continue_, __LINE__); ) \
			for (auto& YUNI_JOIN(yn_list_, __LINE__) = (container); YUNI_JOIN(yn_continue_, __LINE__); YUNI_JOIN(yn_continue_, __LINE__) = false) \
				if (YUNI_JOIN(yn_list_, __LINE__).size() > 0) \
					for (auto _yn_each_it = YUNI_JOIN(yn_list_, __LINE__).begin(); YUNI_JOIN(yn_continue_, __LINE__); YUNI_JOIN(yn_continue_, __LINE__) = false) \
						for (bool _foreach_continue = true; _foreach_continue && _yn_each_it != YUNI_JOIN(yn_list_, __LINE__).end(); ++_yn_each_it) \
							for (var = *_yn_each_it; !(_foreach_continue = !_foreach_continue); _foreach_continue = false) \
				/* else statement [optional] */


# define YUNI_REVERSE_FOREACH(var, container) \
		for (bool YUNI_JOIN(yn_continue_, __LINE__) = true; YUNI_JOIN(yn_continue_, __LINE__); ) \
			for (auto& YUNI_JOIN(yn_list_, __LINE__) = (container); YUNI_JOIN(yn_continue_, __LINE__); YUNI_JOIN(yn_continue_, __LINE__) = false) \
				if (YUNI_JOIN(yn_list_, __LINE__).size() > 0) \
					for (auto _yn_each_it = YUNI_JOIN(yn_list_, __LINE__).rbegin(); YUNI_JOIN(yn_continue_, __LINE__); YUNI_JOIN(yn_continue_, __LINE__) = false) \
						for (bool _foreach_continue = true; _foreach_continue && _yn_each_it != YUNI_JOIN(yn_list_, __LINE__).rend(); ++_yn_each_it) \
							for (var = *_yn_each_it; !(_foreach_continue = !_foreach_continue); _foreach_continue = false) \
				/* else statement [optional] */


