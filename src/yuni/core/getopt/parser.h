#ifndef __YUNI_CORE_GETOPT_PARSER_H__
# define __YUNI_CORE_GETOPT_PARSER_H__

# include "../../yuni.h"
# include <map>
# include "option.h"
# include <string.h>
# include "../validator/text/default.h"


namespace Yuni
{
namespace GetOpt
{

	/*!
	** \brief A command line options parser
	*/
	class Parser final
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

#endif // __YUNI_CORE_GETOPT_PARSER_H__
