#ifndef __YUNI_PARSER_GENERATOR_NODE_H__
# define __YUNI_PARSER_GENERATOR_NODE_H__

# include "../../yuni.h"
# include "../../core/string.h"
# include <map>
# include <iosfwd>


namespace Yuni
{
namespace Parser
{
namespace PEG
{

	class Node final
	{
	public:
		enum Type
		{
			asRule,
			asString,
			asSet,
			asAND,
			asOR,
		};

	public:
		//! List of nodes
		typedef std::vector<Node> Vector;
		//! Map of nodes
		typedef std::map<String, Node> Map;

	public:
		Node();

		void clear();
		bool checkRules(AnyString& error, const Node::Map& rules) const;

		void exportStd(std::ostream& out, uint depth = 1) const;
		void exportDOTSubgraph(Clob& out, const Map& rules, const String& rulename) const;
		void exportCPP(Clob& out, const Map& rules, Clob::Vector& helpers, uint depth, bool canreturn, uint& sp) const;
		void resetIndex(uint& base);
		void resetEnumID(const AnyString& rulename);
		void resetRuleIndexesFromMap(const Node::Map& rules);
		uint treeCount() const;

	public:
		struct
		{
			//! negate the return value
			bool negate;
			//! Minimum number of occurences
			uint min;
			//! Maximum number of occurences
			uint max;

			void reset(uint a, uint b) {min = a; max = b;}
		}
		match;

		struct
		{
			//! Type of node
			Type type;
			//! Text or set of chars to match
			String text;
		}
		rule;

		//! Sub nodes
		Vector children;

		//! Global index among all nodes and sub nodes
		String id;
		//! Enum ID for code generation
		String enumID;

	}; // class Node





} // namespace PEG
} // namespace Parser
} // namespace Yuni

# include "node.hxx"

#endif // __YUNI_PARSER_GENERATOR_NODE_H__
