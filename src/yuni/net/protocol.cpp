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
#include "protocol.h"



namespace Yuni
{
namespace Net
{
namespace Protocol
{


	static inline Type schemeToType_F(const String& s)
	{
		if ("file" == s)
			return file;
		if ("ftp" == s)
			return ftp;
		return unknown;
	}


	static inline Type schemeToType_H(const String& s)
	{
		if ("http" == s)
			return http;
		if ("https" == s)
			return https;
		return unknown;
	}


	static inline Type schemeToType_I(const String& s)
	{
		if ("imap" == s)
			return imap;
		if ("imaps" == s)
			return imaps;
		if ("irc" == s)
			return irc;
		if ("ircs" == s)
			return ircs;
		return unknown;
	}


	static inline Type schemeToType_K(const String& s)
	{
		if ("kerberos" == s)
			return kerberos;
		return unknown;
	}


	static inline Type schemeToType_L(const String& s)
	{
		if ("ldap" == s)
			return ldap;
		if ("ldaps" == s)
			return ldaps;
		return unknown;
	}


	static inline Type schemeToType_N(const String& s)
	{
		if ("nfs" == s)
			return nfs;
		if ("ntp" == s)
			return ntp;
		if ("news" == s)
			return news;
		return unknown;
	}


	static inline Type schemeToType_P(const String& s)
	{
		if ("pop3" == s)
			return pop3;
		if ("pop3s" == s)
			return pop3s;
		return unknown;
	}


	static inline Type schemeToType_R(const String& s)
	{
		if ("rtsp" == s)
			return rtsp;
		return unknown;
	}



	static inline Type schemeToType_S(const String& s)
	{
		if ("ssh" == s)
			return ssh;
		if ("svn" == s)
			return snmp;
		if ("smtp" == s)
			return smtp;
		if ("sftp" == s)
			return sftp;
		if ("snmp" == s)
			return snmp;
		return unknown;
	}



	Type  SchemeToType(const String& s)
	{
		if (!s.empty())
		{
			// A little tip to reduce the count of string comparisons
			switch (s[0])
			{
				case 'f': return schemeToType_F(s);
				case 'h': return schemeToType_H(s);
				case 'i': return schemeToType_I(s);
				case 'k': return schemeToType_K(s);
				case 'l': return schemeToType_L(s);
				case 'n': return schemeToType_N(s);
				case 'p': return schemeToType_P(s);
				case 'r': return schemeToType_R(s);
				case 's': return schemeToType_S(s);
			}
		}
		return unknown;
	}



	String ToScheme(const Type& type)
	{
		switch (type)
		{
			case unknown : return String();
			case file : return "file";

			case news : return "news";

			case domain : return "ns";

			case ftp : return "ftp";

			case http : return "http";
			case https : return "https";

			case imap : return "imap";
			case imaps : return "imaps";
			case irc : return "irc";
			case ircs : return "ircs";

			case kerberos : return "kerberos";

			case ldap : return "ldap";
			case ldaps : return "ldaps";

			case nfs : return "nfs";
			case ntp : return "ntp";

			case pop3 : return "pop3";
			case pop3s : return "pop3s";

			case rtsp : return "rtsp";

			case sftp : return "sftp";
			case smtp : return "smtp";
			case smtps : return "smtp";
			case snmp : return "snmp";
			case ssh : return "ssh";
			case svn : return "svn";
		}
		return String();
	}





} // namespace Protocol
} // namespace Net
} // namespace Yuni
