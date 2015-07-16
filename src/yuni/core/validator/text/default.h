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
#ifndef __YUNI_CORE_VALIDATOR_DEFAULT_H__
# define __YUNI_CORE_VALIDATOR_DEFAULT_H__

# include "validator.h"
# include "default.private.h"


namespace Yuni
{
namespace Validator
{
namespace Text
{


	/*!
	** \brief Allow all strings / char
	*/
	class AllowAll : public IValidatorTmpl<AllowAll>
	{
	public:
		template<class U> bool validate(const U&) const
		{
			return true; // allow all
		}

	}; // class AllowAll


	/*!
	** \brief Deny all strings / char
	*/
	class DenyAll : public IValidatorTmpl<DenyAll>
	{
	public:
		template<class U> bool validate(const U&) const
		{
			return false; // deny all
		}

	}; // class DenyAll




	/*!
	** \brief Allow/Deny all strings/char by default, with a list of exceptions
	**
	** \code
	** Validator::Text::ExceptionList<Validator::allowByDefault> validator;
	** validator.exception("january");
	** validator.exception("december");
	**
	** std::cout << validator("february") << std::endl; // True
	** std::cout << validator("december") << std::endl; // False
	** \endcode
	**
	** A more convenient typedef is AllowByDefault and DenyByDefauly.
	** \see typedef AllowByDefault
	** \see typedef DenyByDefauly
	*/
	template<Yuni::Validator::DefaultPolicy DefaultPolicy>
	class ExceptionList final : public IValidatorTmpl<ExceptionList<DefaultPolicy> >
	{
	public:
		//! \name Constructors
		//@{
		/*!
		** \brief Default Constructor
		*/
		ExceptionList();
		/*!
		** \brief Copy constructor
		*/
		ExceptionList(const ExceptionList& rhs);
		/*!
		** \brief Copy constructor
		*/
		template<Yuni::Validator::DefaultPolicy OtherDefaultPolicy>
		ExceptionList(const ExceptionList<OtherDefaultPolicy>& rhs);
		/*!
		** \brief Constructor with a default exception list
		*/
		ExceptionList(const String::Vector& rhs);
		/*!
		** \brief Constructor with a default exception list
		*/
		ExceptionList(const String::List& rhs);
		//@}


		//! \name String validation
		//@{
		/*!
		** \brief Validate (or not) a string
		*/
		template<class U> bool validate(const U& s) const;
		//@}


		//! \name Add an exception in the list
		//@{
		/*!
		** \brief Add an exception
		**
		** \param text The exception (arbitrary string)
		*/
		template<class U> void exception(const U& text);

		/*!
		** \brief Append an arbitrary string
		** \param text A arbitrary string
		*/
		ExceptionList& operator += (const AnyString& text);

		template<Yuni::Validator::DefaultPolicy OtherDefaultPolicy>
		ExceptionList& operator += (const ExceptionList<OtherDefaultPolicy>& rhs);

		//! Operator += on a std::vector
		ExceptionList& operator += (const String::Vector& rhs);

		//! Operator += on a std::list
		ExceptionList& operator += (const String::List& rhs);

		//! Operator << on an arbitrary string
		ExceptionList& operator << (const AnyString& rhs);

		//! Operator << on another exception list
		template<Yuni::Validator::DefaultPolicy OtherDefaultPolicy>
		ExceptionList& operator << (const ExceptionList<OtherDefaultPolicy>& rhs);

		//! Operator << on a std::vector
		ExceptionList& operator << (const String::Vector& rhs);

		//! Operator << on a std::list
		ExceptionList& operator << (const String::List& rhs);
		//@}


		//! \name Reset the exception list
		//@{
		//! Operator =
		template<Yuni::Validator::DefaultPolicy OtherDefaultPolicy>
		ExceptionList& operator = (const ExceptionList<OtherDefaultPolicy>& rhs);
		//! Operator =
		ExceptionList& operator = (const String::Vector& rhs);
		//! Operator =
		ExceptionList& operator = (const String::List& rhs);
		//@}

	private:
		//! List of exceptions
		String::Vector pExceptionList;

	}; // class ExceptionList





	/*!
	** \brief Allow all strings/char by default, with an exception list
	*/
	typedef ExceptionList<Validator::allowByDefault>  AllowByDefault;

	/*!
	** \brief Deny all strings/char by default, with an exception list
	*/
	typedef ExceptionList<Validator::denyByDefault>   DenyByDefault;




} // namespace Text
} // namespace Validator
} // namespace Yuni

# include "default.hxx"

#endif // __YUNI_CORE_VALIDATOR_DEFAULT_H__
