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



namespace Yuni
{
namespace GetOpt
{

	template<class U>
	inline void Parser::add(U& var, char shortname)
	{
		// The new option
		IOption* option = (IOption*) new Private::GetOptImpl::Option<U, true>(var, shortname);
		// append the new option
		appendShortOption(option, shortname);
	}


	template<class U>
	inline void Parser::addFlag(U& var, char shortname)
	{
		// The new option
		IOption* option = (IOption*) new Private::GetOptImpl::Option<U, true, false>(var, shortname);
		// append the new option
		appendShortOption(option, shortname);
	}


	template<class U>
	inline void Parser::add(U& var, char shortname, const AnyString& longname)
	{
		IOption* option = (IOption*) new Private::GetOptImpl::Option<U, true>(var, shortname, longname);
		// append the new option
		appendOption(option, shortname);
	}


	template<class U>
	inline void Parser::add(U& var, char shortname, const AnyString& longname, const AnyString& description)
	{
		IOption* option = (IOption*) new Private::GetOptImpl::Option<U, true>(var, shortname, longname, description);
		// append the new option
		appendOption(option, shortname);
	}



	template<class U>
	inline void Parser::addFlag(U& var, char shortname, const AnyString& longname)
	{
		// The new option
		IOption* option = (IOption*) new Private::GetOptImpl::Option<U, true, false>(var, shortname, longname);
		// append the new option
		appendOption(option, shortname);
	}


	template<class U>
	void Parser::addFlag(U& var, char shortname, const AnyString& longname, const AnyString& description)
	{
		// The new option
		IOption* option = (IOption*) new Private::GetOptImpl::Option<U, true, false>(var, shortname, longname, description);
		// append the new option
		appendOption(option, shortname);
	}


	template<class U>
	void Parser::addFlag(U& var, char shortname, const AnyString& longname, const AnyString& description, bool visible)
	{
		// The new option
		IOption* option = (visible)
			? (IOption*) new Private::GetOptImpl::Option<U, true, false>(var, shortname, longname, description)
			: (IOption*) new Private::GetOptImpl::Option<U, false, false>(var, shortname, longname, description);
		// append the new option
		appendOption(option, shortname);
	}


	template<class U>
	void Parser::add(U& var, char shortname, const AnyString& longname, const AnyString& description, bool visible)
	{
		// The new option
		IOption* option = (visible)
			? (IOption*) new Private::GetOptImpl::Option<U, true>(var, shortname, longname, description)
			: (IOption*) new Private::GetOptImpl::Option<U, false>(var, shortname, longname, description);
		// append the new option
		appendOption(option, shortname);
	}



	template<class U>
	inline void Parser::remainingArguments(U& var)
	{
		delete pRemains;
		pRemains = new Private::GetOptImpl::Option<U, false>(var, '\0');
	}


	inline void Parser::addParagraph(const AnyString& text)
	{
		pAllOptions.push_back(new Private::GetOptImpl::Paragraph(text));
	}


	inline uint Parser::errors() const
	{
		return pErrors;
	}


	inline bool Parser::ignoreUnknownArgs() const
	{
		return pIgnoreUnknownArgs;
	}


	inline void Parser::ignoreUnknownArgs(bool ignore)
	{
		pIgnoreUnknownArgs = ignore;
	}





} // namespace GetOpt
} // namespace Yuni

