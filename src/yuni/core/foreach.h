#ifndef __YUNI_CORE_FOREACH_H__
# define __YUNI_CORE_FOREACH_H__

# include "preprocessor/std.h"



/*!
** \brief foreach..else statement
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


#endif // __YUNI_CORE_FOREACH_H__
