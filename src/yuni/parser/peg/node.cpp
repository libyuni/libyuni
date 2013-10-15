
#include "node.h"



namespace Yuni
{
namespace Parser
{
namespace PEG
{

	template<class StreamT>
	static inline void PrintText(StreamT& out, const String& text)
	{
		String::const_utf8iterator end = text.utf8end();
		String::const_utf8iterator i   = text.utf8begin();

		for (; i != end; ++i)
		{
			if (i->size() == 1)
			{
				switch ((char) *i)
				{
					case '\n': out << "\\\\n"; break;
					case '\t': out << "\\\\t"; break;
					case '\r': out << "\\\\r"; break;
					case '\f': out << "\\\\f"; break;
					case '\v': out << "\\\\v"; break;
					case '"' : out << "\\\\\""; break;
					default: out << *i;
				}
			}
			else
				out << *i;
		}
	}



	static inline void PrintSubNodesIDs(Clob& out, const Node& node)
	{
		if (node.rule.type == Node::asRule and node.rule.text == "EOF")
			return;

		out << "\t\t\"" << node.id << "\" [label = \"";
		if (node.match.negate)
			out << "! ";

		switch (node.rule.type)
		{
			case Node::asAND:
			{
				out << "AND";
				break;
			}
			case Node::asOR:
			{
				out << "OR";
				break;
			}
			case Node::asString:
			{
				out << "\\\"";
				PrintText(out, node.rule.text);
				out << "\\\"";
				break;
			}
			case Node::asSet:
			{
				out << "one of \\\"";
				PrintText(out, node.rule.text);
				out << "\\\"";
				break;
			}
			case Node::asRule:
			{
				PrintText(out, node.rule.text);
				break;
			}
			default:
				PrintText(out, node.rule.text);
		}

		out << "\"";

		if (node.rule.type == Node::asAND or node.rule.type == Node::asOR)
			out << " shape = diamond";

		out << "];\n";
		for (uint i = 0; i != node.children.size(); ++i)
			PrintSubNodesIDs(out, node.children[i]);
	}


	static inline void PrintSubNodesLinks(Clob& out, const Node::Map& rules, const Node& node, const String& source, bool inverseColor)
	{
		if (node.rule.type == Node::asRule)
			return;

		if (node.rule.type == Node::asRule)
		{
			Node::Map::const_iterator i = rules.find(node.rule.text);
			if (i != rules.end())
				out << "\t\t\"" << source << "\" -> \"" << i->second.id << "\"";
			else
				out << "\t\t\"" << source << "\" -> \"" << node.id << "\"";
		}
		else
			out << "\t\t\"" << source << "\" -> \"" << node.id << "\"";

		out << ";\n";

		// relation
		if (not (node.match.min == 1 and node.match.max == 1))
		{
			out << "\t\t\"" << node.id << "\" -> \"" << node.id << "\" [";
			if (node.rule.type == Node::asRule)
				out << "lhead = \"cluster-" << node.rule.text << "\"; ";
			out << "label=<<font color=\"#FF5500\">";

			if (node.match.min == 0 and node.match.max == (uint) -1)
				out << '*' << node.id;
			else if (node.match.min == 0 and node.match.max == 1)
				out << '?';
			else if (node.match.min == 1 and node.match.max == (uint) -1)
				out << '+';
			else
			{
				out << '{' << node.match.min << ',';
				if (node.match.max == (uint) -1)
					out << 'n';
				else
					out << node.match.max;
				out << '}';
			}

			out << "</font>>];\n";
		}

		// display all subnodes
		if (not node.children.empty())
		{
			out << "\t\tsubgraph \"cluster-" << node.id << "\" {\n";
			if (inverseColor)
			{
				out << "\t\tstyle = filled;\n";
				out << "\t\tcolor = lightgrey;\n";
				out << "\t\tnode [style = filled, color = white];\n";
			}
			else
			{
				out << "\t\tstyle = filled;\n";
				out << "\t\tcolor = \"#dfdfdf\";\n";
				out << "\t\tnode [style = filled, color = lightgrey];\n";
			}

			switch (node.rule.type)
			{
				case Node::asOR:
				{
					PrintSubNodesLinks(out, rules, node.children[0], node.id, not inverseColor);
					for (uint i = 1; i != node.children.size(); ++i)
						PrintSubNodesLinks(out, rules, node.children[i], node.id, not inverseColor);
					break;
				}
				default:
				{
					PrintSubNodesLinks(out, rules, node.children[0], node.id, not inverseColor);
					for (uint i = 1; i != node.children.size(); ++i)
						PrintSubNodesLinks(out, rules, node.children[i], node.children[i - 1].id, not inverseColor);
				}
			}

			out << "\t\t}\n";
		}
	}





	void Node::clear()
	{
		match.negate = false;
		match.min = 1;
		match.max = 1;
		rule.type = asRule;
		rule.text.clear();
		children.clear();
	}


	void Node::resetIndex(uint& base)
	{
		id.clear() << "n" << base;
		++base;
		for (uint i = 0; i != children.size(); ++i)
			children[i].resetIndex(base);
	}


	void Node::resetRuleIndexesFromMap(const Node::Map& rules)
	{
		if (rule.type == Node::asRule)
		{
			if (rule.text == "EOF")
			{
				id = "EOF";
				enumID.clear();
			}
			else
			{
				Node::Map::const_iterator i = rules.find(rule.text);
				if (i != rules.end())
				{
					id = (i->second).id;
					enumID = (i->second).enumID;
				}
			}
		}

		for (uint i = 0; i != children.size(); ++i)
			children[i].resetRuleIndexesFromMap(rules);
	}


	void Node::resetEnumID(const AnyString& rulename)
	{
		enumID.clear();
		enumID << "rg";

		bool maj = false;
		AnyString::const_utf8iterator end = rulename.utf8end();
		AnyString::const_utf8iterator i   = rulename.utf8begin();
		for (; i != end; ++i)
		{
			char c = (char) *i;
			if (c == '-' or c == '_' or c == ' ')
			{
				maj = true;
			}
			else
			{
				if (not maj)
				{
					enumID += *i;
				}
				else
				{
					maj = false;
					if (i->size() > 1)
						enumID += *i;
					else
						enumID += (char) String::ToUpper(c);
				}
			}
		}

		if (enumID.size() > 2)
			enumID[2] = (char) String::ToUpper(enumID[2]);
	}


	bool Node::checkRules(AnyString& error, const Node::Map& rules) const
	{
		if (rule.type == asRule)
		{
			if (rule.text != '|' and rule.text != '.' and rule.text != "EOF")
			{
				if (rules.count(rule.text) == 0)
				{
					error = rule.text;
					return false;
				}
			}
		}

		for (uint i = 0; i != children.size(); ++i)
		{
			if (not children[i].checkRules(error, rules))
				return false;
		}
		return true;
	}



	void Node::exportDOTSubgraph(Clob& out, const Node::Map& rules, const String& rulename) const
	{
		// display all subnodes
		out << "\t\t\"" << id << "\" [label = \"" << rulename << "\"];\n";
		for (uint i = 0; i != children.size(); ++i)
			PrintSubNodesIDs(out, children[i]);

		// display all subnodes
		if (not children.empty())
		{
			PrintSubNodesLinks(out, rules, children[0], id, false);
			for (uint i = 1; i != children.size(); ++i)
				PrintSubNodesLinks(out, rules, children[i], children[i - 1].id, false);
		}
	}


	template<class StreamT>
	static inline StreamT& PrintTabs(StreamT& out, uint depth)
	{
		for (uint i = 0; i != depth; ++i)
			out << '\t';
		return out;
	}


	template<class StreamT>
	static inline StreamT& PrintString(StreamT& out, const String& text)
	{
		String::const_utf8iterator end = text.utf8end();
		String::const_utf8iterator i   = text.utf8begin();

		for (; i != end; ++i)
		{
			if (i->size() == 1)
			{
				switch ((char) *i)
				{
					case '\n': out << "\\n"; break;
					case '\t': out << "\\t"; break;
					case '\r': out << "\\r"; break;
					case '\f': out << "\\f"; break;
					case '\v': out << "\\v"; break;
					case '"' : out << "\\\""; break;
					default: out << *i;
				}
			}
			else
				out << *i;
		}

		return out;
	}


	template<class StreamT>
	static inline void PrintAsciiChar(StreamT& out, char c)
	{
		switch (c)
		{
			case '\'':  out << "\\\'";break;
			case '\n':  out << "\\n";break;
			case '\t':  out << "\\t";break;
			case '\r':  out << "\\r";break;
			case '\f':  out << "\\f";break;
			case '\v':  out << "\\v";break;
			default: out << c;
		}
	}



	void Node::exportCPP(Clob& out, const Map& rules, Clob::Vector& helpers, uint depth, bool canreturn, uint& sp) const
	{
		uint d = depth;

		AnyString neg;
		if (not match.negate)
			neg = "not ";
		AnyString returnfalse = (canreturn) ? "return false;" : "break;";
		String stmt;


		switch (rule.type)
		{
			case Node::asRule:
			{
				Node::Map::const_iterator r = rules.find(rule.text);
				if (r != rules.end())
					stmt << neg << "yy" << r->second.enumID << "(ctx)";
				else
				{
					assert(false and "missing rule !");
					return;
				}
				break;
			}

			case Node::asString:
			{
				// matchSingleAscii is guaranted to not modify the stack if it does not match
				if (rule.text.size() == 1)
				{
					stmt << neg << "ctx.matchSingleAsciiChar('";
					PrintAsciiChar(stmt, rule.text[0]);
					stmt << "')";
				}
				else
				{
					stmt << neg << "ctx.matchString(AnyString(\"";
					PrintString(stmt, rule.text) << "\", " << rule.text.size() << "), " << rule.text.utf8size() << ")";
				}
				break;
			}

			case Node::asSet:
			{
				if (rule.text.size() == 1)
				{
					stmt << neg << "ctx.matchSingleAsciiChar('";
					PrintAsciiChar(stmt, rule.text[0]);
					stmt << "')";
				}
				else
				{
					// not utf8 chars
					if (rule.text.utf8size() == rule.text.size())
					{
						stmt << neg << "ctx.matchOneOf(AnyString(\"";
						PrintString(stmt, rule.text) << "\", " << rule.text.size() << "), " << rule.text.utf8size() << ")";
					}
					else
					{
						// Not handled yet
						PrintTabs(out, d) << "# error Sequence of chars with UTF8 chars not handled yet\n";
					}
				}
				break;
			}

			case Node::asAND:
			{
				if (not match.negate and match.min == 1 and match.max == 1)
				{
					for (uint i = 0; i != children.size(); ++i)
						children[i].exportCPP(out, rules, helpers, d, canreturn, sp);
					return;
				}
				else
				{
					uint from = ++sp;
					sp += children.size();
					if (match.negate)
						stmt << neg << '(';
					uint helpersFrom = (uint) helpers.size();
					helpers.resize(helpersFrom + children.size());

					for (uint i = 0; i != children.size(); ++i)
					{
						helpers.push_back(nullptr);
						Clob& datacpp = helpers[i + helpersFrom];
						datacpp << "\ttemplate<class ContextT>\n";
						datacpp << "\tstatic inline bool __helper" << (from + i) << "(ContextT& ctx)\n";
						datacpp << "\t{\n";
						children[i].exportCPP(datacpp, rules, helpers, 2, true, sp);
						datacpp << "\t\treturn true;\n";
						datacpp << "\t}\n";

						if (i != 0)
							stmt << " and ";
						stmt << "__helper" << (from + i) << "(ctx)";
					}
					if (match.negate)
						stmt << ')';
				}
				break;
			}

			case Node::asOR:
			{
				uint lsp = ++sp;
				uint osp = ++sp;
				PrintTabs(out, d) << "uint sp" << osp << " = ctx.push();\n";
				PrintTabs(out, d) << "bool rt" << lsp << " = false;\n";
				PrintTabs(out, d) << "do\n";
				PrintTabs(out, d) << "{\n";
				children[0].exportCPP(out, rules, helpers, d + 1, false, sp);
				PrintTabs(out, d) << "\trt" << lsp << " = true;\n";
				PrintTabs(out, d) << "}\n";
				PrintTabs(out, d) << "while (false);\n";

				PrintTabs(out, d) << "while (not rt" << lsp << ")\n";
				PrintTabs(out, d) << "{\n";
				PrintTabs(out, d) << "\tctx.restart(sp" << osp << ");\n";
				children[1].exportCPP(out, rules, helpers, d + 1, false, sp);
				PrintTabs(out, d) << "\trt" << lsp << " = true;\n";
				PrintTabs(out, d) << "\tbreak;\n";
				PrintTabs(out, d) << "}\n";
				out << '\n';
				PrintTabs(out, d) << "if (not rt" << lsp << ")\n";
				PrintTabs(out, d) << "{\n";
				PrintTabs(out, d) << "\tctx.offset = sp" << osp << "; // restore context before if\n";
				PrintTabs(out, d) << "\t" << returnfalse << "\n";
				PrintTabs(out, d) << "}\n";
				return; // nothing to do here
				break;
			}
		}


		if (match.min == 0 and match.max == 1)
		{
			if (rule.type == Node::asRule)
			{
				uint osp = ++sp;
				PrintTabs(out, d) << "uint sp" << osp << " = ctx.push(); // 0 or 1\n";
				PrintTabs(out, d) << "if (" << stmt << ")\n";
				PrintTabs(out, d) << "\tctx.offset = sp" << osp << ";\n";
			}
			else
			{
				// others are guaranted to not modify the stack
				PrintTabs(out, d) << "if (" << stmt << ")\n";
				PrintTabs(out, d) << "\t{} // exec stmt - don't care of the result\n";
			}
			return;
		}

		if (match.min == 1)
		{
			PrintTabs(out, d) << "if (" << stmt << ")\n";
			PrintTabs(out, d) << "\t" << returnfalse << '\n';
		}
		if (match.max == 1)
			return;

		if (match.min <= 1 and match.max == (uint) -1)
		{
			if (rule.type == Node::asRule)
			{
				uint osp = ++sp;
				PrintTabs(out, d) << "while (true)\n";
				PrintTabs(out, d) << "{\n";
				PrintTabs(out, d) << "\tuint sp" << osp << " = ctx.push(); // 0 or 1\n";
				PrintTabs(out, d) << "\tif (" << stmt << ")\n";
				PrintTabs(out, d) << "\t{\n";
				PrintTabs(out, d) << "\t\tctx.offset = sp" << osp << ";\n";
				PrintTabs(out, d) << "\t\tbreak;\n";
				PrintTabs(out, d) << "\t}\n";
				PrintTabs(out, d) << "}\n";
			}
			else
			{
				// others are guaranted to not modify the stack
				PrintTabs(out, d) << "while (" << stmt << ")\n";
				PrintTabs(out, d) << "{}\n";
			}
			return;
		}
		assert(false and "a rule is missing !");
	}




	void Node::exportStd(std::ostream& out, uint d) const
	{
		PrintTabs(out, d);

		switch (rule.type)
		{
			case Node::asAND:
			{
				out << "AND";
				break;
			}
			case Node::asOR:
			{
				out << "OR";
				break;
			}
			case Node::asString:
			{
				out << "\\\"";
				PrintText(out, rule.text);
				out << "\\\"";
				break;
			}
			case Node::asSet:
			{
				out << "one of \\\"";
				PrintText(out, rule.text);
				out << "\\\"";
				break;
			}
			case Node::asRule:
			{
				PrintText(out, rule.text);
				break;
			}
		}

		out << '\n';

		for (uint i = 0; i != children.size(); ++i)
			children[i].exportStd(out, d + 1);
	}





} // namespace PEG
} // namespace Parser
} // namespace Yuni

