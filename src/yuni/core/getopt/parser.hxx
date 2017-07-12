/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** gitlab: https://gitlab.com/libyuni/libyuni/
** github: https://github.com/libyuni/libyuni/ {mirror}
*/
#pragma once



namespace Yuni
{
namespace GetOpt
{

	template<class U>
	inline void Parser::add(U& var, char shortname)
	{
		auto* option = new (std::nothrow) Private::GetOptImpl::Option<U, true>(var, shortname);
		if (option)
			appendShortOption(option, shortname);
	}


	template<class U>
	inline void Parser::addFlag(U& var, char shortname)
	{
		auto* option = new (std::nothrow) Private::GetOptImpl::Option<U, true, false>(var, shortname);
		if (option)
			appendShortOption(option, shortname);
	}


	template<class U>
	inline void Parser::add(U& var, char shortname, const AnyString& longname)
	{
		auto* option = new (std::nothrow) Private::GetOptImpl::Option<U, true>(var, shortname, longname);
		if (option)
			appendOption(option, shortname);
	}


	template<class U>
	inline void Parser::add(U& var, char shortname, const AnyString& longname, const AnyString& description)
	{
		auto* option = new (std::nothrow) Private::GetOptImpl::Option<U, true>(var, shortname, longname, description);
		if (option)
			appendOption(option, shortname);
	}



	template<class U>
	inline void Parser::addFlag(U& var, char shortname, const AnyString& longname)
	{
		auto* option = new (std::nothrow) Private::GetOptImpl::Option<U, true, false>(var, shortname, longname);
		if (option)
			appendOption(option, shortname);
	}


	template<class U>
	void Parser::addFlag(U& var, char shortname, const AnyString& longname, const AnyString& description)
	{
		auto* option = new (std::nothrow) Private::GetOptImpl::Option<U, true, false>(var, shortname, longname, description);
		if (option)
			appendOption(option, shortname);
	}


	template<class U>
	void Parser::addFlag(U& var, char shortname, const AnyString& longname, const AnyString& description, bool visible)
	{
		IOption* option;
		if (visible)
			option = new (std::nothrow) Private::GetOptImpl::Option<U, true, false>(var, shortname, longname, description);
		else
			option = new (std::nothrow) Private::GetOptImpl::Option<U, false, false>(var, shortname, longname, description);
		if (option)
			appendOption(option, shortname);
	}


	template<class U>
	void Parser::add(U& var, char shortname, const AnyString& longname, const AnyString& description, bool visible)
	{
		IOption* option;
		if (visible)
			option = new (std::nothrow) Private::GetOptImpl::Option<U, true>(var, shortname, longname, description);
		else
			option = new (std::nothrow) Private::GetOptImpl::Option<U, false>(var, shortname, longname, description);
		if (option)
			appendOption(option, shortname);
	}



	template<class U>
	inline void Parser::remainingArguments(U& var)
	{
		delete pRemains;
		pRemains = new (std::nothrow) Private::GetOptImpl::Option<U, false>(var, '\0');
	}


	inline void Parser::addParagraph(const AnyString& text)
	{
		auto* option = new (std::nothrow) Private::GetOptImpl::Paragraph(text);
		if (option)
			pAllOptions.push_back(option);
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

