#ifndef __YUNI_CORE_URI_URI_HXX__
# define __YUNI_CORE_URI_URI_HXX__

namespace Yuni
{

	inline Uri::Uri(const AnyString& string)
	{
		extractURIFromString(string);
	}


	template<class StringT>
	inline void Uri::path(const StringT& s)
	{
		pInfos.path = s;
		pInfos.isValid = true;
	}


	template<class StringT>
	inline void Uri::user(const StringT& s)
	{
		pInfos.user = s;
		pInfos.isValid = true;
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


	template<class StringT>
	void Uri::password(const StringT& s)
	{
		pInfos.password = s;
		pInfos.isValid = true;
	}


	inline const String& Uri::path() const
	{
		return pInfos.path;
	}

	inline const String& Uri::query() const
	{
		return pInfos.query;
	}


	template<class StringT>
	void Uri::query(const StringT& s)
	{
		pInfos.query = s;
		pInfos.isValid = true;
	}


	inline const String& Uri::fragment() const
	{
		return pInfos.fragment;
	}


	template<class StringT>
	void Uri::fragment(const StringT& s)
	{
		pInfos.fragment = s;
		pInfos.isValid = true;
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


	inline void Uri::scheme(const Net::Protocol::Type& type)
	{
		pInfos.scheme = Net::Protocol::ToScheme(type);
		pInfos.isValid = true;
	}


	template<class StringT>
	void Uri::server(const StringT& s)
	{
		pInfos.server = s;
		pInfos.isValid = true;
	}


	template<class StringT>
	void Uri::scheme(const StringT& s)
	{
		pInfos.scheme = s;
		pInfos.isValid = true;
	}




} // namespace Yuni




//! \name Operator overload for stream printing
//@{
inline std::ostream& operator << (std::ostream& out, const Yuni::Uri& u)
{
	return u.print(out);
}
//@}


#endif // __YUNI_CORE_URI_URI_H__
