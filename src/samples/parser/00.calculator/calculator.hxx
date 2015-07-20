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
#ifndef __HEADER_DEMO_CALCULATOR_GRAMMAR_HXX__
# define __HEADER_DEMO_CALCULATOR_GRAMMAR_HXX__



inline std::ostream& operator << (std::ostream& out, const ::Demo::Calculator::Node& node)
{
	::Yuni::Clob content;
	::Demo::Calculator::Node::Export(content, node);out << content;
	return out;
}



inline std::ostream& operator << (std::ostream& out, const ::Demo::Calculator::Node::Ptr node)
{
	if (!(!node))
		out << *node;
	else
		out << "<invalid ast node>";
	return out;
}



namespace Demo
{
namespace Calculator
{


	inline Node::Node()
		: rule(rgUnknown)
		, offset()
		, offsetEnd()
	{}


	inline Node::Node(const Node& rhs)
		: inherited()
		, rule(rhs.rule)
		, offset(rhs.offset)
		, offsetEnd(rhs.offsetEnd)
		, text(rhs.text)
	{
		if (not rhs.children.empty())
		{
			children.resize(rhs.children.size());
			for (unsigned int i = 0; i != (unsigned) rhs.children.size(); ++i)
				children[i] = new Node(*rhs.children[i]);
		}
	}


	inline Node::~Node()
	{
	}


	template<class F> inline bool Node::each(const F& callback)
	{
		for (unsigned int i = 0; i != (unsigned) children.size(); ++i)
		{
			if (not callback(*children[i]))
				return false;
		}
		return true;
	}


	template<class F> inline bool Node::each(const F& callback) const
	{
		for (unsigned int i = 0; i != (unsigned) children.size(); ++i)
		{
			if (not callback(*children[i]))
				return false;
		}
		return true;
	}


	template<class F> inline bool Node::each(enum Rule rule, const F& callback)
	{
		for (unsigned int i = 0; i != (unsigned) children.size(); ++i)
		{
			Node& subnode = *children[i];
			if (subnode.rule == rule and not callback(subnode))
				return false;
		}
		return true;
	}


	template<class F> inline bool Node::each(enum Rule rule, const F& callback) const
	{
		for (unsigned int i = 0; i != (unsigned) children.size(); ++i)
		{
			const Node& subnode = *children[i];
			if (subnode.rule == rule and not callback(subnode))
				return false;
		}
		return true;
	}


	template<class StringT> inline bool Node::extractFirstChildText(StringT& out, enum Rule rule) const
	{
		for (unsigned int i = 0; i != (unsigned) children.size(); ++i)
		{
			const Node& subnode = *children[i];
			if (subnode.rule == rule)
			{
				out += subnode.text;
				return true;
			}
		}
		return false;
	}


	template<class StringT> inline bool Node::extractChildText(StringT& out, enum Rule rule, const AnyString& separator) const
	{
		bool somethingFound = false;

		if (separator.empty())
		{
			for (unsigned int i = 0; i != (unsigned) children.size(); ++i)
			{
				const Node& subnode = *children[i];
				if (subnode.rule == rule)
				{
					out += subnode.text;
					somethingFound = true;
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i != (unsigned) children.size(); ++i)
			{
				const Node& subnode = *children[i];
				if (subnode.rule == rule)
				{
					if (not out.empty())
						out += separator;
					out += subnode.text;
					somethingFound = true;
				}
			}
		}
		return somethingFound;
	}


	inline uint Node::findFirst(enum Rule rule) const
	{
		for (unsigned int i = 0; i != (unsigned) children.size(); ++i)
		{
			if (children[i]->rule == rule)
				return i;
		}
		return (uint)-1;
	}


	#ifdef __HEADER_DEMO_CALCULATOR_GRAMMAR_HAS_CXX_INITIALIZER_LIST
	inline Node::Ptr  Node::xpath(std::initializer_list<enum Rule> path) const
	{
		if (path.size() > 0)
		{
			auto it = path.begin();
			for (unsigned int i = 0; i != (unsigned) children.size(); ++i)
			{
				if (children[i]->rule == *it)
				{
					Node::Ptr result = children[i];
					++it;
					for (; it != path.end(); ++it)
					{
						bool found = false;
						auto& subnode = *result;
						for (unsigned int j = 0; j != (unsigned) subnode.children.size(); ++j)
						{
							if (subnode.children[j]->rule == *it)
							{
								result = subnode.children[j];
								found = true;
								break;
							}
						}
						if (not found)
							return nullptr;
					}
					return result;
				}
			}
		}
		return nullptr;
	}
	#endif







} // namespace Calculator
} // namespace Demo

#endif // __HEADER_DEMO_CALCULATOR_GRAMMAR_HXX__
