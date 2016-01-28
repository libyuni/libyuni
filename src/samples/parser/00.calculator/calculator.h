/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** github: https://github.com/libyuni/libyuni/
** gitlab: https://gitlab.com/libyuni/libyuni/ (mirror)
*/
#ifndef __HEADER_DEMO_CALCULATOR_GRAMMAR_H__
# define __HEADER_DEMO_CALCULATOR_GRAMMAR_H__

# include <yuni/yuni.h>
# include <yuni/core/string.h>
# include <yuni/core/bind.h>
# include <yuni/core/smartptr/intrusive.h>
# if __cplusplus > 199711L
#	define __HEADER_DEMO_CALCULATOR_GRAMMAR_HAS_CXX_INITIALIZER_LIST
#	include <initializer_list>
# endif

namespace Demo
{
namespace Calculator
{


	enum Rule
	{
		//! Unknown rule
		rgUnknown = 0,
		//! enum for the rule `expr`
		rgExpr = 1,
		//! enum for the rule `expr-atom`
		rgExprAtom = 2,
		//! enum for the rule `expr-group`
		rgExprGroup = 3,
		//! enum for the rule `expr-product`
		rgExprProduct = 4,
		//! enum for the rule `expr-sum`
		rgExprSum = 5,
		//! enum for the rule `number`
		rgNumber = 6,
		//! enum for the rule `start`
		rgStart = 7,
		//! enum for the rule `wp` [inline]
		rgWp = 8,
		//! enum for the final rule
		rgEOF = 9
	};
	enum
	{
		//! The total number of rules
		ruleCount = 10
	};



	//! Convert a rule id into its text representation
	AnyString RuleToString(enum Rule);



	enum Error
	{
		//! No error
		errNone,
		//! Parse error
		errParse,
	};





	class Notification final
	{
	public:
		//! Most suitable martptr
		typedef Yuni::SmartPtr<Notification> Ptr;
		//! Vector of nodes
		typedef std::vector<Ptr> Vector;

	public:
		Notification()
			: offset()
			, line()
		{}

	public:
		//! Start offset
		uint offset;
		//! Line Index
		uint line;
		//! Filename
		YString message;
		//! Filename
		YString filename;

	};




	class Node final : public Yuni::IIntrusiveSmartPtr<Node, false, Yuni::Policy::SingleThreaded>
	{
	public:
		//! Ancestor
		typedef Yuni::IIntrusiveSmartPtr<Node, false, Yuni::Policy::SingleThreaded> inherited;
		//! Threading policy
		typedef inherited::ThreadingPolicy ThreadingPolicy;
		//! The most suitable smart ptr for the class
		typedef inherited::SmartPtrType<Node>::Ptr  Ptr;
		//! Vector of nodes
		typedef std::vector<Node::Ptr> Vector;


	public:
		static void Export(Yuni::Clob& out, const Node& node);
		static void Export(Yuni::Clob& out, const Node& node, bool color);
		static void ExportToHTML(Yuni::Clob& out, const Node& node);

	public:
		//! Default constructor
		Node();
		//! Copy constructor
		Node(const Node& rhs);
		//! Destructor
		~Node();

		void clear();

		//! Iterate through all child nodes
		template<class F> bool each(const F& callback);

		//! Iterate through all child nodes (const)
		template<class F> bool each(const F& callback) const;

		template<class F> bool each(enum Rule rule, const F& callback);

		template<class F> bool each(enum Rule rule, const F& callback) const;

		template<class StringT> bool extractFirstChildText(StringT& out, enum Rule rule) const;

		template<class StringT> bool extractChildText(StringT& out, enum Rule rule, const AnyString& separator = nullptr) const;

		uint findFirst(enum Rule rule) const;

		#ifdef __HEADER_DEMO_CALCULATOR_GRAMMAR_HAS_CXX_INITIALIZER_LIST
		Node::Ptr  xpath(std::initializer_list<enum Rule> path) const;
		#endif

		Node& operator = (const Node& rhs);


	public:
		//! The rule ID
		enum Rule rule;
		//! Start offset
		uint offset;
		//! End offset
		uint offsetEnd;
		//! Text associated to the node (if any)
		AnyString text;

		//! Children
		Node::Vector children;
	};





	class Parser final
	{
	public:
		typedef Yuni::Bind<bool (Yuni::Clob& out, const AnyString& uri)>   OnURILoading;
		typedef Yuni::Bind<bool (const AnyString& filename, uint line, uint offset, Error, const YString::Vector&)>  OnError;

	public:
		Parser();
		~Parser();

		void clear();
		bool loadFromFile(const AnyString& filename);
		bool load(const AnyString& content);
		void translateOffset(uint& column, uint& line, const Node&) const;
		void translateOffset(uint& column, uint& line, uint offset) const;
		uint translateOffsetToLine(const Node& node) const;


	public:
		//! Event: load another include file
		OnURILoading onURILoading;
		//! Event: load another include file
		OnError  onError;

		//! The root node, if any
		Node::Ptr root;

		//! Notifications
		Notification::Vector notifications;


	private:
		void* pData;

	}; // class Parser





} // namespace Calculator
} // namespace Demo

# include "calculator.hxx"

#endif // __HEADER_DEMO_CALCULATOR_GRAMMAR_H__
