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
#include "grammar.h"
#include "../../io/file.h"



namespace Yuni
{
namespace Parser
{
namespace PEG
{

	namespace // anonymous
	{

		#include "yuni/private/parser/peg/__parser.include.cpp.hxx"



		class CPPConverter final
		{
		public:
			CPPConverter(const AnyString& root, const Node::Map& rules);
			bool initialize(const AnyString& name);
			void startHeaderheaderGuardID();
			void endHeaderheaderGuardID();
			void generateH();
			void generateHXX();
			void generateCPP();
			bool writeFiles() const;

		public:
			//! Original rules
			const Node::Map& rules;

			//! Code in the .h file
			Clob h;
			//! Code in the .hxx file
			Clob hxx;
			//! Code in the .cpp file
			Clob cpp;

			//! Header guard ID
			YString headerGuardID;
			//! Class name
			YString classname;
			//! Namespace
			YString::Vector namespaces;

			//! Root filename
			YString rootfilename;
			//! Local include
			YString localInclude;

		}; // class CPPConverter



		template<class YStringT>
		static inline void EnumIDToClassname(YStringT& out, const YString& enumID)
		{
			out = "Node";
			out << enumID;
			if (not enumID.empty())
				out[4] = (char) YString::ToUpper(out[4]);
		}


		inline CPPConverter::CPPConverter(const AnyString& root, const Node::Map& rules) :
			rules(rules),
			rootfilename(root)
		{
			IO::ExtractFileName(localInclude, root);
		}


		inline bool CPPConverter::initialize(const AnyString& name)
		{
			name.split(namespaces, "/\\:");
			return true;
		}


		inline void CPPConverter::generateH()
		{
			const Node::Map::const_iterator end = rules.end();

			h << "#pragma once\n";
			h << "#include <yuni/yuni.h>\n";
			h << "#include <yuni/core/string.h>\n";
			h << "#include <yuni/core/bind.h>\n";
			h << "#include <yuni/core/dictionary.h>\n";
			h << "#include <yuni/core/smartptr/intrusive.h>\n";
			h << "#include <initializer_list>\n";
			h << "\n\n";
			h << '\n';
			h << '\n';
			h << '\n';
			h << '\n';

			for (uint32_t i = 0; i != namespaces.size(); ++i)
				h << "namespace " << namespaces[i] << "\n{\n";
			h << '\n';
			h << "	enum Rule\n";
			h << "	{\n";
			h << "		//! Unknown rule\n";
			h << "		rgUnknown = 0,\n";
			uint32_t enumIndex = 1;
			for (Node::Map::const_iterator i = rules.begin(); i != end; ++i, ++enumIndex)
			{
				h << "		//! enum for the rule `" << i->first << '`';
				if (i->second.attributes.inlined)
					h << " [inline]";
				h << '\n';
				h << "		" << i->second.enumID << ", // = " << enumIndex << '\n';
			}
			h << "		//! enum for the final rule\n";
			h << "		rgEOF // " << (enumIndex++) << '\n';
			h << "	};\n";

			// rule count
			h << '\n';
			h << '\n';
			h << "	//! The total number of rules\n";
			h << "	constexpr const uint32_t ruleCount = " << enumIndex << ";\n";
			h << "\n\n\n";

			h << "	//! Convert a rule id into its text representation\n";
			h << "	YUNI_DECL AnyString ruleToString(enum Rule);\n";
			h << '\n';
			h << "	//! Get if a rule is an error\n";
			h << "	YUNI_DECL bool ruleIsError(enum Rule ruleid);\n";
			h << '\n';
			h << "	//! Get if the rule should be ignored when duplucating an AST (starting from 'tk-' and some special rules)\n";
			h << "	YUNI_DECL bool shouldIgnoreForDuplication(enum Rule);\n";
			h << '\n';
			h << '\n';
			h << '\n';
			h << "	enum class Error\n";
			h << "	{\n";
			h << "		//! No error\n";
			h << "		none,\n";
			h << "		//! Parse error\n";
			h << "		parse,\n";
			h << "	};\n";
			h << '\n';
			h << '\n';
			h << '\n';
			h << '\n';
			h << '\n';
			h << "	class YUNI_DECL Notification final\n";
			h << "	{\n";
			h << "	public:\n";
			h << "		//! Most suitable martptr\n";
			h << "		using Ptr = Yuni::SmartPtr<Notification>;\n";
			h << "		//! Vector of nodes\n";
			h << "		using Vector = std::vector<Ptr>;\n";
			h << '\n';
			h << "	public:\n";
			h << "		//! Start offset\n";
			h << "		uint32_t offset = 0;\n";
			h << "		//! Line Index\n";
			h << "		uint32_t line = 0;\n";
			h << "		//! Filename\n";
			h << "		YString message;\n";
			h << "		//! Filename\n";
			h << "		YString filename;\n";
			h << '\n';
			h << "	}; // class Notification\n";
			h << '\n';
			h << '\n';
			h << '\n';
			h << '\n';
			h << "	class YUNI_DECL Node;\n";
			h << '\n';
			h << '\n';
			h << "	class YUNI_DECL NodeVector final\n";
			h << "	{\n";
			h << "	public:\n";
			h << "		using T = Node;\n";
			h << '\n';
			h << "		struct Iterator final: public std::iterator<std::input_iterator_tag, T>\n";
			h << "		{\n";
			h << "			Iterator(T** array, uint32_t index) noexcept :m_array(array), m_index{index} {}\n";
			h << "			Iterator(const Iterator&) noexcept = default;\n";
			h << "			Iterator& operator++() noexcept { ++m_index; return *this; }\n";
			h << "			Iterator operator++(int) noexcept {Iterator tmp(*this); operator++(); return tmp;}\n";
			h << "			bool operator==(const Iterator& rhs) const noexcept { return rhs.m_index == m_index; };\n";
			h << "			bool operator!=(const Iterator& rhs) const noexcept { return rhs.m_index != m_index; };\n";
			h << "			T& operator * () noexcept { return *(m_array[m_index]); }\n";
			h << '\n';
			h << "		private:\n";
			h << "			T** const m_array;\n";
			h << "			uint32_t m_index = 0;\n";
			h << "			friend class NodeVector;\n";
			h << "		};\n";
			h << '\n';
			h << "		struct ConstIterator final: public std::iterator<std::input_iterator_tag, T>\n";
			h << "		{\n";
			h << "			ConstIterator(T** array, uint32_t index) noexcept :m_array(array), m_index{index} {}\n";
			h << "			ConstIterator(const ConstIterator&) noexcept = default;\n";
			h << "			ConstIterator& operator++() noexcept { ++m_index; return *this; }\n";
			h << "			ConstIterator operator++(int) noexcept {ConstIterator tmp(*this); operator++(); return tmp;}\n";
			h << "			bool operator==(const ConstIterator& rhs) const noexcept { return rhs.m_index == m_index; };\n";
			h << "			bool operator!=(const ConstIterator& rhs) const noexcept { return rhs.m_index != m_index; };\n";
			h << "			const T& operator * () const noexcept { return *(m_array[m_index]); }\n";
			h << '\n';
			h << "		private:\n";
			h << "			T** const m_array;\n";
			h << "			uint32_t m_index = 0;\n";
			h << "			friend class NodeVector;\n";
			h << "		};\n";
			h << '\n';
			h << "	public:\n";
			h << "		NodeVector() = default;\n";
			h << "		NodeVector(const NodeVector&) = delete;\n";
			h << "		NodeVector(NodeVector&&) = default;\n";
			h << "		~NodeVector();\n";
			h << '\n';
			h << "		Iterator begin() noexcept;\n";
			h << "		Iterator end() noexcept;\n";
			h << '\n';
			h << "		ConstIterator begin() const noexcept;\n";
			h << "		ConstIterator end() const noexcept;\n";
			h << '\n';
			h << "		ConstIterator cbegin() const noexcept;\n";
			h << "		ConstIterator cend() const noexcept;\n";
			h << '\n';
			h << "		void push_back(T* const element);\n";
			h << '\n';
			h << "		template<class U> void push_back(U& element);\n";
			h << '\n';
			h << "		void push_front(T* const element);\n";
			h << '\n';
			h << "		template<class U> void push_front(U& element);\n";
			h << '\n';
			h << "		//! Remove the last element\n";
			h << "		void pop_back() noexcept;\n";
			h << '\n';
			h << "		//! Empty the container\n";
			h << "		void clear() noexcept;\n";
			h << '\n';
			h << "		//! remove an element\n";
			h << "		void erase(uint32_t index) noexcept;\n";
			h << '\n';
			h << "		void erase(const Iterator& it) noexcept;\n";
			h << '\n';
			h << "		void erase(const ConstIterator& it) noexcept;\n";
			h << '\n';
			h << "		//! Resize the container to N elements\n";
			h << "		void resize(uint32_t count);\n";
			h << '\n';
			h << "		//! Get if the container is empty\n";
			h << "		bool empty() const noexcept;\n";
			h << '\n';
			h << "		//! Get the number of elements\n";
			h << "		uint32_t size() const noexcept;\n";
			h << '\n';
			h << "		//! Get the maximum number of elements before resizing\n";
			h << "		uint32_t capacity() const noexcept;\n";
			h << '\n';
			h << "		void shrink_to_fit() noexcept;\n";
			h << '\n';
			h << "		//! Retrieve the last element\n";
			h << "		T& back();\n";
			h << "		//! Retrieve the last element (const)\n";
			h << "		const T& back() const;\n";
			h << '\n';
			h << "		//! Retrive the first element\n";
			h << "		T& front();\n";
			h << "		//! Retrive the first element (const)\n";
			h << "		const T& front() const;\n";
			h << '\n';
			h << "		//! Retrieve the Nth element\n";
			h << "		T& operator [] (uint32_t i) noexcept;\n";
			h << "		//! Retrieve the Nth element (const)\n";
			h << "		const T& operator [] (uint32_t i) const noexcept;\n";
			h << "		//! Move operator\n";
			h << "		NodeVector& operator = (NodeVector&&) = default;\n";
			h << "		//! Copy operator\n";
			h << "		NodeVector& operator = (const NodeVector&);\n";
			h << '\n';
			h << '\n';
			h << "	private:\n";
			h << "		static void deleteElement(T* const ptr) noexcept;\n";
			h << "		void grow();\n";
			h << '\n';
			h << "	private:\n";
			h << "		static constexpr uint32_t preAllocatedCount = 2;\n";
			h << "		uint32_t m_size = 0;\n";
			h << "		uint32_t m_capacity = preAllocatedCount;\n";
			h << "		T** m_pointer = &(m_innerstorage[0]);\n";
			h << "		T* m_innerstorage[preAllocatedCount];\n";
			h << '\n';
			h << "		friend struct Iterator;\n";
			h << "	};\n";
			h << '\n';
			h << '\n';
			h << '\n';
			h << '\n';
			h << "	class YUNI_DECL Node final : public Yuni::IIntrusiveSmartPtr<Node, false, Yuni::Policy::SingleThreaded>\n";
			h << "	{\n";
			h << "	public:\n";
			h << "		//! Ancestor\n";
			h << "		using inherited = Yuni::IIntrusiveSmartPtr<Node, false, Yuni::Policy::SingleThreaded>;\n";
			h << "		//! Threading policy\n";
			h << "		using ThreadingPolicy = inherited::ThreadingPolicy;\n";
			h << "		//! The most suitable smart ptr for the class\n";
			h << "		using Ptr = inherited::SmartPtrType<Node>::Ptr;\n";
			h << '\n';
			h << "		//! Callback definition for export a node\n";
			h << "		using ExportCallback = void (*)(const Node& node, YString& tmp);\n";
			h << '\n';
			h << '\n';
			h << "	public:\n";
			h << "		//! Export the tree node\n";
			h << "		static void Export(Yuni::Clob& out, const Node& node);\n";
			h << "		//! Export the tree node (with color output)\n";
			h << "		static void Export(Yuni::Clob& out, const Node& node, bool color, ExportCallback callback = nullptr);\n";
			h << "		//! Export the tree node to HTML\n";
			h << "		static void ExportToHTML(Yuni::Clob& out, const Node& node);\n";
			h << '\n';
			h << "		//! Export the tree node into a JSON object\n";
			h << "		static void ExportToJSON(Yuni::Clob& out, const Node& node);\n";
			h << "		//! Export the tree node into a JSON object (with callback for additional data)\n";
			h << "		static void ExportToJSON(Yuni::Clob& out, const Node& node, void (*callback)(Yuni::Dictionary<AnyString, YString>::Unordered&, const Node&));\n";
			h << '\n';
			h << '\n';
			h << "	public:\n";
			h << "		//! Default constructor\n";
			h << "		Node() = default;\n";
			h << "		//! Default constructor with a pre-defined rule\n";
			h << "		explicit Node(enum Rule);\n";
			h << "		//! Default constructor with a pre-defined rule and a given text\n";
			h << "		Node(enum Rule, const AnyString& text);\n";
			h << "		//! Copy constructor\n";
			h << "		Node(const Node& rhs) = delete;\n";
			h << "		//! Destructor\n";
			h << "		~Node() = default;\n";
			h << '\n';
			h << "		void clear();\n";
			h << '\n';
			h << "		bool empty() const;\n";
			h << '\n';
		//	h << "		void swap(Node&);\n";
			h << '\n';
			h << "		//! Iterate through all child nodes\n";
			h << "		template<class F> bool each(const F& callback);\n";
			h << '\n';
			h << "		//! Iterate through all child nodes (const)\n";
			h << "		template<class F> bool each(const F& callback) const;\n";
			h << '\n';
			h << "		template<class F> bool each(enum Rule rule, const F& callback);\n";
			h << '\n';
			h << "		template<class F> bool each(enum Rule rule, const F& callback) const;\n";
			h << '\n';
			h << "		template<class StringT> bool extractFirstChildText(StringT& out, enum Rule rule) const;\n";
			h << '\n';
			h << "		template<class StringT> bool extractChildText(StringT& out, enum Rule rule, const AnyString& separator = nullptr) const;\n";
			h << '\n';
			h << "		uint32_t findFirst(enum Rule rule) const;\n";
			h << '\n';
			h << "		bool exists(enum Rule rule) const;\n";
			h << '\n';
			h << "		const Node* xpath(std::initializer_list<enum Rule> path) const;\n";
			// h << "		Node::Ptr  xpath(enum Rule path) const;\n";
			h << '\n';
			h << "		Node& operator = (const Node& rhs);\n";
			h << '\n';
			h << "		void toText(YString& out) const;\n";
			h << '\n';
			h << '\n';
			h << "		const Node& firstChild() const;\n";
			h << "		Node& firstChild();\n";
			h << '\n';
			h << "		const Node& lastChild() const;\n";
			h << "		Node& lastChild();\n";
			h << '\n';
			h << "		Node& append(Rule);\n";
			h << "		Node& append(Rule, Rule);\n";
			h << "		Node& append(Rule, Rule, Rule);\n";
			h << '\n';
			h << '\n';
			h << "	public:\n";
			h << "		//! The rule ID\n";
			h << "		enum Rule rule = rgUnknown;\n";
			h << "		//! Start offset\n";
			h << "		uint32_t offset = 0;\n";
			h << "		//! End offset\n";
			h << "		uint32_t offsetEnd = 0;\n";
			h << "		//! Text associated to the node (if any)\n";
			h << "		AnyString text;\n";
			h << '\n';
			h << "		//! Parent node, if any\n";
			h << "		Node* parent = nullptr;\n";
			h << '\n';
			h << "		//! Children\n";
			h << "		NodeVector children;\n";
			h << "	};\n";
			h << '\n';
			h << '\n';
			h << '\n';
			h << '\n';
			h << '\n';
			h << "	class YUNI_DECL Parser final\n";
			h << "	{\n";
			h << "	public:\n";
			h << "		using OnURILoading = Yuni::Bind<bool (Yuni::Clob& out, const AnyString& uri)>;\n";
			h << "		using OnError = Yuni::Bind<bool (const AnyString& filename, uint32_t line, uint32_t offset, Error, const YString::Vector&)>;\n";
			h << '\n';
			h << "	public:\n";
			h << "		Parser();\n";
			h << "		Parser(const Parser&) = delete;\n";
			h << "		~Parser();\n";
			h << '\n';
			h << "		void clear();\n";
			h << "		bool loadFromFile(const AnyString& filename);\n";
			h << "		bool load(const AnyString& content);\n";
			h << "		void translateOffset(uint& column, uint& line, const Node&) const;\n";
			h << "		void translateOffset(uint& column, uint& line, uint32_t offset) const;\n";
			h << "		uint32_t translateOffsetToLine(const Node& node) const;\n";
			h << '\n';
			h << "		Parser& operator = (const Parser&) = delete;\n";
			h << '\n';
			h << '\n';
			h << "	public:\n";
			h << "		//! Event: load another include file\n";
			h << "		OnURILoading onURILoading;\n";
			h << "		//! Event: load another include file\n";
			h << "		OnError  onError;\n";
			h << '\n';
			h << "		//! The root node, if any\n";
			h << "		Node::Ptr root;\n";
			h << '\n';
			h << "		//! Notifications\n";
			h << "		Notification::Vector notifications;\n";
			h << '\n';
			h << '\n';
			h << "	private:\n";
			h << "		void* pData = nullptr;\n";
			h << '\n';
			h << "	}; // class Parser\n";
		}


		inline void CPPConverter::generateHXX()
		{
			hxx << "#pragma once\n";
			hxx << "\n\n\n\n";


			for (uint32_t i = 0; i != namespaces.size(); ++i)
				hxx << "namespace " << namespaces[i] << "\n{\n";
			hxx << '\n';
			hxx << '\n';
			hxx << "	inline NodeVector::T& NodeVector::back()\n";
			hxx << "	{\n";
			hxx << "		assert(m_size != 0);\n";
			hxx << "		return *(m_pointer[m_size - 1]);\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	inline const NodeVector::T& NodeVector::back() const\n";
			hxx << "	{\n";
			hxx << "		assert(m_size != 0);\n";
			hxx << "		return *(m_pointer[m_size - 1]);\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	inline NodeVector::T& NodeVector::front()\n";
			hxx << "	{\n";
			hxx << "		assert(m_size != 0);\n";
			hxx << "		return *(m_pointer[0]);\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	inline const NodeVector::T& NodeVector::front() const\n";
			hxx << "	{\n";
			hxx << "		assert(m_size != 0);\n";
			hxx << "		return *(m_pointer[0]);\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	inline bool NodeVector::empty() const noexcept\n";
			hxx << "	{\n";
			hxx << "		return (0 == m_size);\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	inline uint32_t NodeVector::size() const noexcept\n";
			hxx << "	{\n";
			hxx << "		return m_size;\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	inline uint32_t NodeVector::capacity() const noexcept\n";
			hxx << "	{\n";
			hxx << "		return m_capacity;\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	inline NodeVector::T& NodeVector::operator [] (uint32_t i) noexcept\n";
			hxx << "	{\n";
			hxx << "		assert(i < m_size);\n";
			hxx << "		return *(m_pointer[i]);\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	inline const NodeVector::T& NodeVector::operator [] (uint32_t i) const noexcept\n";
			hxx << "	{\n";
			hxx << "		assert(i < m_size);\n";
			hxx << "		return *(m_pointer[i]);\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	inline void NodeVector::erase(const Iterator& it) noexcept\n";
			hxx << "	{\n";
			hxx << "		erase(it.m_index);\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	inline void NodeVector::erase(const ConstIterator& it) noexcept\n";
			hxx << "	{\n";
			hxx << "		erase(it.m_index);\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	template<class U>\n";
			hxx << "	inline void NodeVector::push_back(U& element)\n";
			hxx << "	{\n";
			hxx << "		push_back(T::Ptr::WeakPointer(element));\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	template<class U>\n";
			hxx << "	inline void NodeVector::push_front(U& element)\n";
			hxx << "	{\n";
			hxx << "		push_front(T::Ptr::WeakPointer(element));\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	inline NodeVector::Iterator NodeVector::begin() noexcept\n";
			hxx << "	{\n";
			hxx << "		return Iterator(m_pointer, 0);\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	inline NodeVector::Iterator NodeVector::end() noexcept\n";
			hxx << "	{\n";
			hxx << "		return Iterator(m_pointer, m_size);\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	inline NodeVector::ConstIterator NodeVector::begin() const noexcept\n";
			hxx << "	{\n";
			hxx << "		return ConstIterator(m_pointer, 0);\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	inline NodeVector::ConstIterator NodeVector::end() const noexcept\n";
			hxx << "	{\n";
			hxx << "		return ConstIterator(m_pointer, m_size);\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	inline NodeVector::ConstIterator NodeVector::cbegin() const noexcept\n";
			hxx << "	{\n";
			hxx << "		return ConstIterator(m_pointer, 0);\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	inline NodeVector::ConstIterator NodeVector::cend() const noexcept\n";
			hxx << "	{\n";
			hxx << "		return ConstIterator(m_pointer, m_size);\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	inline void NodeVector::deleteElement(T* const element) noexcept\n";
			hxx << "	{\n";
			hxx << "		if (element->release())\n";
			hxx << "			delete element;\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	inline NodeVector& NodeVector::operator = (const NodeVector& rhs)\n";
			hxx << "	{\n";
			hxx << "		clear();\n";
			hxx << "		for (uint32_t i = 0; i != rhs.m_size; ++i)\n";
			hxx << "			push_back(rhs.m_pointer[i]);\n";
			hxx << "		return *this;";
			hxx << "	}\n";
			hxx << '\n';
			hxx << '\n';
			hxx << '\n';
			hxx << '\n';
			hxx << "	//! References for tokens\n";
			hxx << "	static constexpr const bool isToken[] =\n";
			hxx << "	{\n";
			hxx << "		false, // rgUnknown\n";
			for (auto& pair: rules)
			{
				if (pair.first.startsWith("tk-"))
					hxx << "		true, // " << pair.first << '\n';
				else
					hxx << "		false, // " << pair.first << '\n';
			}
			hxx << "	};\n";
			hxx << "\n\n";
			hxx << "	inline bool shouldIgnoreForDuplication(enum Rule rule)\n";
			hxx << "	{\n";
			hxx << "		//assert(static_cast<uint32_t>(rule) < " << (rules.size() + 1) << ");\n";
			hxx << "		return isToken[static_cast<uint32_t>(rule)];\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << '\n';
			hxx << "	static constexpr const bool isAttributeError[] =\n";
			hxx << "	{\n";
			hxx << "		false, // rgUnknown\n";
			for (auto& pair: rules)
			{
				bool error = pair.first == "error" or pair.first.startsWith("error-");
				hxx << "		" << (error ? "true" : "false") << ", // " << pair.second.enumID << '\n';
			}
			hxx << "		false, // rgEOF\n";
			hxx << "	};\n";
			hxx << '\n';
			hxx << '\n';
			hxx << "	inline bool ruleIsError(enum Rule ruleid)\n";
			hxx << "	{\n";
			hxx << "		// assert((uint) ruleid < (uint) ruleCount);\n";
			hxx << "		return isAttributeError[static_cast<uint32_t>(ruleid)];\n";
			hxx << "	}\n";

			hxx << "\n\n\n\n";
			hxx << "	inline Node::Node(enum Rule rule)\n";
			hxx << "		: rule(rule)\n";
			hxx << "	{}\n";
			hxx << '\n';
			hxx << '\n';
			hxx << "	inline Node::Node(enum Rule rule, const AnyString& text)\n";
			hxx << "		: rule(rule)\n";
			hxx << "		, text(text)\n";
			hxx << "	{}\n";
			hxx << '\n';
			hxx << '\n';
			hxx << "	template<class F> inline bool Node::each(const F& callback)\n";
			hxx << "	{\n";
			hxx << "		for (uint32_t i = 0; i != children.size(); ++i)\n";
			hxx << "		{\n";
			hxx << "			if (not callback(children[i]))\n";
			hxx << "				return false;\n";
			hxx << "		}\n";
			hxx << "		return true;\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << '\n';
			hxx << "	template<class F> inline bool Node::each(const F& callback) const\n";
			hxx << "	{\n";
			hxx << "		for (uint32_t i = 0; i != children.size(); ++i)\n";
			hxx << "		{\n";
			hxx << "			if (not callback(children[i]))\n";
			hxx << "				return false;\n";
			hxx << "		}\n";
			hxx << "		return true;\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << '\n';
			hxx << "	template<class F> inline bool Node::each(enum Rule rule, const F& callback)\n";
			hxx << "	{\n";
			hxx << "		for (uint32_t i = 0; i != children.size(); ++i)\n";
			hxx << "		{\n";
			hxx << "			Node& subnode = children[i];\n";
			hxx << "			if (subnode.rule == rule and not callback(subnode))\n";
			hxx << "				return false;\n";
			hxx << "		}\n";
			hxx << "		return true;\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << '\n';
			hxx << "	template<class F> inline bool Node::each(enum Rule rule, const F& callback) const\n";
			hxx << "	{\n";
			hxx << "		for (uint32_t i = 0; i != children.size(); ++i)\n";
			hxx << "		{\n";
			hxx << "			const Node& subnode = children[i];\n";
			hxx << "			if (subnode.rule == rule and not callback(subnode))\n";
			hxx << "				return false;\n";
			hxx << "		}\n";
			hxx << "		return true;\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << '\n';
			hxx << "	template<class StringT> inline bool Node::extractFirstChildText(StringT& out, enum Rule rule) const\n";
			hxx << "	{\n";
			hxx << "		for (uint32_t i = 0; i != children.size(); ++i)\n";
			hxx << "		{\n";
			hxx << "			const Node& subnode = children[i];\n";
			hxx << "			if (subnode.rule == rule)\n";
			hxx << "			{\n";
			hxx << "				out += subnode.text;\n";
			hxx << "				return true;\n";
			hxx << "			}\n";
			hxx << "		}\n";
			hxx << "		return false;\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << '\n';
			hxx << "	template<class StringT> inline bool Node::extractChildText(StringT& out, enum Rule rule, const AnyString& separator) const\n";
			hxx << "	{\n";
			hxx << "		bool somethingFound = false;\n";
			hxx << '\n';
			hxx << "		if (separator.empty())\n";
			hxx << "		{\n";
			hxx << "			for (uint32_t i = 0; i != children.size(); ++i)\n";
			hxx << "			{\n";
			hxx << "				const Node& subnode = children[i];\n";
			hxx << "				if (subnode.rule == rule)\n";
			hxx << "				{\n";
			hxx << "					out += subnode.text;\n";
			hxx << "					somethingFound = true;\n";
			hxx << "				}\n";
			hxx << "			}\n";
			hxx << "		}\n";
			hxx << "		else\n";
			hxx << "		{\n";
			hxx << "			for (uint32_t i = 0; i != children.size(); ++i)\n";
			hxx << "			{\n";
			hxx << "				const Node& subnode = children[i];\n";
			hxx << "				if (subnode.rule == rule)\n";
			hxx << "				{\n";
			hxx << "					if (not out.empty())\n";
			hxx << "						out += separator;\n";
			hxx << "					out += subnode.text;\n";
			hxx << "					somethingFound = true;\n";
			hxx << "				}\n";
			hxx << "			}\n";
			hxx << "		}\n";
			hxx << "		return somethingFound;\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << '\n';
			hxx << "	inline uint32_t Node::findFirst(enum Rule rule) const\n";
			hxx << "	{\n";
			hxx << "		for (uint32_t i = 0; i != children.size(); ++i)\n";
			hxx << "		{\n";
			hxx << "			if (children[i].rule == rule)\n";
			hxx << "				return i;\n";
			hxx << "		}\n";
			hxx << "		return (uint)-1;\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << '\n';
			hxx << "	inline bool Node::empty() const\n";
			hxx << "	{\n";
			hxx << "		return children.empty();\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << '\n';
			hxx << "	inline bool Node::exists(enum Rule rule) const\n";
			hxx << "	{\n";
			hxx << "		for (uint32_t i = 0; i != children.size(); ++i)\n";
			hxx << "		{\n";
			hxx << "			if (children[i].rule == rule)\n";
			hxx << "				return true;\n";
			hxx << "		}\n";
			hxx << "		return false;\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << '\n';
			hxx << "	inline const Node& Node::firstChild() const\n";
			hxx << "	{\n";
			hxx << "		return children.front();\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << '\n';
			hxx << "	inline Node& Node::firstChild()\n";
			hxx << "	{\n";
			hxx << "		return children.front();\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << '\n';
			hxx << "	inline const Node& Node::lastChild() const\n";
			hxx << "	{\n";
			hxx << "		return children.back();\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << '\n';
			hxx << "	inline Node& Node::lastChild()\n";
			hxx << "	{\n";
			hxx << "		return children.back();\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << '\n';
			hxx << "	inline const Node* Node::xpath(std::initializer_list<enum Rule> path) const\n";
			hxx << "	{\n";
			hxx << "		if (path.size() > 0)\n";
			hxx << "		{\n";
			hxx << "			auto it = path.begin();\n";
			hxx << "			for (auto& child: children)\n";
			hxx << "			{\n";
			hxx << "				if (child.rule == *it)\n";
			hxx << "				{\n";
			hxx << "					const Node* result = &child;\n";
			hxx << "					++it;\n";
			hxx << "					for (; it != path.end(); ++it)\n";
			hxx << "					{\n";
			hxx << "						bool found = false;\n";
			hxx << "						auto& subnode = *result;\n";
			hxx << "						for (uint32_t j = 0; j != subnode.children.size(); ++j)\n";
			hxx << "						{\n";
			hxx << "							if (subnode.children[j].rule == *it)\n";
			hxx << "							{\n";
			hxx << "								result = &(subnode.children[j]);\n";
			hxx << "								found = true;\n";
			hxx << "								break;\n";
			hxx << "							}\n";
			hxx << "						}\n";
			hxx << "						if (not found)\n";
			hxx << "							return nullptr;\n";
			hxx << "					}\n";
			hxx << "					return result;\n";
			hxx << "				}\n";
			hxx << "			}\n";
			hxx << "		}\n";
			hxx << "		return nullptr;\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << '\n';
			hxx << "	inline Node& Node::append(Rule r1)\n";
			hxx << "	{\n";
			hxx << "		Node* node = new Node{r1};\n";
			hxx << "		children.push_back(node);\n";
			hxx << "		return *node;\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	inline Node& Node::append(Rule r1, Rule r2)\n";
			hxx << "	{\n";
			hxx << "		return append(r1).append(r2);\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << "	inline Node& Node::append(Rule r1, Rule r2, Rule r3)";
			hxx << "	{\n";
			hxx << "		return append(r1).append(r2).append(r3);\n";
			hxx << "	}\n";
			hxx << '\n';
			hxx << '\n';
			// hxx << "	inline Node::Ptr  Node::xpath(enum Rule path) const\n";
			// hxx << "	{\n";
			// hxx << "		for (uint32_t i = 0; i != (uint) children.size(); ++i)\n";
			// hxx << "		{\n";
			// hxx << "			if (children[i]->rule == path)\n";
			// hxx << "				return children[i];\n";
			// hxx << "		}\n";
			// hxx << "		return nullptr;\n";
			// hxx << "	}\n";
			// hxx << '\n';
			// hxx << '\n';
			hxx << '\n';
			hxx << '\n';
		}


		static inline void GenerateFunctionForEachRule(Clob& cpp, uint& sp, const Node::Map& rules, const AnyString& name, const Node& node)
		{
			Clob body;

			// subnodes
			{
				// We have to reserve from the begining this vector, to avoir future calls to
				// std::vector::resize, and to invalidate references to internal elements
				Clob::Vector helpers;
				String::Vector datatext;
				helpers.reserve(node.treeCount());

				// skip the first AND block, which is useless actually
				if (node.rule.type == Node::asAND)
				{
					for (uint32_t i = 0; i != (uint) node.children.size(); ++i)
						node.children[i].exportCPP(body, rules, helpers, datatext, 2, true, sp);
				}
				else
					node.exportCPP(body, rules, helpers, datatext, 2, true, sp);

				// helpers
				bool foundHelper = false;

				if (not datatext.empty())
				{
					for (uint32_t i = 0; i != (uint) datatext.size(); ++i)
						cpp << '	' << datatext[i] << '\n';
					cpp << "\n\n\n";
				}

				for (uint32_t i = 0; i != (uint) helpers.size(); ++i)
				{
					if (not helpers[i].empty())
					{
						if (not foundHelper)
						{
							foundHelper = true;
							cpp << "\n\n";
						}
						// extract forward declaration
						uint32_t linefeed = helpers[i].find('\n');
						if (linefeed < helpers[i].size())
							cpp << AnyString(helpers[i], 0, linefeed) << ";\n";
					}
				}

				foundHelper = false;
				for (uint32_t i = 0; i != (uint) helpers.size(); ++i)
				{
					if (not helpers[i].empty())
					{
						if (not foundHelper)
						{
							foundHelper = true;
							cpp << "\n\n";
						}
						cpp << helpers[i] << '\n';
					}
				}
			}

			cpp << "	//! Rule " << name << '\n';
			cpp << "	";
			cpp << ((node.enumID != "Rule::start") ? "static inline " : "static ");

			cpp << "bool yy" << node.enumID << "(Datasource& ctx)\n";
			cpp << "	{\n";
			cpp << "		(void) ctx;\n";
			cpp << "		TRACE(\"entering " << node.enumID;
			if (node.attributes.inlined)
				cpp << " [inline]";
			cpp << "\");\n";

			if (not node.attributes.inlined)
				cpp << "		uint32_t _ruleOffset = ctx.enterRule(" << node.enumID << ");\n";
			cpp << '\n';
			cpp << body;
			cpp << '\n';
			if (not node.attributes.inlined)
				cpp << "		ctx.commit(_ruleOffset, " << node.enumID << ");\n";
			cpp << "		return true;\n";
			cpp << "	}\n";
			cpp << "\n\n";
		}


		inline void CPPConverter::generateCPP()
		{
			const Node::Map::const_iterator end = rules.end();
			cpp << '\n';
			cpp << "#include \"" << localInclude << "h\"\n";
			cpp << "#include <cassert>\n";
			cpp << "#include <yuni/core/string.h>\n";
			cpp << "#include <yuni/io/file.h>\n";
			cpp << "#include <yuni/io/directory.h>\n";
			cpp << "#include <yuni/core/dictionary.h>\n";
			cpp << "#include <yuni/core/noncopyable.h>\n";
			cpp << "#include <yuni/datetime/timestamp.h>\n";
			cpp << "#include <yuni/core/system/console/console.h>\n";
			cpp << "#include <iostream>\n";
			cpp << '\n';
			cpp << "using namespace Yuni;\n";
			cpp << "\n\n\n\n";

			cpp << "std::ostream& operator << (std::ostream& out, const ";
			for (uint32_t i = 0; i != namespaces.size(); ++i)
				cpp << "::" << namespaces[i];
			cpp << "::Node& node)\n";
			cpp << "{\n";
			cpp << "	Clob content;\n";
			cpp << "	";
			for (uint32_t i = 0; i != namespaces.size(); ++i)
				cpp << "::" << namespaces[i];
			cpp << "::Node::Export(content, node);";
			cpp << "out << content;\n";
			cpp << "	return out;\n";
			cpp << "}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "std::ostream& operator << (std::ostream& out, const ";
			for (uint32_t i = 0; i != namespaces.size(); ++i)
				cpp << "::" << namespaces[i];
			cpp << "::Node::Ptr node)\n";
			cpp << "{\n";
			cpp << "	if (!!node)\n";
			cpp << "		out << (*node);\n";
			cpp << "	else\n";
			cpp << "		out << \"<invalid ast node>\";\n";
			cpp << "	return out;\n";
			cpp << "}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';


			for (uint32_t i = 0; i != namespaces.size(); ++i)
				cpp << "namespace " << namespaces[i] << "\n{\n";

			cpp << "namespace // anonymous\n{\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	static constexpr const bool _attrAttributeCapture[] =\n";
			cpp << "	{\n";
			cpp << "		false, // rgUnknown\n";
			for (Node::Map::const_iterator i = rules.begin(); i != end; ++i)
				cpp << "		" << (i->second.attributes.capture ? "true" : "false") << ", // " << i->second.enumID << '\n';
			cpp << "		false, // rgEOF\n";
			cpp << "	};\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	static bool ruleAttributeCapture(enum Rule ruleid)\n";
			cpp << "	{\n";
			cpp << "		assert((uint) ruleid < (uint) ruleCount);\n";
			cpp << "		return _attrAttributeCapture[(uint) ruleid];\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';


			cpp << "	static constexpr const bool _attrAttributeImportant[] = {\n";
			cpp << "		false, // rgUnknown\n";
			for (Node::Map::const_iterator i = rules.begin(); i != end; ++i)
				cpp << "		" << (i->second.attributes.important ? "true" : "false") << ", // " << i->second.enumID << '\n';
			cpp << "		false, // rgEOF\n";
			cpp << "	};\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	static inline bool ruleAttributeImportant(enum Rule ruleid)\n";
			cpp << "	{\n";
			cpp << "		assert((uint) ruleid < (uint) ruleCount);\n";
			cpp << "		return _attrAttributeImportant[(uint) ruleid];\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << "	static const char* const _attrAttributeSimpleTextCapture[] =\n";
			cpp << "	{\n";
			cpp << "		nullptr, // rgUnknown\n";
			String textCapture;
			for (Node::Map::const_iterator i = rules.begin(); i != end; ++i)
			{
				cpp << "		";
				if (i->second.children.size() == 1 and i->second.children[0].isSimpleTextCapture())
				{
					textCapture = i->second.children[0].rule.text;
					textCapture.replace("\\", "\\\\");
					textCapture.replace("\"", "\\\"");
					cpp << '"' << textCapture << '"';
				}
				else
					cpp << "nullptr";

				cpp << ", // " << i->second.enumID << '\n';
			}
			cpp << "		nullptr, // rgEOF\n";
			cpp << "	};\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	static AnyString ruleAttributeSimpleTextCapture(enum Rule ruleid)\n";
			cpp << "	{\n";
			cpp << "		assert((uint) ruleid < (uint) ruleCount);\n";
			cpp << "		return _attrAttributeSimpleTextCapture[(uint) ruleid];\n";
			cpp << "	}\n\n\n\n";

			PrepareCPPInclude(cpp);

			cpp << "	// Forward declarations\n";
			for (Node::Map::const_iterator i = rules.begin(); i != end; ++i)
				cpp << "	static inline bool yy" << i->second.enumID << "(Datasource& ctx);\n";

			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';

			// generate all rules
			{
				uint32_t sp = 0;
				for (Node::Map::const_iterator i = rules.begin(); i != end; ++i)
					GenerateFunctionForEachRule(cpp, sp, rules, i->first, i->second);
			}

			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << "} // anonymous namespace\n";
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';


			cpp << "	NodeVector::~NodeVector()\n";
			cpp << "	{\n";
			cpp << "		uint32_t size = m_size;\n";
			cpp << "		for (uint32_t i = 0; i != size; ++i)\n";
			cpp << "			deleteElement(m_pointer[i]);\n";
			cpp << "		if (m_capacity != preAllocatedCount)\n";
			cpp << "			free(m_pointer);\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << "	void NodeVector::shrink_to_fit() noexcept\n";
			cpp << "	{\n";
			cpp << "		if (m_capacity != preAllocatedCount)\n";
			cpp << "		{\n";
			cpp << "			if (m_size == 0)\n";
			cpp << "			{\n";
			cpp << "				free(m_pointer);\n";
			cpp << "				m_pointer = &(m_innerstorage[0]);\n";
			cpp << "				m_capacity = preAllocatedCount;\n";
			cpp << "			}\n";
			cpp << "		}\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	void NodeVector::push_front(NodeVector::T* const element)\n";
			cpp << "	{\n";
			cpp << "		assert(element != nullptr);\n";
			cpp << "		uint32_t size = m_size;\n";
			cpp << "		if (size == 0)\n";
			cpp << "			push_back(element);\n";
			cpp << '\n';
			cpp << "		uint32_t newsize = size + 1;\n";
			cpp << "		if (not (newsize < m_capacity))\n";
			cpp << "			grow();\n";
			cpp << "		uint32_t i = size;\n";
			cpp << "		while (i-- > 0)\n";
			cpp << "			m_pointer[i + 1] = m_pointer[i];\n";
			cpp << "		m_size = newsize;\n";
			cpp << "		element->addRef();\n";
			cpp << "		m_pointer[0] = element;\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	void NodeVector::resize(uint32_t count)\n";
			cpp << "	{\n";
			cpp << "		uint32_t size = m_size;\n";
			cpp << "		if (count != size)\n";
			cpp << "		{\n";
			cpp << "			if (count > size)\n";
			cpp << "			{\n";
			cpp << "				for (uint32_t i = size; i != count; ++i)\n";
			cpp << "					push_back(new T);\n";
			cpp << "			}\n";
			cpp << "			else\n";
			cpp << "			{\n";
			cpp << "				for (uint32_t i = count; i != size; ++i)\n";
			cpp << "					deleteElement(m_pointer[i]);\n";
			cpp << "			}\n";
			cpp << "			m_size = count;\n";
			cpp << "		}\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	void NodeVector::erase(uint32_t index) noexcept\n";
			cpp << "	{\n";
			cpp << "		uint32_t size = m_size;\n";
			cpp << "		if (index < size)\n";
			cpp << "		{\n";
			cpp << "			deleteElement(m_pointer[index]);\n";
			cpp << "			for (uint32_t i = index + 1; i < size; ++i)\n";
			cpp << "				m_pointer[i - 1] = m_pointer[i];\n";
			cpp << "			m_size = --size;\n";
			cpp << "		}\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	void NodeVector::clear() noexcept\n";
			cpp << "	{\n";
			cpp << "		uint32_t size = m_size;\n";
			cpp << "		if (size)\n";
			cpp << "		{\n";
			cpp << "			for (uint32_t i = 0; i != size; ++i)\n";
			cpp << "				deleteElement(m_pointer[i]);\n";
			cpp << "			m_size = 0;\n";
			cpp << "		}\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	void NodeVector::pop_back() noexcept\n";
			cpp << "	{\n";
			cpp << "		uint32_t size = m_size;\n";
			cpp << "		if (size != 0)\n";
			cpp << "		{\n";
			cpp << "			--size;\n";
			cpp << "			deleteElement(m_pointer[size]);\n";
			cpp << "			m_size = size;\n";
			cpp << "		}\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	void NodeVector::push_back(NodeVector::T* const element)\n";
			cpp << "	{\n";
			cpp << "		assert(element != nullptr);\n";
			cpp << "		uint32_t oldsize = m_size;\n";
			cpp << "		uint32_t newsize = oldsize + 1;\n";
			cpp << "		if (not (newsize < m_capacity))\n";
			cpp << "			grow();\n";
			cpp << "		m_size = newsize;\n";
			cpp << "		element->addRef();\n";
			cpp << "		m_pointer[oldsize] = element;\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	void NodeVector::grow()\n";
			cpp << "	{\n";
			cpp << "		uint32_t newcapacity;\n";
			cpp << "		T** newptr;\n";
			cpp << "		if (m_capacity == preAllocatedCount)\n";
			cpp << "		{\n";
			cpp << "			newcapacity = 8;\n";
			cpp << "			newptr = (T**) malloc(sizeof(T*) * newcapacity);\n";
			cpp << "			if (!newptr)\n";
			cpp << "			{\n";
			cpp << "				#if __cpp_exceptions >= 199711\n";
			cpp << "				throw std::bad_alloc();\n";
			cpp << "				#else\n";
			cpp << "				abort();\n";
			cpp << "				#endif\n";
			cpp << "			}\n";
			cpp << "			newptr[0] = m_innerstorage[0];\n";
			cpp << "			newptr[1] = m_innerstorage[1];\n";
			cpp << "		}\n";
			cpp << "		else\n";
			cpp << "		{\n";
			cpp << "			newcapacity = m_capacity + 8;\n";
			cpp << "			newptr = (T**) realloc(m_pointer, sizeof(T*) * newcapacity);\n";
			cpp << "			if (!newptr)\n";
			cpp << "			{\n";
			cpp << "				#if __cpp_exceptions >= 199711\n";
			cpp << "				throw std::bad_alloc();\n";
			cpp << "				#else\n";
			cpp << "				abort();\n";
			cpp << "				#endif\n";
			cpp << "			}\n";
			cpp << "		}\n";
			cpp << "		m_capacity = newcapacity;\n";
			cpp << "		m_pointer = newptr;\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << "	AnyString ruleToString(Rule ruleid)\n";
			cpp << "	{\n";
			cpp << "		switch (ruleid)\n";
			cpp << "		{\n";
			cpp << "			case rgUnknown:\n";
			cpp << "				return \"<unknown>\";\n";
			for (Node::Map::const_iterator i = rules.begin(); i != end; ++i)
			{
				cpp << "			case " << i->second.enumID << ":\n";
				cpp << "				return \"" << i->first << "\";\n";
			}
			cpp << "			case rgEOF:\n";
			cpp << "				return \"EOF\";\n";
			cpp << "		}\n";
			cpp << "		return \"<error>\";\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << "	Parser::Parser()\n";
			cpp << "	{\n";
			cpp << "		onURILoading.bind(& StandardURILoaderHandler);\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	Parser::~Parser()\n";
			cpp << "	{\n";
			cpp << "		delete (Datasource*) pData;\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	void Parser::clear()\n";
			cpp << "	{\n";
			cpp << "		root = nullptr;\n";
			cpp << "		delete (Datasource*) pData;\n";
			cpp << "		pData = nullptr;\n";
			cpp << "		if (not notifications.empty())\n";
			cpp << "			Notification::Vector().swap(notifications);\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	bool Parser::loadFromFile(const AnyString& filename)\n";
			cpp << "	{\n";
			cpp << "		if (!pData)\n";
			cpp << "			pData = new Datasource(notifications);\n";
			cpp << '\n';
			cpp << "		Datasource& ctx = *((Datasource*) pData);\n";
			cpp << "		switch (ctx.open(filename))\n";
			cpp << "		{\n";
			cpp << "			case Datasource::OpenFlag::opened: {\n";
			cpp << "				DATASOURCE_PARSE(ctx);\n";
			cpp << "				return ctx.success;\n";
			cpp << "			}\n";
			cpp << "			case Datasource::OpenFlag::ignore: {\n";
			cpp << "				return true;";
			cpp << "			}\n";
			cpp << "			case Datasource::OpenFlag::error: {\n";
			cpp << "				return false;";
			cpp << "			}\n";
			cpp << "		}\n";
			cpp << "		return false;\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	bool Parser::load(const AnyString& content)\n";
			cpp << "	{\n";
			cpp << "		if (!pData)\n";
			cpp << "			pData = new Datasource(notifications);\n";
			cpp << '\n';
			cpp << "		Datasource& ctx = *((Datasource*) pData);\n";
			cpp << "		ctx.openContent(content);\n";
			cpp << "		DATASOURCE_PARSE(ctx);\n";
			cpp << "		return ctx.success;\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	void Parser::translateOffset(uint& column, uint& line, const Node& node) const\n";
			cpp << "	{\n";
			cpp << "		column = 0;\n";
			cpp << "		line = 0;\n";
			cpp << "		if (YUNI_LIKELY(pData))\n";
			cpp << "		{\n";
			cpp << "			Datasource& ctx = *((Datasource*) pData);\n";
			cpp << "			ctx.translateOffset(column, line, node.offset);\n";
			cpp << "		}\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	void Parser::translateOffset(uint& column, uint& line, uint32_t offset) const\n";
			cpp << "	{\n";
			cpp << "		column = 0;\n";
			cpp << "		line = 0;\n";
			cpp << "		if (YUNI_LIKELY(pData))\n";
			cpp << "		{\n";
			cpp << "			Datasource& ctx = *((Datasource*) pData);\n";
			cpp << "			ctx.translateOffset(column, line, offset);\n";
			cpp << "		}\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	uint32_t Parser::translateOffsetToLine(const Node& node) const\n";
			cpp << "	{\n";
			cpp << "		uint32_t column;\n";
			cpp << "		uint32_t line;\n";
			cpp << "		translateOffset(column, line, node);\n";
			cpp << "		return line;\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << '\n';
			cpp << "	void Node::ExportToHTML(Clob& out, const Node& node)\n";
			cpp << "	{\n";
			cpp << "		String tmp;\n";
			cpp << "		String indent;\n";
			cpp << "		InternalNodeExportHTML(out, node, indent, tmp);\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	void Node::Export(Clob& out, const Node& node, bool color, ExportCallback callback)\n";
			cpp << "	{\n";
			cpp << "		String tmp;\n";
			cpp << "		String indent;\n";
			cpp << "		if (not color)\n";
			cpp << "			InternalNodeExportConsole<false>(out, node, false, indent, tmp, callback);\n";
			cpp << "		else\n";
			cpp << "			InternalNodeExportConsole<true>(out, node, false, indent, tmp, callback);\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	void Node::ExportToJSON(Yuni::Clob& out, const Node& node, void (*callback)(Yuni::Dictionary<AnyString, YString>::Unordered&, const Node&))\n";
			cpp << "	{\n";
			cpp << "		String tmp;\n";
			cpp << "		String indent;\n";
			cpp << "		out << \"{ \\\"data\\\": [\\n\";\n";
			cpp << "		InternalNodeExportJSON(out, node, false, indent, tmp, callback);\n";
			cpp << "		out << \"\t{}\\n\";\n";
			cpp << "		out << \"] }\\n\";\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	void Node::ExportToJSON(Yuni::Clob& out, const Node& node)\n";
			cpp << "	{\n";
			cpp << "		ExportToJSON(out, node, nullptr);\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	void Node::Export(Clob& out, const Node& node)\n";
			cpp << "	{\n";
			cpp << "		Export(out, node, ::Yuni::System::Console::IsStdoutTTY());\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
		/*	cpp << "	Node::Node(const Node& rhs)\n";
			cpp << "		: inherited()\n";
			cpp << "		, rule(rhs.rule)\n";
			cpp << "		, offset(rhs.offset)\n";
			cpp << "		, offsetEnd(rhs.offsetEnd)\n";
			cpp << "		, text(rhs.text)\n";
			cpp << "	{\n";
			cpp << "		// cloning children\n";
			cpp << "		if (not rhs.children.empty())\n";
			cpp << "		{\n";
			cpp << "			for (uint32_t i = 0; i != (uint) rhs.children.size(); ++i)\n";
			cpp << "				children[i] = new Node(*rhs.children[i]);\n";
			cpp << "		}\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';*/
			cpp << "	Node& Node::operator = (const Node& rhs)\n";
			cpp << "	{\n";
			cpp << "		rule = rhs.rule;\n";
			cpp << "		offset = rhs.offset;\n";
			cpp << "		offsetEnd = rhs.offsetEnd;\n";
			cpp << "		text = rhs.text;\n";
			cpp << "		children = rhs.children;\n";
			cpp << "		return *this;\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	void Node::toText(YString& out) const\n";
			cpp << "	{\n";
			cpp << "		if (not text.empty())\n";
			cpp << "		{\n";
			cpp << "			if (not out.empty())\n";
			cpp << "				out += ' ';\n";
			cpp << "			out += text;\n";
			cpp << "			out.trimRight();\n";
			cpp << "		}\n";
			cpp << "		for (auto& child: children)\n";
			cpp << "			child.toText(out);\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
			cpp << "	void Node::clear()\n";
			cpp << "	{\n";
			cpp << "		children.clear();\n";
			cpp << "		text.clear();\n";
			cpp << "		offset = 0;\n";
			cpp << "		offsetEnd = 0;\n";
			cpp << "		rule = rgUnknown;\n";
			cpp << "	}\n";
			cpp << '\n';
			cpp << '\n';
/*			cpp << "	void Node::swap(Node& other)\n";
			cpp << "	{\n";
			cpp << "		std::swap(rule, other.rule);\n";
			cpp << "		std::swap(offset, other.offset);\n";
			cpp << "		std::swap(offsetEnd, other.offsetEnd);\n";
			cpp << "		std::swap(text, other.text);\n";
			cpp << "		std::swap(children, other.children);\n";
			cpp << "	}\n";*/
			cpp << '\n';
			cpp << '\n';
		}


		inline void CPPConverter::startHeaderheaderGuardID()
		{
			headerGuardID = "__HEADER";

			for (uint32_t i = 0; i != namespaces.size(); ++i)
				headerGuardID << '_' << namespaces[i];
			headerGuardID << "_GRAMMAR";
			headerGuardID.toUpper();
		}


		inline void CPPConverter::endHeaderheaderGuardID()
		{
			h << "\n\n\n\n\n";
			hxx << "\n\n\n\n\n";
			cpp << "\n\n\n\n\n";

			if (not namespaces.empty())
			{
				uint32_t i = (uint) namespaces.size();
				do
				{
					--i;
					h << "} // namespace " << namespaces[i] << '\n';
					hxx << "} // namespace " << namespaces[i] << '\n';
					cpp << "} // namespace " << namespaces[i] << '\n';
				}
				while (i > 0);
			}

			h << '\n' << "#include \"" << localInclude << "hxx\"\n";
		}


		inline bool CPPConverter::writeFiles() const
		{
			AnyString root = rootfilename;
			YString file;
			file.clear() << root << "h";
			if (not IO::File::SetContent(file, h))
				return false;

			file.clear() << root << "hxx";
			if (not IO::File::SetContent(file, hxx))
				return false;

			file.clear() << root << "cpp";
			if (not IO::File::SetContent(file, cpp))
				return false;

			return true;
		}


	} // anonymous namespace






	bool Grammar::exportToCPP(const AnyString& rootfilename, const AnyString& name) const
	{
		CPPConverter data(rootfilename, pRules);
		if (YUNI_UNLIKELY(not data.initialize(name)))
			return false;

		data.startHeaderheaderGuardID();
		data.generateH();
		data.generateHXX();
		data.generateCPP();
		data.endHeaderheaderGuardID();
		return data.writeFiles();
	}





} // namespace PEG
} // namespace Parser
} // namespace Yuni

