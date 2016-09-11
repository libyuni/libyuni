#pragma once




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
		false, // expr
		false, // expr-atom
		false, // expr-group
		false, // expr-product
		false, // expr-sum
		false, // number
		false, // start
		false, // wp
	};


	inline bool shouldIgnoreForDuplication(enum Rule rule)
	{
		//assert(static_cast<uint32_t>(rule) < 9);
		return isToken[static_cast<uint32_t>(rule)];
	}


	static constexpr const bool isAttributeError[] =
	{
		false, // rgUnknown
		false, // rgExpr
		false, // rgExprAtom
		false, // rgExprGroup
		false, // rgExprProduct
		false, // rgExprSum
		false, // rgNumber
		false, // rgStart
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









} // namespace Calculator
} // namespace Demo
