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
#include "uri.h"



namespace Yuni
{

	inline Uri::Uri(const AnyString& string)
	{
		extractURIFromString(string);
	}


	inline void Uri::clear()
	{
		pInfos.clear();
	}


	inline const String& Uri::scheme() const
	{
		return pInfos.scheme;
	}

	inline const String& Uri::server() const
	{
		return pInfos.server;
	}

	inline int Uri::port() const
	{
		return pInfos.port;
	}

	inline bool Uri::hasPort() const
	{
		return (INT_MIN != pInfos.port);
	}

	inline const String& Uri::user() const
	{
		return pInfos.user;
	}


	inline const String& Uri::password() const
	{
		return pInfos.password;
	}


	inline const String& Uri::path() const
	{
		return pInfos.path;
	}

	inline const String& Uri::query() const
	{
		return pInfos.query;
	}


	inline const String& Uri::fragment() const
	{
		return pInfos.fragment;
	}



	inline Net::Protocol::Type Uri::protocol() const
	{
		return pInfos.isValid
			? Net::Protocol::SchemeToType(pInfos.scheme)
			: Net::Protocol::unknown;
	}


	inline bool Uri::schemeIsFile() const
	{
		return (pInfos.scheme.empty() and not pInfos.path.empty()) or "file" == pInfos.scheme;
	}

	inline bool Uri::schemeIsHTTP() const
	{
		return not pInfos.scheme.empty() and ("http" == pInfos.scheme or "https" == pInfos.scheme);
	}

	inline bool Uri::schemeIsFTP() const
	{
		return not pInfos.scheme.empty() and "ftp" == pInfos.scheme;
	}

	inline bool Uri::schemeIsSSH() const
	{
		return not pInfos.scheme.empty() and "ssh" == pInfos.scheme;
	}

	inline bool Uri::schemeIsLDAP() const
	{
		return not pInfos.scheme.empty() and "ldap" == pInfos.scheme;
	}



	inline bool Uri::isValid() const
	{
		return pInfos.isValid;
	}





	inline Uri& Uri::operator = (const Uri& rhs)
	{
		pInfos.assign(rhs.pInfos);
		return *this;
	}


	template<class StringT>
	inline Uri& Uri::operator = (const StringT& rhs)
	{
		extractURIFromString(rhs);
		return *this;
	}


	inline bool Uri::operator == (const Uri& rhs) const
	{
		return pInfos.isEqualsTo(rhs.pInfos);
	}


	inline bool Uri::operator != (const Uri& rhs) const
	{
		return !((*this) == rhs);
	}


	inline String Uri::operator() () const
	{
		return pInfos.toString();
	}


	inline String Uri::toString() const
	{
		return pInfos.toString();
	}



} // namespace Yuni




//! \name Operator overload for stream printing
//@{
inline std::ostream& operator << (std::ostream& out, const Yuni::Uri& u)
{
	return u.print(out);
}
//@}


