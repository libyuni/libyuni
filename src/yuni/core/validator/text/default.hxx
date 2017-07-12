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
#include "default.h"


namespace Yuni
{
namespace Validator
{
namespace Text
{


	template<Yuni::Validator::DefaultPolicy DefaultPolicy>
	inline ExceptionList<DefaultPolicy>::ExceptionList()
	{}


	template<Yuni::Validator::DefaultPolicy DefaultPolicy>
	inline ExceptionList<DefaultPolicy>::ExceptionList(const ExceptionList<DefaultPolicy>& rhs)
		:pExceptionList(rhs.pExceptionList)
	{}


	template<Yuni::Validator::DefaultPolicy DefaultPolicy>
	template<Yuni::Validator::DefaultPolicy OtherDefaultPolicy>
	inline ExceptionList<DefaultPolicy>::ExceptionList(const ExceptionList<OtherDefaultPolicy>& rhs)
		:pExceptionList(rhs.pExceptionList)
	{}


	template<Yuni::Validator::DefaultPolicy DefaultPolicy>
	inline ExceptionList<DefaultPolicy>::ExceptionList(const std::vector<String>& rhs)
		:pExceptionList(rhs)
	{}


	template<Yuni::Validator::DefaultPolicy DefaultPolicy>
	template<typename U>
	bool ExceptionList<DefaultPolicy>::validate(const U& s) const
	{
		if (!pExceptionList.empty())
		{
			const std::vector<String>::const_iterator end = pExceptionList.end();
			for (std::vector<String>::const_iterator i = pExceptionList.begin(); i != end; ++i)
			{
				if (*i == s)
					return !DefaultPolicy;
			}
		}
		return DefaultPolicy;
	}


	template<Yuni::Validator::DefaultPolicy DefaultPolicy>
	template<class U>
	inline void ExceptionList<DefaultPolicy>::exception(const U& e)
	{
		pExceptionList.push_back(e);
	}


	template<Yuni::Validator::DefaultPolicy DefaultPolicy>
	template<Yuni::Validator::DefaultPolicy OtherDefaultPolicy>
	inline ExceptionList<DefaultPolicy>& ExceptionList<DefaultPolicy>::operator = (const ExceptionList<OtherDefaultPolicy>& rhs)
	{
		pExceptionList = rhs.pExceptionList;
		return *this;
	}


	template<Yuni::Validator::DefaultPolicy DefaultPolicy>
	inline ExceptionList<DefaultPolicy>& ExceptionList<DefaultPolicy>::operator = (const std::vector<String>& rhs)
	{
		pExceptionList = rhs;
		return *this;
	}


	template<Yuni::Validator::DefaultPolicy DefaultPolicy>
	inline ExceptionList<DefaultPolicy>& ExceptionList<DefaultPolicy>::operator += (const AnyString& rhs)
	{
		pExceptionList.push_back(rhs);
		return *this;
	}


	template<Yuni::Validator::DefaultPolicy DefaultPolicy>
	template<Yuni::Validator::DefaultPolicy OtherDefaultPolicy>
	inline ExceptionList<DefaultPolicy>& ExceptionList<DefaultPolicy>::operator += (const ExceptionList<OtherDefaultPolicy>& rhs)
	{
		pExceptionList += rhs.pExceptionList;
		return *this;
	}


	template<Yuni::Validator::DefaultPolicy DefaultPolicy>
	inline ExceptionList<DefaultPolicy>& ExceptionList<DefaultPolicy>::operator += (const std::vector<String>& rhs)
	{
		for (auto& i: rhs)
			pExceptionList.push_back(i);
		return *this;
	}


	template<Yuni::Validator::DefaultPolicy DefaultPolicy>
	inline ExceptionList<DefaultPolicy>& ExceptionList<DefaultPolicy>::operator << (const AnyString& rhs)
	{
		pExceptionList.push_back(rhs);
		return *this;
	}


	template<Yuni::Validator::DefaultPolicy DefaultPolicy>
	template<Yuni::Validator::DefaultPolicy OtherDefaultPolicy>
	inline ExceptionList<DefaultPolicy>& ExceptionList<DefaultPolicy>::operator << (const ExceptionList<OtherDefaultPolicy>& rhs)
	{
		pExceptionList += rhs.pExceptionList;
		return *this;
	}


	template<Yuni::Validator::DefaultPolicy DefaultPolicy>
	inline ExceptionList<DefaultPolicy>& ExceptionList<DefaultPolicy>::operator << (const std::vector<String>& rhs)
	{
		for (auto& i: rhs)
			pExceptionList.push_back(i);
		return *this;
	}




} // namespace Text
} // namespace Validator
} // namespace Yuni
