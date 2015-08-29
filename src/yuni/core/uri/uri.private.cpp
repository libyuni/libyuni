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
#include "uri.private.h"
#include <limits.h>
#include <iostream>



namespace Yuni
{
namespace Private
{
namespace Uri
{


	Informations::Informations()
		: port(INT_MIN)
		, isValid(false)
	{}


	Informations::Informations(const Informations& rhs)
		: scheme(rhs.scheme)
		, user(rhs.user)
		, password(rhs.password)
		, server(rhs.server)
		, port(rhs.port)
		, path(rhs.path)
		, query(rhs.query)
		, fragment(rhs.fragment)
		, isValid(rhs.isValid)
	{}


	void Informations::clear()
	{
		scheme.clear();
		server.clear();
		user.clear();
		password.clear();
		port = INT_MIN;
		path.clear();
		query.clear();
		fragment.clear();
		isValid = false;
	}


	void Informations::assign(const Informations& rhs)
	{
		scheme   = rhs.scheme;
		server   = rhs.server;
		user     = rhs.user;
		port     = rhs.port;
		path     = rhs.path;
		query    = rhs.query;
		fragment = rhs.fragment;
		isValid  = rhs.isValid;
	}


	template<class U>
	static void WriteStructInformationsToStream(const Informations& infos, U& s)
	{
		if (infos.isValid)
		{
			if (not infos.scheme.empty())
				s << infos.scheme << ':';

			if (not infos.server.empty())
			{
				if (not infos.scheme.empty())
					s << "//";
				if (not infos.user.empty())
				{
					s << infos.user;
					if (not infos.password.empty())
						s << ':' << infos.password;
					s << "@";
				}
				s << infos.server;
				if (infos.port > 0)
					s << ':' << infos.port;
			}
			else
			{
				if (not infos.scheme.empty() and "file" == infos.scheme)
					s << "//";
			}

			s << infos.path;
			if (not infos.query.empty())
				s << '?' << infos.query;
			if (not infos.fragment.empty())
				s << '#' << infos.fragment;
		}
	}



	String Informations::toString() const
	{
		if (isValid)
		{
			String s;
			WriteStructInformationsToStream(*this, s);
			return s;
		}
		return String();
	}


	void Informations::print(std::ostream& out) const
	{
		if (isValid)
			WriteStructInformationsToStream(*this, out);
	}


	bool Informations::isEqualsTo(const Informations& rhs) const
	{
		return isValid and rhs.isValid and scheme == rhs.scheme
			 and path == rhs.path and server == rhs.server and port == rhs.port
			 and query == rhs.query and fragment == rhs.fragment
			 and user == rhs.user and password == rhs.password;
	}





} // namespace Uri
} // namespace Private
} // namespace Yuni

