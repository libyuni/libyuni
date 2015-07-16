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
#include "../../yuni.h"
#include <map>
#include "option.h"
#include <string.h>
#include "../validator/text/default.h"



namespace Yuni
{
namespace GetOpt
{

	/*!
	** \brief A command line options parser
	** \ingroup Core
	*/
	class YUNI_DECL Parser final
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default constructor
		*/
		Parser();
		/*!
		** \brief Destructor
		*/
		~Parser();
		//@}


		//! \name Option management
		//@{
		/*!
		** \brief Add an option
		**
		** \param[in] var The variable where the value(s) will be written
		** \param shortname The short name of the option (a single char)
		*/
		template<class U>
		void add(U& var, char shortname);

		/*!
		** \brief Add an option
		**
		** \param[in] var The variable where the value(s) will be written
		** \param shortname The short name of the option (a single char)
		** \param longname The long name of the option
		*/
		template<class U>
		void add(U& var, char shortname, const AnyString& longname);

		/*!
		** \brief Add an option
		**
		** \param[in] var The variable where the value(s) will be written
		** \param shortname The short name of the option (a single char)
		** \param longname The long name of the option
		** \param description The description of the option (used in the help usage)
		*/
		template<class U>
		void add(U& var, char shortname, const AnyString& longname, const AnyString& description);

		/*!
		** \brief Add an option
		**
		** \param[in] var The variable where the value(s) will be written
		** \param shortname The short name of the option (a single char)
		** \param longname The long name of the option
		** \param description The description of the option (used in the help usage)
		** \param visible True if the option is visible from the help usage
		*/
		template<class U>
		void add(U& var, char shortname, const AnyString& longname, const AnyString& description, bool visible);

		/*!
		** \brief Add an option that does not require an additional parameter
		**
		** \param[in] var The variable where the value(s) will be written
		** \param shortname The short name of the option (a single char)
		*/
		template<class U>
		void addFlag(U& var, char shortname);

		/*!
		** \brief Add an option that does not require an additional parameter
		**
		** \param[in] var The variable where the value(s) will be written
		** \param shortname The short name of the option (a single char)
		** \param longname The long name of the option
		*/
		template<class U>
		void addFlag(U& var, char shortname, const AnyString& longname);

		/*!
		** \brief Add an option that does not require an additional parameter
		**
		** \param[in] var The variable where the value(s) will be written
		** \param shortname The short name of the option (a single char)
		** \param longname The long name of the option
		** \param description The description of the option (used in the help usage)
		*/
		template<class U>
		void addFlag(U& var, char shortname, const AnyString& longname, const AnyString& description);

		/*!
		** \brief Add an option that does not require an additional parameter
		**
		** \param[in] var The variable where the value(s) will be written
		** \param shortname The short name of the option (a single char)
		** \param longname The long name of the option
		** \param description The description of the option (used in the help usage)
		** \param visible True if the option is visible from the help usage
		*/
		template<class U>
		void addFlag(U& var, char shortname, const AnyString& longname, const AnyString& description, bool visible);

		//! Remove all options
		void clear();
		//@}


		//! \name Remaining arguments
		//@{
		/*!
		** \brief Set the target variable where remaining arguments will be written
		**
		** In this example, 'value2' and 'value4' will be considered as remaining arguments :
		** \code
		** ./myprogram -n value1 value2 -p value3 value4
		** \endcode
		*/
		template<class U> void remainingArguments(U& var);
		//@}


		//! \name Command line parsing
		//@{
		/*!
		** \brief Parse the command line
		**
		** \param argc The count of arguments
		** \param argv The list of arguments
		** \return False if the program should abort
		*/
		bool operator () (int argc, char* argv[]);
		//@}


		//! \name Help usage
		//@{
		/*!
		** \brief Add a text paragraph after the last added option
		**
		** \param text Any text of an arbitrary length
		*/
		void addParagraph(const AnyString& text);

		/*!
		** \brief Generate and display an help usage
		**
		** \note If you want your own help usage, add the option ('h', "help")
		** to handle yourself the behavior.
		*/
		void helpUsage(const char* argv0);
		//@}


		//! \name Errors
		//@{
		/*!
		** \brief The count of errors that have been encountered
		*/
		uint errors() const;

		//! Get if unknown args are ignored (not considered as errors)
		bool ignoreUnknownArgs() const;
		//! Set if unknown args should be ignored
		void ignoreUnknownArgs(bool ignore);
		//@}


	private:
		//! Predicate to compare two CString
		struct CStringComparison
		{
			bool operator()(const char* s1, const char* s2) const {return ::strcmp(s1, s2) < 0;}
		};

		//! IOption
		typedef Private::GetOptImpl::IOption IOption;
		//! Option list (order given by the user)
		typedef std::vector<IOption*> OptionList;
		//! All options ordered by their short name
		typedef std::map<char, IOption*> OptionsOrderedByShortName;
		//! All options ordered by their long name
		typedef std::map<const char*, IOption*, CStringComparison> OptionsOrderedByLongName;

	private:
		//! Append a new option
		void appendOption(IOption* option, char shortname);
		//! Append a new short option
		void appendShortOption(IOption* option, char shortname);

	private:
		//! All existing options
		OptionList pAllOptions;
		//! All options ordered by their short name
		OptionsOrderedByShortName pShortNames;
		//! All options ordered by their long name
		OptionsOrderedByLongName pLongNames;
		//! Options for remaining arguments
		IOption* pRemains;

		//! Count of error
		uint pErrors;
		//! Flag to ignore (or not) unknown arguments
		bool pIgnoreUnknownArgs;

		// A friend
		friend class Private::GetOptImpl::Context;

	}; // class Parser





} // namespace GetOpt
} // namespace Yuni

# include "parser.hxx"

