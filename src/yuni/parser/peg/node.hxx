#ifndef __YUNI_PARSER_GENERATOR_NODE_HXX__
# define __YUNI_PARSER_GENERATOR_NODE_HXX__



namespace Yuni
{
namespace Parser
{
namespace PEG
{

	inline Node::Node()
	{
		match.negate = false;
		match.min = 1;
		match.max = 1;
		rule.type = asRule;
	}


	inline uint Node::treeCount() const
	{
		uint count = (uint) children.size();
		for (uint i = 0; i != children.size(); ++i)
			count += children[i].treeCount();
		return count;
	}




} // namespace PEG
} // namespace Parser
} // namespace Yuni

#endif // __YUNI_PARSER_GENERATOR_NODE_HXX__
