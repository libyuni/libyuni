#pragma once
#include <yuni/yuni.h>
#include <yuni/core/string.h>
#include <yuni/core/bind.h>
#include <yuni/core/dictionary.h>
#include <yuni/core/smartptr/intrusive.h>
#include <initializer_list>






namespace Demo
{
namespace JSON
{

	enum Rule
	{
		//! Unknown rule
		rgUnknown = 0,
		//! enum for the rule `array`
		rgArray, // = 1
		//! enum for the rule `char-extended` [inline]
		rgCharExtended, // = 2
		//! enum for the rule `digit` [inline]
		rgDigit, // = 3
		//! enum for the rule `error`
		rgError, // = 4
		//! enum for the rule `error-bracket`
		rgErrorBracket, // = 5
		//! enum for the rule `error-eol`
		rgErrorEol, // = 6
		//! enum for the rule `error-parenthese`
		rgErrorParenthese, // = 7
		//! enum for the rule `error-semicolon`
		rgErrorSemicolon, // = 8
		//! enum for the rule `false`
		rgFalse, // = 9
		//! enum for the rule `hex` [inline]
		rgHex, // = 10
		//! enum for the rule `integer`
		rgInteger, // = 11
		//! enum for the rule `key`
		rgKey, // = 12
		//! enum for the rule `null`
		rgNull, // = 13
		//! enum for the rule `number`
		rgNumber, // = 14
		//! enum for the rule `number-def` [inline]
		rgNumberDef, // = 15
		//! enum for the rule `number-exponent`
		rgNumberExponent, // = 16
		//! enum for the rule `number-hexa`
		rgNumberHexa, // = 17
		//! enum for the rule `number-qualifier`
		rgNumberQualifier, // = 18
		//! enum for the rule `number-qualifier-type`
		rgNumberQualifierType, // = 19
		//! enum for the rule `number-sign`
		rgNumberSign, // = 20
		//! enum for the rule `number-value`
		rgNumberValue, // = 21
		//! enum for the rule `object`
		rgObject, // = 22
		//! enum for the rule `object-entry`
		rgObjectEntry, // = 23
		//! enum for the rule `punc` [inline]
		rgPunc, // = 24
		//! enum for the rule `single-char` [inline]
		rgSingleChar, // = 25
		//! enum for the rule `sp` [inline]
		rgSp, // = 26
		//! enum for the rule `start`
		rgStart, // = 27
		//! enum for the rule `string`
		rgString, // = 28
		//! enum for the rule `string-literal`
		rgStringLiteral, // = 29
		//! enum for the rule `tk-brace-close`
		rgTkBraceClose, // = 30
		//! enum for the rule `tk-brace-open`
		rgTkBraceOpen, // = 31
		//! enum for the rule `tk-bracket-close`
		rgTkBracketClose, // = 32
		//! enum for the rule `tk-bracket-open`
		rgTkBracketOpen, // = 33
		//! enum for the rule `tk-colon`
		rgTkColon, // = 34
		//! enum for the rule `tk-comma`
		rgTkComma, // = 35
		//! enum for the rule `tk-comment-block`
		rgTkCommentBlock, // = 36
		//! enum for the rule `tk-comment-block-content`
		rgTkCommentBlockContent, // = 37
		//! enum for the rule `tk-comment-end-block`
		rgTkCommentEndBlock, // = 38
		//! enum for the rule `tk-comment-line`
		rgTkCommentLine, // = 39
		//! enum for the rule `tk-comment-line-content`
		rgTkCommentLineContent, // = 40
		//! enum for the rule `tk-comment-start-block`
		rgTkCommentStartBlock, // = 41
		//! enum for the rule `tk-comment-start-inline`
		rgTkCommentStartInline, // = 42
		//! enum for the rule `tk-comment-subbk` [inline]
		rgTkCommentSubbk, // = 43
		//! enum for the rule `tk-dot`
		rgTkDot, // = 44
		//! enum for the rule `tk-double-quote`
		rgTkDoubleQuote, // = 45
		//! enum for the rule `true`
		rgTrue, // = 46
		//! enum for the rule `value`
		rgValue, // = 47
		//! enum for the rule `wp` [inline]
		rgWp, // = 48
		//! enum for the final rule
		rgEOF // 49
	};


	//! The total number of rules
	constexpr const uint32_t ruleCount = 50;



	//! Convert a rule id into its text representation
	YUNI_DECL AnyString ruleToString(enum Rule);

	//! Get if a rule is an error
	YUNI_DECL bool ruleIsError(enum Rule ruleid);

	//! Get if the rule should be ignored when duplucating an AST (starting from 'tk-' and some special rules)
	YUNI_DECL bool shouldIgnoreForDuplication(enum Rule);



	enum class Error
	{
		//! No error
		none,
		//! Parse error
		parse,
	};





	class YUNI_DECL Notification final
	{
	public:
		//! Most suitable martptr
		typedef Yuni::SmartPtr<Notification> Ptr;
		//! Vector of nodes
		typedef std::vector<Ptr> Vector;

	public:
		//! Start offset
		uint32_t offset = 0;
		//! Line Index
		uint32_t line = 0;
		//! Filename
		YString message;
		//! Filename
		YString filename;

	}; // class Notification




	class YUNI_DECL Node;


	class YUNI_DECL NodeVector final
	{
	public:
		using T = Node;

		struct Iterator final: public std::iterator<std::input_iterator_tag, T>
		{
			Iterator(T** array, uint32_t index) noexcept :m_array(array), m_index{index} {}
			Iterator(const Iterator&) noexcept = default;
			Iterator& operator++() noexcept { ++m_index; return *this; }
			Iterator operator++(int) noexcept {Iterator tmp(*this); operator++(); return tmp;}
			bool operator==(const Iterator& rhs) const noexcept { return rhs.m_index == m_index; };
			bool operator!=(const Iterator& rhs) const noexcept { return rhs.m_index != m_index; };
			T& operator * () noexcept { return *(m_array[m_index]); }

		private:
			T** const m_array;
			uint32_t m_index = 0;
			friend class NodeVector;
		};

		struct ConstIterator final: public std::iterator<std::input_iterator_tag, T>
		{
			ConstIterator(T** array, uint32_t index) noexcept :m_array(array), m_index{index} {}
			ConstIterator(const ConstIterator&) noexcept = default;
			ConstIterator& operator++() noexcept { ++m_index; return *this; }
			ConstIterator operator++(int) noexcept {ConstIterator tmp(*this); operator++(); return tmp;}
			bool operator==(const ConstIterator& rhs) const noexcept { return rhs.m_index == m_index; };
			bool operator!=(const ConstIterator& rhs) const noexcept { return rhs.m_index != m_index; };
			const T& operator * () const noexcept { return *(m_array[m_index]); }

		private:
			T** const m_array;
			uint32_t m_index = 0;
			friend class NodeVector;
		};

	public:
		NodeVector() = default;
		NodeVector(const NodeVector&) = delete;
		NodeVector(NodeVector&&) = default;
		~NodeVector();

		Iterator begin() noexcept;
		Iterator end() noexcept;

		ConstIterator begin() const noexcept;
		ConstIterator end() const noexcept;

		ConstIterator cbegin() const noexcept;
		ConstIterator cend() const noexcept;

		void push_back(T* const element);

		template<class U> void push_back(U& element);

		void push_front(T* const element);

		template<class U> void push_front(U& element);

		//! Remove the last element
		void pop_back() noexcept;

		//! Empty the container
		void clear() noexcept;

		//! remove an element
		void erase(uint32_t index) noexcept;

		void erase(const Iterator& it) noexcept;

		void erase(const ConstIterator& it) noexcept;

		//! Resize the container to N elements
		void resize(uint32_t count);

		//! Get if the container is empty
		bool empty() const noexcept;

		//! Get the number of elements
		uint32_t size() const noexcept;

		//! Get the maximum number of elements before resizing
		uint32_t capacity() const noexcept;

		void shrink_to_fit() noexcept;

		//! Retrieve the last element
		T& back();
		//! Retrieve the last element (const)
		const T& back() const;

		//! Retrive the first element
		T& front();
		//! Retrive the first element (const)
		const T& front() const;

		//! Retrieve the Nth element
		T& operator [] (uint32_t i) noexcept;
		//! Retrieve the Nth element (const)
		const T& operator [] (uint32_t i) const noexcept;
		//! Move operator
		NodeVector& operator = (NodeVector&&) = default;
		//! Copy operator
		NodeVector& operator = (const NodeVector&);


	private:
		static void deleteElement(T* const ptr) noexcept;
		void grow();

	private:
		static constexpr uint32_t preAllocatedCount = 2;
		uint32_t m_size = 0;
		uint32_t m_capacity = preAllocatedCount;
		T** m_pointer = &(m_innerstorage[0]);
		T* m_innerstorage[preAllocatedCount];

		friend class Iterator;
	};




	class YUNI_DECL Node final : public Yuni::IIntrusiveSmartPtr<Node, false, Yuni::Policy::SingleThreaded>
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

		//! Callback definition for export a node
		typedef void (* ExportCallback)(const Node& node, YString& tmp);


	public:
		//! Export the tree node
		static void Export(Yuni::Clob& out, const Node& node);
		//! Export the tree node (with color output)
		static void Export(Yuni::Clob& out, const Node& node, bool color, ExportCallback callback = nullptr);
		//! Export the tree node to HTML
		static void ExportToHTML(Yuni::Clob& out, const Node& node);

		//! Export the tree node into a JSON object
		static void ExportToJSON(Yuni::Clob& out, const Node& node);
		//! Export the tree node into a JSON object (with callback for additional data)
		static void ExportToJSON(Yuni::Clob& out, const Node& node, void (*callback)(Yuni::Dictionary<AnyString, YString>::Unordered&, const Node&));


	public:
		//! Default constructor
		Node() = default;
		//! Default constructor with a pre-defined rule
		explicit Node(enum Rule);
		//! Default constructor with a pre-defined rule and a given text
		Node(enum Rule, const AnyString& text);
		//! Copy constructor
		Node(const Node& rhs) = delete;
		//! Destructor
		~Node() = default;

		void clear();

		bool empty() const;


		//! Iterate through all child nodes
		template<class F> bool each(const F& callback);

		//! Iterate through all child nodes (const)
		template<class F> bool each(const F& callback) const;

		template<class F> bool each(enum Rule rule, const F& callback);

		template<class F> bool each(enum Rule rule, const F& callback) const;

		template<class StringT> bool extractFirstChildText(StringT& out, enum Rule rule) const;

		template<class StringT> bool extractChildText(StringT& out, enum Rule rule, const AnyString& separator = nullptr) const;

		uint32_t findFirst(enum Rule rule) const;

		bool exists(enum Rule rule) const;

		const Node* xpath(std::initializer_list<enum Rule> path) const;

		Node& operator = (const Node& rhs);

		void toText(YString& out) const;


		const Node& firstChild() const;
		Node& firstChild();

		const Node& lastChild() const;
		Node& lastChild();

		Node& append(Rule);
		Node& append(Rule, Rule);
		Node& append(Rule, Rule, Rule);


	public:
		//! The rule ID
		enum Rule rule = rgUnknown;
		//! Start offset
		uint32_t offset = 0;
		//! End offset
		uint32_t offsetEnd = 0;
		//! Text associated to the node (if any)
		AnyString text;

		//! Parent node, if any
		Node* parent = nullptr;

		//! Children
		NodeVector children;
	};





	class YUNI_DECL Parser final
	{
	public:
		typedef Yuni::Bind<bool (Yuni::Clob& out, const AnyString& uri)>   OnURILoading;
		typedef Yuni::Bind<bool (const AnyString& filename, uint32_t line, uint32_t offset, Error, const YString::Vector&)>  OnError;

	public:
		Parser();
		Parser(const Parser&) = delete;
		~Parser();

		void clear();
		bool loadFromFile(const AnyString& filename);
		bool load(const AnyString& content);
		void translateOffset(uint& column, uint& line, const Node&) const;
		void translateOffset(uint& column, uint& line, uint32_t offset) const;
		uint32_t translateOffsetToLine(const Node& node) const;

		Parser& operator = (const Parser&) = delete;


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
		void* pData = nullptr;

	}; // class Parser





} // namespace JSON
} // namespace Demo

#include "json.hxx"
