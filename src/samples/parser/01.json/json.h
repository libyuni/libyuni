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
#ifndef __HEADER_DEMO_JSON_GRAMMAR_H__
# define __HEADER_DEMO_JSON_GRAMMAR_H__

# include <yuni/yuni.h>
# include <yuni/core/string.h>
# include <yuni/core/bind.h>
# include <yuni/core/dictionary.h>
# include <yuni/core/smartptr/intrusive.h>
# if __cplusplus > 199711L
#	define __HEADER_DEMO_JSON_GRAMMAR_HAS_CXX_INITIALIZER_LIST
#	include <initializer_list>
# endif

//! Metadata support
# define __HEADER_DEMO_JSON_GRAMMAR_HAS_METADATA  1





namespace Demo
{
namespace JSON
{

	enum Rule
	{
		//! Unknown rule
		rgUnknown = 0,
		//! enum for the rule `array`
		rgArray = 1,
		//! enum for the rule `char-extended` [inline]
		rgCharExtended = 2,
		//! enum for the rule `digit` [inline]
		rgDigit = 3,
		//! enum for the rule `error`
		rgError = 4,
		//! enum for the rule `error-bracket`
		rgErrorBracket = 5,
		//! enum for the rule `error-eol`
		rgErrorEol = 6,
		//! enum for the rule `error-parenthese`
		rgErrorParenthese = 7,
		//! enum for the rule `error-semicolon`
		rgErrorSemicolon = 8,
		//! enum for the rule `false`
		rgFalse = 9,
		//! enum for the rule `hex` [inline]
		rgHex = 10,
		//! enum for the rule `integer`
		rgInteger = 11,
		//! enum for the rule `key`
		rgKey = 12,
		//! enum for the rule `null`
		rgNull = 13,
		//! enum for the rule `number`
		rgNumber = 14,
		//! enum for the rule `number-def` [inline]
		rgNumberDef = 15,
		//! enum for the rule `number-exponent`
		rgNumberExponent = 16,
		//! enum for the rule `number-hexa`
		rgNumberHexa = 17,
		//! enum for the rule `number-qualifier`
		rgNumberQualifier = 18,
		//! enum for the rule `number-qualifier-type`
		rgNumberQualifierType = 19,
		//! enum for the rule `number-sign`
		rgNumberSign = 20,
		//! enum for the rule `number-value`
		rgNumberValue = 21,
		//! enum for the rule `object`
		rgObject = 22,
		//! enum for the rule `object-entry`
		rgObjectEntry = 23,
		//! enum for the rule `punc` [inline]
		rgPunc = 24,
		//! enum for the rule `single-char` [inline]
		rgSingleChar = 25,
		//! enum for the rule `sp` [inline]
		rgSp = 26,
		//! enum for the rule `start`
		rgStart = 27,
		//! enum for the rule `string`
		rgString = 28,
		//! enum for the rule `string-literal`
		rgStringLiteral = 29,
		//! enum for the rule `tk-brace-close`
		rgTkBraceClose = 30,
		//! enum for the rule `tk-brace-open`
		rgTkBraceOpen = 31,
		//! enum for the rule `tk-bracket-close`
		rgTkBracketClose = 32,
		//! enum for the rule `tk-bracket-open`
		rgTkBracketOpen = 33,
		//! enum for the rule `tk-colon`
		rgTkColon = 34,
		//! enum for the rule `tk-comma`
		rgTkComma = 35,
		//! enum for the rule `tk-comment-block`
		rgTkCommentBlock = 36,
		//! enum for the rule `tk-comment-block-content`
		rgTkCommentBlockContent = 37,
		//! enum for the rule `tk-comment-end-block`
		rgTkCommentEndBlock = 38,
		//! enum for the rule `tk-comment-line`
		rgTkCommentLine = 39,
		//! enum for the rule `tk-comment-line-content`
		rgTkCommentLineContent = 40,
		//! enum for the rule `tk-comment-start-block`
		rgTkCommentStartBlock = 41,
		//! enum for the rule `tk-comment-start-inline`
		rgTkCommentStartInline = 42,
		//! enum for the rule `tk-comment-subbk` [inline]
		rgTkCommentSubbk = 43,
		//! enum for the rule `tk-dot`
		rgTkDot = 44,
		//! enum for the rule `tk-double-quote`
		rgTkDoubleQuote = 45,
		//! enum for the rule `true`
		rgTrue = 46,
		//! enum for the rule `value`
		rgValue = 47,
		//! enum for the rule `wp` [inline]
		rgWp = 48,
		//! enum for the final rule
		rgEOF = 49
	};
	enum
	{
		//! The total number of rules
		ruleCount = 50
	};



	//! Convert a rule id into its text representation
	AnyString RuleToString(enum Rule);


	//! Get if a rule is an error
	bool  RuleIsError(enum Rule ruleid);


	//! Get if the rule should be ignored when duplucating an AST (starting from 'tk-' and some special rules)
	bool ShouldIgnoreRuleForDuplication(enum Rule);



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
		//! Set of symbols
		typedef Yuni::Set<Ptr>::Unordered Set;

		//! Callback definition for releasing metadata
		typedef void (* MetadataReleaseCallback)(void*);
		//! Callback definition for cloning metadata
		typedef void* (* MetadataCloneCallback)(void*);


	public:
		//! Export the tree node
		static void Export(Yuni::Clob& out, const Node& node);
		//! Export the tree node (with color output)
		static void Export(Yuni::Clob& out, const Node& node, bool color);
		//! Export the tree node to HTML
		static void ExportToHTML(Yuni::Clob& out, const Node& node);

		#if __HEADER_DEMO_JSON_GRAMMAR_HAS_METADATA != 0
		//! Callback for releasing metadata
		static MetadataReleaseCallback  metadataRelease;
		//! Callback for cloning metadata
		static MetadataCloneCallback  metadataClone;
		#endif


	public:
		//! Default constructor
		Node();
		//! Default constructor with a pre-defined rule
		Node(enum Rule);
		//! Copy constructor
		Node(const Node& rhs);
		//! Destructor
		~Node();

		void clear();

		void swap(Node&);

		//! Iterate through all child nodes
		template<class F> bool each(const F& callback);

		//! Iterate through all child nodes (const)
		template<class F> bool each(const F& callback) const;

		template<class F> bool each(enum Rule rule, const F& callback);

		template<class F> bool each(enum Rule rule, const F& callback) const;

		template<class StringT> bool extractFirstChildText(StringT& out, enum Rule rule) const;

		template<class StringT> bool extractChildText(StringT& out, enum Rule rule, const AnyString& separator = nullptr) const;

		uint findFirst(enum Rule rule) const;

		bool exists(enum Rule rule) const;

		#ifdef __HEADER_DEMO_JSON_GRAMMAR_HAS_CXX_INITIALIZER_LIST
		Node::Ptr  xpath(std::initializer_list<enum Rule> path) const;
		#endif

		#ifdef __HEADER_DEMO_JSON_GRAMMAR_HAS_CXX_INITIALIZER_LIST
		bool xpathExists(std::initializer_list<enum Rule> path) const;
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

		#if __HEADER_DEMO_JSON_GRAMMAR_HAS_METADATA != 0
		//! Metadata
		void* metadata;
		#endif

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





} // namespace JSON
} // namespace Demo

# include "json.hxx"

#endif // __HEADER_DEMO_JSON_GRAMMAR_H__
