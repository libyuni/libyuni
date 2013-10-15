#ifndef __HEADER_DEMO_CALCULATOR_GRAMMAR_NODES_H__
# define __HEADER_DEMO_CALCULATOR_GRAMMAR_NODES_H__

# include <yuni/yuni.h>
# include <yuni/core/string.h>


namespace Demo
{
namespace Calculator
{


	enum Rule
	{
		//! enum for the rule `expr`
		rgExpr,
		//! enum for the rule `expr-atom`
		rgExprAtom,
		//! enum for the rule `expr-product`
		rgExprProduct,
		//! enum for the rule `expr-sum`
		rgExprSum,
		//! enum for the rule `number`
		rgNumber,
		//! enum for the rule `start`
		rgStart,
		//! enum for the rule `wp`
		rgWp,
		//! enum for the final rule
		rgEOF
	};

	enum
	{
		//! The total number of rules
		ruleCount = 8
	};



	enum Error
	{
		errNone,
	};



	class NodeRgExpr final
	{
	public:
		//! rule enum id
		static inline Rule rule()  { return rgExpr; }
	};


	class NodeRgExprAtom final
	{
	public:
		//! rule enum id
		static inline Rule rule()  { return rgExprAtom; }
	};


	class NodeRgExprProduct final
	{
	public:
		//! rule enum id
		static inline Rule rule()  { return rgExprProduct; }
	};


	class NodeRgExprSum final
	{
	public:
		//! rule enum id
		static inline Rule rule()  { return rgExprSum; }
	};


	class NodeRgNumber final
	{
	public:
		//! rule enum id
		static inline Rule rule()  { return rgNumber; }
	};


	class NodeRgStart final
	{
	public:
		//! rule enum id
		static inline Rule rule()  { return rgStart; }
	};


	class NodeRgWp final
	{
	public:
		//! rule enum id
		static inline Rule rule()  { return rgWp; }
	};




} // namespace Calculator
} // namespace Demo

#endif // __HEADER_DEMO_CALCULATOR_GRAMMAR_NODES_H__
