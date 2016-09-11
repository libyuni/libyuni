#pragma once




inline std::ostream& operator << (std::ostream& out, const ::Demo::JSON::Node& node)
{
	::Yuni::Clob content;
	::Demo::JSON::Node::Export(content, node);out << content;
	return out;
}



inline std::ostream& operator << (std::ostream& out, const ::Demo::JSON::Node::Ptr node)
{
	if (!(!node))
		out << *node;
	else
		out << "<invalid ast node>";
	return out;
}



namespace Demo
{
namespace JSON
{




	inline NodeVector::T& NodeVector::back()
	{
		assert(m_size != 0);
		return *(m_pointer[m_size - 1]);
	}

	inline const NodeVector::T& NodeVector::back() const
	{
		assert(m_size != 0);
		return *(m_pointer[m_size - 1]);
	}

	inline NodeVector::T& NodeVector::front()
	{
		assert(m_size != 0);
		return *(m_pointer[0]);
	}

	inline const NodeVector::T& NodeVector::front() const
	{
		assert(m_size != 0);
		return *(m_pointer[0]);
	}

	inline bool NodeVector::empty() const noexcept
	{
		return (0 == m_size);
	}

	inline uint32_t NodeVector::size() const noexcept
	{
		return m_size;
	}

	inline uint32_t NodeVector::capacity() const noexcept
	{
		return m_capacity;
	}

	inline NodeVector::T& NodeVector::operator [] (uint32_t i) noexcept
	{
		assert(i < m_size);
		return *(m_pointer[i]);
	}

	inline const NodeVector::T& NodeVector::operator [] (uint32_t i) const noexcept
	{
		assert(i < m_size);
		return *(m_pointer[i]);
	}

	inline void NodeVector::erase(const Iterator& it) noexcept
	{
		erase(it.m_index);
	}

	inline void NodeVector::erase(const ConstIterator& it) noexcept
	{
		erase(it.m_index);
	}

	template<class U>
	inline void NodeVector::push_back(U& element)
	{
		push_back(T::Ptr::WeakPointer(element));
	}

	template<class U>
	inline void NodeVector::push_front(U& element)
	{
		push_front(T::Ptr::WeakPointer(element));
	}

	inline NodeVector::Iterator NodeVector::begin() noexcept
	{
		return Iterator(m_pointer, 0);
	}

	inline NodeVector::Iterator NodeVector::end() noexcept
	{
		return Iterator(m_pointer, m_size);
	}

	inline NodeVector::ConstIterator NodeVector::begin() const noexcept
	{
		return ConstIterator(m_pointer, 0);
	}

	inline NodeVector::ConstIterator NodeVector::end() const noexcept
	{
		return ConstIterator(m_pointer, m_size);
	}

	inline NodeVector::ConstIterator NodeVector::cbegin() const noexcept
	{
		return ConstIterator(m_pointer, 0);
	}

	inline NodeVector::ConstIterator NodeVector::cend() const noexcept
	{
		return ConstIterator(m_pointer, m_size);
	}

	inline void NodeVector::deleteElement(T* const element) noexcept
	{
		if (element->release())
			delete element;
	}

	inline NodeVector& NodeVector::operator = (const NodeVector& rhs)
	{
		clear();
		for (uint32_t i = 0; i != rhs.m_size; ++i)
			push_back(rhs.m_pointer[i]);
		return *this;	}




	//! References for tokens
	static constexpr const bool isToken[] =
	{
		false, // rgUnknown
		false, // array
		false, // char-extended
		false, // digit
		false, // error
		false, // error-bracket
		false, // error-eol
		false, // error-parenthese
		false, // error-semicolon
		false, // false
		false, // hex
		false, // integer
		false, // key
		false, // null
		false, // number
		false, // number-def
		false, // number-exponent
		false, // number-hexa
		false, // number-qualifier
		false, // number-qualifier-type
		false, // number-sign
		false, // number-value
		false, // object
		false, // object-entry
		false, // punc
		false, // single-char
		false, // sp
		false, // start
		false, // string
		false, // string-literal
		true, // tk-brace-close
		true, // tk-brace-open
		true, // tk-bracket-close
		true, // tk-bracket-open
		true, // tk-colon
		true, // tk-comma
		true, // tk-comment-block
		true, // tk-comment-block-content
		true, // tk-comment-end-block
		true, // tk-comment-line
		true, // tk-comment-line-content
		true, // tk-comment-start-block
		true, // tk-comment-start-inline
		true, // tk-comment-subbk
		true, // tk-dot
		true, // tk-double-quote
		false, // true
		false, // value
		false, // wp
	};


	inline bool shouldIgnoreForDuplication(enum Rule rule)
	{
		//assert(static_cast<uint32_t>(rule) < 49);
		return isToken[static_cast<uint32_t>(rule)];
	}


	static constexpr const bool isAttributeError[] =
	{
		false, // rgUnknown
		false, // rgArray
		false, // rgCharExtended
		false, // rgDigit
		true, // rgError
		true, // rgErrorBracket
		true, // rgErrorEol
		true, // rgErrorParenthese
		true, // rgErrorSemicolon
		false, // rgFalse
		false, // rgHex
		false, // rgInteger
		false, // rgKey
		false, // rgNull
		false, // rgNumber
		false, // rgNumberDef
		false, // rgNumberExponent
		false, // rgNumberHexa
		false, // rgNumberQualifier
		false, // rgNumberQualifierType
		false, // rgNumberSign
		false, // rgNumberValue
		false, // rgObject
		false, // rgObjectEntry
		false, // rgPunc
		false, // rgSingleChar
		false, // rgSp
		false, // rgStart
		false, // rgString
		false, // rgStringLiteral
		false, // rgTkBraceClose
		false, // rgTkBraceOpen
		false, // rgTkBracketClose
		false, // rgTkBracketOpen
		false, // rgTkColon
		false, // rgTkComma
		false, // rgTkCommentBlock
		false, // rgTkCommentBlockContent
		false, // rgTkCommentEndBlock
		false, // rgTkCommentLine
		false, // rgTkCommentLineContent
		false, // rgTkCommentStartBlock
		false, // rgTkCommentStartInline
		false, // rgTkCommentSubbk
		false, // rgTkDot
		false, // rgTkDoubleQuote
		false, // rgTrue
		false, // rgValue
		false, // rgWp
		false, // rgEOF
	};


	inline bool ruleIsError(enum Rule ruleid)
	{
		// assert((uint) ruleid < (uint) ruleCount);
		return isAttributeError[static_cast<uint32_t>(ruleid)];
	}




	inline Node::Node(enum Rule rule)
		: rule(rule)
	{}


	inline Node::Node(enum Rule rule, const AnyString& text)
		: rule(rule)
		, text(text)
	{}


	template<class F> inline bool Node::each(const F& callback)
	{
		for (uint32_t i = 0; i != children.size(); ++i)
		{
			if (not callback(children[i]))
				return false;
		}
		return true;
	}


	template<class F> inline bool Node::each(const F& callback) const
	{
		for (uint32_t i = 0; i != children.size(); ++i)
		{
			if (not callback(children[i]))
				return false;
		}
		return true;
	}


	template<class F> inline bool Node::each(enum Rule rule, const F& callback)
	{
		for (uint32_t i = 0; i != children.size(); ++i)
		{
			Node& subnode = children[i];
			if (subnode.rule == rule and not callback(subnode))
				return false;
		}
		return true;
	}


	template<class F> inline bool Node::each(enum Rule rule, const F& callback) const
	{
		for (uint32_t i = 0; i != children.size(); ++i)
		{
			const Node& subnode = children[i];
			if (subnode.rule == rule and not callback(subnode))
				return false;
		}
		return true;
	}


	template<class StringT> inline bool Node::extractFirstChildText(StringT& out, enum Rule rule) const
	{
		for (uint32_t i = 0; i != children.size(); ++i)
		{
			const Node& subnode = children[i];
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
			for (uint32_t i = 0; i != children.size(); ++i)
			{
				const Node& subnode = children[i];
				if (subnode.rule == rule)
				{
					out += subnode.text;
					somethingFound = true;
				}
			}
		}
		else
		{
			for (uint32_t i = 0; i != children.size(); ++i)
			{
				const Node& subnode = children[i];
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


	inline uint32_t Node::findFirst(enum Rule rule) const
	{
		for (uint32_t i = 0; i != children.size(); ++i)
		{
			if (children[i].rule == rule)
				return i;
		}
		return (uint)-1;
	}


	inline bool Node::empty() const
	{
		return children.empty();
	}


	inline bool Node::exists(enum Rule rule) const
	{
		for (uint32_t i = 0; i != children.size(); ++i)
		{
			if (children[i].rule == rule)
				return true;
		}
		return false;
	}


	inline const Node& Node::firstChild() const
	{
		return children.front();
	}


	inline Node& Node::firstChild()
	{
		return children.front();
	}


	inline const Node& Node::lastChild() const
	{
		return children.back();
	}


	inline Node& Node::lastChild()
	{
		return children.back();
	}


	inline const Node* Node::xpath(std::initializer_list<enum Rule> path) const
	{
		if (path.size() > 0)
		{
			auto it = path.begin();
			for (auto& child: children)
			{
				if (child.rule == *it)
				{
					const Node* result = &child;
					++it;
					for (; it != path.end(); ++it)
					{
						bool found = false;
						auto& subnode = *result;
						for (uint32_t j = 0; j != subnode.children.size(); ++j)
						{
							if (subnode.children[j].rule == *it)
							{
								result = &(subnode.children[j]);
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


	inline Node& Node::append(Rule r1)
	{
		Node* node = new Node{r1};
		children.push_back(node);
		return *node;
	}

	inline Node& Node::append(Rule r1, Rule r2)
	{
		return append(r1).append(r2);
	}

	inline Node& Node::append(Rule r1, Rule r2, Rule r3)	{
		return append(r1).append(r2).append(r3);
	}









} // namespace JSON
} // namespace Demo
