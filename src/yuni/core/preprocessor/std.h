#ifndef __YUNI_CORE_PREPROCESSOR_STANDARD_H__
# define __YUNI_CORE_PREPROCESSOR_STANDARD_H__

/* !!! "C compatibility" header !!! */


/*!
** \brief String concatenation
*/
# define YUNI_JOIN(X,Y)  X ## Y

/*!
** \brief Convenient define to deal with temporary (or not) unused parameter
*/
# define YUNI_UNUSED_ARGUMENT(X) (void)(X)



# define __YN_STRINGIZE(X) #X

/*!
** \brief Transform X into a string
*/
# define YUNI_STRINGIZE(X)  __YN_STRINGIZE(X)

/*!
** \brief Test if something is empty
**
** \code
** #include <iostream>
** #define MY_EMPTY_STRING
**
** int main()
** {
**	if (YUNI_IS_EMPTY(MY_EMPTY_STRING))
**		std::cout << "empty !\n";
**	return 0;
** }
** \endcode
*/
#define YUNI_IS_EMPTY(X)  (YUNI_STRINGIZE(X)[0] == '\0')



/*! The identity function */
# define YUNI_IDENTITY(...) __VA_ARGS__

/*! An empty value */
# define YUNI_EMPTY

/*! Comma */
# define YUNI_COMMA ,

/*! Semicolon */
# define YUNI_SEMICOLON ;

/*! Dot */
# define YUNI_DOT .

/*! Minus */
# define YUNI_MINUS -



#endif /* __YUNI_CORE_PREPROCESSOR_STANDARD_H__ */
