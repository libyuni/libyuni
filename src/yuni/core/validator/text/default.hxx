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
#ifndef __YUNI_CORE_VALIDATOR_DEFAULT_HXX__
# define __YUNI_CORE_VALIDATOR_DEFAULT_HXX__


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
	inline ExceptionList<DefaultPolicy>::ExceptionList(const String::Vector& rhs)
		:pExceptionList(rhs)
	{}


	template<Yuni::Validator::DefaultPolicy DefaultPolicy>
	inline ExceptionList<DefaultPolicy>::ExceptionList(const String::List& rhs)
	{
		if (!rhs.empty())
		{
			const String::List::const_iterator end = rhs.end();
			for (String::List::const_iterator i = rhs.begin(); i != end; ++i)
				pExceptionList.push_back(*i);
		}
	}




	template<Yuni::Validator::DefaultPolicy DefaultPolicy>
	template<typename U>
	bool ExceptionList<DefaultPolicy>::validate(const U& s) const
	{
		if (!pExceptionList.empty())
		{
			const String::Vector::const_iterator end = pExceptionList.end();
			for (String::Vector::const_iterator i = pExceptionList.begin(); i != end; ++i)
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
	inline ExceptionList<DefaultPolicy>& ExceptionList<DefaultPolicy>::operator = (const String::Vector& rhs)
	{
		pExceptionList = rhs;
		return *this;
	}


	template<Yuni::Validator::DefaultPolicy DefaultPolicy>
	ExceptionList<DefaultPolicy>& ExceptionList<DefaultPolicy>::operator = (const String::List& rhs)
	{
		pExceptionList.clear();
		if (!rhs.empty())
		{
			const String::List::const_iterator end = rhs.end();
			for (String::List::const_iterator i = rhs.begin(); i != end; ++i)
				pExceptionList.push_back(*i);
		}
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
	inline ExceptionList<DefaultPolicy>& ExceptionList<DefaultPolicy>::operator += (const String::Vector& rhs)
	{
		pExceptionList += rhs;
		return *this;
	}


	template<Yuni::Validator::DefaultPolicy DefaultPolicy>
	inline ExceptionList<DefaultPolicy>& ExceptionList<DefaultPolicy>::operator += (const String::List& rhs)
	{
		if (!rhs.empty())
		{
			const String::List::const_iterator end = rhs.end();
			for (String::List::const_iterator i = rhs.begin(); i != end; ++i)
				pExceptionList.push_back(*i);
		}
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
	inline ExceptionList<DefaultPolicy>& ExceptionList<DefaultPolicy>::operator << (const String::Vector& rhs)
	{
		pExceptionList += rhs;
		return *this;
	}


	template<Yuni::Validator::DefaultPolicy DefaultPolicy>
	ExceptionList<DefaultPolicy>& ExceptionList<DefaultPolicy>::operator << (const String::List& rhs)
	{
		if (!rhs.empty())
		{
			const String::List::const_iterator end = rhs.end();
			for (String::List::const_iterator i = rhs.begin(); i != end; ++i)
				pExceptionList.push_back(*i);
		}
		return *this;
	}




} // namespace Text
} // namespace Validator
} // namespace Yuni

#endif // __YUNI_CORE_VALIDATOR_DEFAULT_H__
