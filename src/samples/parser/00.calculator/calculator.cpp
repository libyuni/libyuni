
#include "calculator.h"
#include <cassert>
#include <yuni/core/string.h>
#include <yuni/io/file.h>
#include <yuni/io/directory.h>
#include <yuni/core/dictionary.h>
#include <yuni/core/noncopyable.h>
#include <yuni/datetime/timestamp.h>
#include <yuni/core/system/console/console.h>

using namespace Yuni;


namespace Demo
{
namespace Calculator
{
namespace // anonymous
{


	static inline bool  RuleAttributeCapture(enum Rule ruleid)
	{
		static const bool attr[] = {
			false, // rgUnknown
			true, // rgExpr
			false, // rgExprAtom
			false, // rgExprGroup
			false, // rgExprProduct
			false, // rgExprSum
			true, // rgNumber
			true, // rgStart
			false, // rgWp
			false, // rgEOF
		};
		assert((uint) ruleid < (uint) ruleCount);
		return attr[(uint) ruleid];
	}



	static inline bool  RuleAttributeImportant(enum Rule ruleid)
	{
		static const bool attr[] = {
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
		assert((uint) ruleid < (uint) ruleCount);
		return attr[(uint) ruleid];
	}



	static inline AnyString  RuleAttributeSimpleTextCapture(enum Rule ruleid)
	{
		static const AnyString attr[] = {
			nullptr, // rgUnknown
			nullptr, // rgExpr
			nullptr, // rgExprAtom
			nullptr, // rgExprGroup
			nullptr, // rgExprProduct
			nullptr, // rgExprSum
			nullptr, // rgNumber
			nullptr, // rgStart
			nullptr, // rgWp
			nullptr, // rgEOF
		};
		assert((uint) ruleid < (uint) ruleCount);
		return attr[(uint) ruleid];
	}




	//! Add traces to the stdcout
	# define HAS_TRACES 0

	# define HAS_STATISTICS 0

	//! Size (in bytes), when increasing the stack capacity
	# define GROW_CHUNK  512 // 512 * sizeof(Chunk) -> 8KiB

	//! Arbitrary value for consistency checks
	# define ARBITRARY_HARD_LIMIT  (1024 * 1024 * 500)


	# ifndef NDEBUG
	# define TRACE_INFO(X)  std::cout << "  == parser == " << X << std::endl
	# else
	# define TRACE_INFO(X)  {}
	# endif


	# if HAS_TRACES != 0

	# define TRACE(X) \
		do { \
			assert(ctx.offset < ctx.capacity); /* valid only because not called from grow */ \
			assert(ctx.stack[ctx.offset].urlindex < (uint) ctx.urls.size()); \
			const String& filename = ctx.urls[ctx.stack[ctx.offset].urlindex]; \
			assert(filename.size() < 65535); \
			assert(filename.capacity() < 65535); \
			assert(filename.data() != NULL); \
			\
			std::cout << "  == parser == stack == " << filename \
				<< ", depth:" << ctx.offset << ": " << X << std::endl; \
		} while (false)


	# define TRACE_LOCAL(X) \
		do { \
			assert(stack[offset].urlindex < (uint) urls.size()); \
			const String& filename = urls[stack[offset].urlindex]; \
			assert(filename.size() < 65535); \
			assert(filename.capacity() < 65535); \
			assert(filename.data() != NULL); \
			\
			std::cout << "  == parser == stack == " << filename \
				<< ", depth:" << offset << ": " << X << std::endl; \
		} while (false)

	# else

	# define TRACE(X)  {}

	# define TRACE_LOCAL(X)  {}

	# endif






	# if HAS_STATISTICS != 0 && !defined(NDEBUG)

	# define DATASOURCE_PARSE(ctx) \
		do { \
			sint64 start = ::Yuni::DateTime::NowMilliSeconds(); \
			ctx.clear(); \
			ctx.success = yyrgStart(ctx) and ctx.isParseComplete(); \
			ctx.duration = (uint64) (::Yuni::DateTime::NowMilliSeconds() - start); \
			ctx.buildAST(); \
			root = ctx.rootnode; \
		} \
		while (false)

	# else

	# define DATASOURCE_PARSE(ctx) \
		do { \
			ctx.clear(); \
			ctx.success = yyrgStart(ctx) and ctx.isParseComplete(); \
			ctx.buildAST(); \
			root = ctx.rootnode; \
		} \
		while (false)

	# endif // HAS_STATISTICS









	struct Chunk
	{
		//! Iterator on the source
		uint offset;
		//! Index of the current source url (see `urls`)
		uint urlindex;
		//! Rule ID - a negative value means that the rule has not been commited yet
		int rule;
		//! End offset - means nothing if rule == 0
		uint offsetEnd;
		//! Parent rule
		uint parent;
	};




	class Datasource final
	{
	public:

	public:
		//! \name Constructors
		//@{
		//! Default constructor
		Datasource(Notification::Vector& notifications);
		~Datasource();
		//@}

		//! \name Matching
		//@{
		bool matchSingleAsciiChar(char);
		bool matchString(const AnyString& text);
		bool matchOneOf(const AnyString& text);

		bool notMatchSingleAsciiChar(char);
		bool notMatchOneOf(const AnyString& text);
		//@}

		//! \name Chunk
		//@{
		void pushInclude(uint urlindex);
		uint push();
		uint enterRule(enum Rule rule);
		void restart(uint from);
		void commit(uint ruleOffset, enum Rule rule);
		//@}

		//! \name Filename manipulation
		//@{
		//! Open a new url
		bool open(const AnyString& newurl);
		//! Open from anonymous origin
		void openContent(const AnyString& content);
		//! Close the current url
		void close();
		//@}

		//! \name AST Builder
		//@{
		//! Clear internal variables
		void clear();
		//! Build the whole AST from the stack informations
		void buildAST();

		//! Get if the parse has been successful or not
		bool isParseComplete() const;
		//@}


		//! \name Notifications
		//@{
		//! Create a new notification
		void notify(const AnyString& message) const;
		//@}


		//! \name Address translation
		//@{
		void translateOffset(uint& column, uint& line, uint offset) const;
		//@}


	public:
		//! Stack
		Chunk* stack;
		uint offset;
		# if HAS_STATISTICS != 0 && !defined(NDEBUG)
		uint maxOffset;
		uint64 duration;
		# endif
		uint capacity;

		//! Root folder
		YString root;
		//! File Content of all encountered urls, ordered by their order of arrival
		Clob::Vector contents;
		//! Reverse mapping for retriving the index of a known file
		::Yuni::Dictionary<String, uint>::Unordered  reverseUrlIndexes;
		//! All urls
		::Yuni::String::Vector urls;

		//! Success flag
		bool success;
		//! Root node
		Node::Ptr rootnode;

		//! Notifications
		Notification::Vector& notifications;

	private:
		void grow();
		void buildASTForNonEmptyContent();
		void findOptimalNewOffsetAfterCommit(uint ruleOffset);

	}; // class Datasource



	class OffsetAutoReset final
	{
	public:
		OffsetAutoReset(Datasource& ctx) :
			ctx(ctx),
			oldOffset(ctx.offset),
			oldFileOffset(ctx.stack[ctx.offset].offset)
		{}

		~OffsetAutoReset()
		{
			ctx.offset = oldOffset;
			ctx.stack[oldOffset].offset = oldFileOffset;
		}

	private:
		Datasource& ctx;
		uint oldOffset;
		uint oldFileOffset;
	};




	# ifndef NDEBUG
	static inline std::ostream& PrintFrame(std::ostream& out, const Chunk& cursor)
	{
		out << "{offset: " << cursor.offset
			<< ", url: " << cursor.urlindex
			<< ", rule: " << cursor.rule
			<< ", end: " << cursor.offsetEnd
			<< ", parent: " << cursor.parent << "}";
		return out;
	}
	# endif




	inline Datasource::Datasource(Notification::Vector& notifications) :
		stack(),
		offset(),
		capacity(GROW_CHUNK),
		notifications(notifications)
	{
		stack = (Chunk*)::malloc(sizeof(Chunk) * GROW_CHUNK);
	}


	inline Datasource::~Datasource()
	{
		// rootnode = nullptr
		::free(stack);
	}


	inline void Datasource::clear()
	{
		offset = 0;
		success = false;

		# if HAS_STATISTICS != 0 && !defined(NDEBUG)
		maxOffset = 0;
		duration  = 0;
		# endif

		rootnode = nullptr;

		// avoid too much memory consumption
		if (capacity > GROW_CHUNK * 1024)
		{
			capacity = GROW_CHUNK * 1024;
			stack = (Chunk*)::realloc(stack, sizeof(Chunk) * capacity);
		}

		// initializing the first frame
		Chunk& firstFrame = stack[0];
		// no offset
		firstFrame.offset = 0;
		// the first one will be good enough
		firstFrame.urlindex = 0;
		// to make the frame 0 the root parent frame (and to avoid useless checks)
		firstFrame.rule = - (int) rgEOF;
		// no end
		firstFrame.offsetEnd = 0;
		// no parent
		firstFrame.parent = 0;
	}


	inline void Datasource::grow()
	{
		// WARNING The variable 'offset' might be unreliable for displying
		// and/or checking values from the stack frames
		assert(capacity + GROW_CHUNK < ARBITRARY_HARD_LIMIT); // arbitrary

		// grow the stack
		stack = (Chunk*)::realloc(stack, (capacity += GROW_CHUNK) * sizeof(Chunk));
		// post-checks
		assert(offset < capacity);
	}


	inline void Datasource::pushInclude(uint urlindex)
	{
		assert(urlindex < ARBITRARY_HARD_LIMIT);
		assert(capacity + GROW_CHUNK < ARBITRARY_HARD_LIMIT); // arbitrary
		assert(offset < ARBITRARY_HARD_LIMIT); // arbitrary
		assert(urls.size() == contents.size());
		assert(urls.size() == reverseUrlIndexes.size());

		if (YUNI_UNLIKELY(not (++offset < capacity))) // grow
			grow();

		Chunk& cursor    = stack[offset];
		cursor.offset    = 0;
		cursor.urlindex  = urlindex;
		cursor.rule      = 0;
		cursor.offsetEnd = 0;
		cursor.parent    = (uint) -1;
	}


	inline uint Datasource::push()
	{
		assert(capacity + GROW_CHUNK < ARBITRARY_HARD_LIMIT); // arbitrary
		assert(offset < ARBITRARY_HARD_LIMIT); // arbitrary

		if (YUNI_UNLIKELY(not (++offset < capacity))) // grow
			grow();

		assert(offset < capacity);
		# if HAS_STATISTICS != 0 && !defined(NDEBUG)
		if (offset > maxOffset)
			maxOffset = offset;
		# endif

		Chunk& cursor      = stack[offset];
		const Chunk& prev  = stack[offset - 1];

		assert(prev.offset < ARBITRARY_HARD_LIMIT);
		assert(prev.urlindex < ARBITRARY_HARD_LIMIT);

		cursor.offset    = prev.offset;
		cursor.urlindex  = prev.urlindex;
		cursor.rule      = 0;
		// cursor.offsetEnd = 0;  means nothing if rule == 0

		// Since TRACE_LOCAL uses cursor.offset, this macro must be called after
		// the previous initialization
		TRACE_LOCAL("    push at offset " << prev.offset);
		return offset - 1;
	}


	inline uint Datasource::enterRule(enum Rule rule)
	{
		assert(capacity + GROW_CHUNK < ARBITRARY_HARD_LIMIT); // arbitrary
		assert(offset < ARBITRARY_HARD_LIMIT); // arbitrary
		assert((uint) rule < (uint) ruleCount);

		// This method is quite similar to `push`
		// Grow !
		if (YUNI_UNLIKELY(not (++offset < capacity)))
			grow();

		# if HAS_STATISTICS != 0 && !defined(NDEBUG)
		if (offset > maxOffset)
			maxOffset = offset;
		# endif

		assert(offset > 0);
		assert(capacity < ARBITRARY_HARD_LIMIT);
		Chunk& cursor      = stack[offset];
		const Chunk& prev  = stack[offset - 1];

		cursor.offset    = prev.offset;
		cursor.urlindex  = prev.urlindex;
		cursor.rule      = - ((int) rule);
		cursor.offsetEnd = cursor.offset; // store offset for reuse at commit

		TRACE_LOCAL("    enter at offset " << cursor.offset);
		return offset;
	}


	inline void Datasource::findOptimalNewOffsetAfterCommit(uint ruleOffset)
	{
		// trying to reduce the stack size by removing all stack frames which are
		// not marked as a real rule this optimization can reduce by 3 the size of
		// the stack and reduce by 4 the time required to parse some big input

		// if a stack frame with the variable 'rule' not null (aka a stack frame
		// dedicated to create a node in the AST), then it would not be safe to
		// get rid of the end of the stack
		for (uint i = ruleOffset + 1; i <= offset; ++i)
		{
			if (stack[i].rule != 0)
			{
				++offset;
				return;
			}
		}

		// it seems that it is safe to ignore the end of the stack - hourray \o/
		offset = ruleOffset + 1;
	}


	inline void Datasource::commit(uint ruleOffset, enum Rule rule)
	{
		assert(stack[ruleOffset].rule == - (int) rule and "inconsistency ruleid in the stack");
		assert(offset >= ruleOffset and "invalid stack entre/commit");
		assert(stack[offset].offset >= stack[ruleOffset].offset and "Huh? Should go forward...");
		(void) rule; // avoid warning in release mode

		// committing the current stack frame
		{
			Chunk& ruleCursor = stack[ruleOffset];

			// restore previous offset - a temporary variable must be used
			// it may happen that `offset` and `ruleOffset` are the same
			uint newEndOffset    = stack[offset].offset;
			ruleCursor.offset    = ruleCursor.offsetEnd;
			ruleCursor.offsetEnd = newEndOffset;
			ruleCursor.rule      = (int) rule;

			assert(ruleCursor.offset <= ruleCursor.offsetEnd and "invalid boundaries");

			// Looking for the parent node
			{
				# ifndef NDEBUG
				bool parentFound = false;
				# endif

				uint i = ruleOffset;
				do
				{
					if (stack[--i].rule < 0)
					{
						ruleCursor.parent = i;
						# ifndef NDEBUG
						parentFound = true;
						# endif
						break;
					}
				}
				while (i != 0);

				# ifndef NDEBUG
				if (not parentFound)
				{
					std::cerr << "invalid parent commiting " << ruleCursor.rule << " at offset " << ruleOffset << std::endl;
					assert(false);
				}
				# endif
			}

			# if HAS_TRACES != 0
			uint csize = ruleCursor.offsetEnd - ruleCursor.offset;
			AnyString content(contents[ruleCursor.urlindex], ruleCursor.offset, csize);
			TRACE_LOCAL("    COMMIT " << RuleToString(rule) << ", offset " << newEndOffset << ": " << content);
			# endif
		}

		// new stack frame - push - to keep this element in the stack
		// calculating the new offset: [optimization]
		findOptimalNewOffsetAfterCommit(ruleOffset);

		if (YUNI_UNLIKELY(not (offset < capacity))) // grow !
			grow();

		# if HAS_STATISTICS != 0 && !defined(NDEBUG)
		if (offset > maxOffset)
			maxOffset = offset;
		# endif

		Chunk& ruleCursor = stack[ruleOffset];
		Chunk& cursor     = stack[offset];
		cursor.offset     = ruleCursor.offsetEnd;
		cursor.urlindex   = ruleCursor.urlindex;
		cursor.rule       = 0;
	}


	inline void Datasource::restart(uint from)
	{
		// the method `push` returns `offset - 1`
		assert(from + 1 < capacity);
		assert(from < offset and "can not restart from a non existing frame");

		offset = from + 1;

		Chunk& cursor      = stack[offset];
		const Chunk& prev  = stack[offset - 1];

		cursor.offset    = prev.offset;
		cursor.urlindex  = prev.urlindex;
		cursor.rule      = 0;
		TRACE_LOCAL("    restart at offset " << cursor.offset);
	}


	inline bool Datasource::open(const AnyString& newurl)
	{
		// getting the root directory once and for all if not already done
		// the operation is not done in the constructor to let the caller
		// initialize this variable if needed
		if (root.empty())
			::Yuni::IO::Directory::Current::Get(root, false);

		// canonicalizing the filename
		String filename;
		::Yuni::IO::Canonicalize(filename, newurl, root);

		// filename index
		uint index;
		Yuni::Dictionary<String, uint>::Unordered::const_iterator knownIndex = reverseUrlIndexes.find(filename);
		if (YUNI_LIKELY(knownIndex == reverseUrlIndexes.end()))
		{
			assert(contents.size() == urls.size());
			// indexes
			index = (uint) contents.size();
			reverseUrlIndexes[filename] = index;
			// load the entire content in memory
			contents.push_back(nullptr);
			urls.push_back(filename);
			if (::Yuni::IO::errNone != ::Yuni::IO::File::LoadFromFile(contents.back(), filename))
				return false;
		}
		else
			index = knownIndex->second;

		// new item in the stack
		pushInclude(index);
		return true;
	}


	inline void Datasource::openContent(const AnyString& content)
	{
		const String filename = "<memory>";
		// filename index
		uint index;
		::Yuni::Dictionary<String, uint>::Unordered::const_iterator knownIndex = reverseUrlIndexes.find(filename);
		if (YUNI_LIKELY(knownIndex == reverseUrlIndexes.end()))
		{
			assert(contents.size() == urls.size());
			// indexes
			index = (uint) contents.size();
			reverseUrlIndexes[filename] = index;
			urls.push_back(filename);
			// load the entire content in memory
			contents.push_back(nullptr);
			contents.back() = content;
		}
		else
			index = knownIndex->second;

		pushInclude(index);
	}


	inline bool Datasource::matchSingleAsciiChar(char c)
	{
		Chunk& cursor    = stack[offset];
		const Clob& data = contents[cursor.urlindex];
		if (cursor.offset < data.size() and c == data[cursor.offset])
		{
			++cursor.offset;
			return true;
		}
		return false;
	}


	inline bool Datasource::matchString(const AnyString& text)
	{
		Chunk& cursor    = stack[offset];
		const Clob& data = contents[cursor.urlindex];

		if (AnyString(data, cursor.offset).startsWith(text))
		{
			cursor.offset += text.size();
			return true;
		}
		return false;
	}


	inline bool Datasource::matchOneOf(const AnyString& text)
	{
		Chunk& cursor    = stack[offset];
		const Clob& data = contents[cursor.urlindex];

		if (cursor.offset < data.size())
		{
			if (text.contains(data[cursor.offset]))
			{
				++cursor.offset;
				return true;
			}
		}
		return false;
	}


	inline bool Datasource::notMatchSingleAsciiChar(char c)
	{
		Chunk& cursor    = stack[offset];
		const Clob& data = contents[cursor.urlindex];
		if (cursor.offset < data.size() and c != data[cursor.offset])
		{
			++cursor.offset;
			return true;
		}
		return false;
	}


	inline bool Datasource::notMatchOneOf(const AnyString& text)
	{
		Chunk& cursor    = stack[offset];
		const Clob& data = contents[cursor.urlindex];

		if (cursor.offset < data.size())
		{
			if (not text.contains(data[cursor.offset]))
			{
				++cursor.offset;
				return true;
			}
		}
		return false;
	}



	static bool StandardURILoaderHandler(Clob& out, const AnyString& uri)
	{
		if (not uri.empty())
		{
			out.clear();
			if (::Yuni::IO::errNone == ::Yuni::IO::File::LoadFromFile(out, uri))
				return true;
		}
		return false;
	}




	static void InternalNodeExportHTML(Clob& out, const Node& node, String& indent, String& tmp)
	{
		assert(&node != NULL);
		out << indent << "<div class=\"node\">";
		out << "<span class=\"rule\">" << RuleToString(node.rule) << "</span>";

		bool attrCapture = RuleAttributeCapture(node.rule);
		if (attrCapture)
		{
			// out << " <span class=\"line\">l." << node.line << "</span> ";
			out << " <code>";
			tmp = node.text;
			tmp.replace("<", "&lt;");
			tmp.replace(">", "&gt;");
			out << tmp << "</code>";
		}

		if (not node.children.empty())
		{
			out << '\n' << indent << "<ul>\n";
			indent.append("    ", 4);
			for (uint i = 0; i != (uint) node.children.size(); ++i)
			{
				out << indent << "<li>\n";
				InternalNodeExportHTML(out, *(node.children[i]), indent, tmp);
				out << indent << "</li>\n";
			}

			indent.chop(4);
			out << indent << "</ul>\n" << indent;
		}
		out << "</div>\n";
	}


	template<bool ColorT>
	static void InternalNodeExportConsole(Clob& out, const Node& node, bool hasSibling, String& indent, String& tmp)
	{
		using namespace ::Yuni::System::Console;
		assert(&node != NULL);

		if (ColorT)
			::Yuni::System::Console::SetTextColor(out, blue);
		out << indent;

		if (not ColorT)
		{
			out << RuleToString(node.rule);
		}
		else
		{
			if (RuleAttributeImportant(node.rule))
			{
				::Yuni::System::Console::SetTextColor(out, purple);
				out << RuleToString(node.rule);
				::Yuni::System::Console::ResetTextColor(out);
			}
			else
			{
				::Yuni::System::Console::ResetTextColor(out);
				out << RuleToString(node.rule);
			}
		}

		bool attrCapture = RuleAttributeCapture(node.rule);
		if (attrCapture)
		{
			tmp = node.text;
			tmp.replace("\n", "\\n");
			out << ": ";

			if (ColorT)
				::Yuni::System::Console::SetTextColor(out, green);
			out << tmp;
			if (ColorT)
				::Yuni::System::Console::ResetTextColor(out);
		}
		else
		{
			// it can be interresting to print the text itself when the node
			// is a simple text capture
			AnyString textCapture = RuleAttributeSimpleTextCapture(node.rule);
			if (not textCapture.empty())
			{
				out << ", ";
				if (ColorT)
					::Yuni::System::Console::SetTextColor(out, green);
				out << textCapture;
				if (ColorT)
					::Yuni::System::Console::ResetTextColor(out);
			}
		}

		if (node.children.size() > 1)
		{
			if (ColorT)
				::Yuni::System::Console::SetTextColor(out, blue);
			out << " (+" << node.children.size() << ')';
		}

		out << '\n';

		// sub nodes
		if (not node.children.empty())
		{
			if (hasSibling)
				indent.append("|   ", 4);
			else
				indent.append("    ", 4);

			for (uint i = 0; i != (uint) node.children.size(); ++i)
			{
				bool hasSibling = (i != (uint) node.children.size() - 1);
				InternalNodeExportConsole<ColorT>(out, *(node.children[i]), hasSibling, indent, tmp);
			}

			indent.chop(4);
		}
	}


	void Datasource::buildAST()
	{
		if (success)
		{
			if (offset > 0) // not empty content
			{
				buildASTForNonEmptyContent();
			}
			else
			{
				// minor optimisation to avoid allocation on empty contents
				rootnode = new Node();
				rootnode->rule = rgUnknown;
				rootnode->offset = 0;
				rootnode->offsetEnd = 0;
			}
		}
		else
			rootnode = nullptr;

		# if HAS_STATISTICS != 0 && !defined(NDEBUG)
		{
			uint realRuleCount = 0;

			for (uint i = 0; i != offset; ++i)
			{
				if (stack[i].rule > 0)
					++realRuleCount;
			}

			TRACE_INFO("STATISTICS");
			TRACE_INFO("result: " << (success ? "OK" : "FAILED") << "  (" << duration << "ms)");
			uint ratio = (uint)((double) realRuleCount * 100. / offset);
			TRACE_INFO("stack: " << realRuleCount << " rules for " << offset << " stack frames, ratio: " << ratio << "% (higher is better), max depth: " << maxOffset);
			TRACE_INFO("stack: " << ((capacity * sizeof(Chunk)) / 1024) << " KiB (capacity: " << capacity << ", " << sizeof(Chunk) << " bytes per frame)");
			uint64 totalCapa = 0;
			for (uint i = 0; i != (uint) contents.size(); ++i)
				totalCapa += contents[i].capacity();
			TRACE_INFO("content from urls: " << contents.size() << ", total " << (totalCapa / 1024) << " KiB in memory");

			totalCapa += (capacity * sizeof(Chunk));
			for (uint i = 0; i != (uint) urls.size(); ++i)
				totalCapa += urls[i].capacity();
			totalCapa += root.capacity();
			TRACE_INFO("total memory usage: " << (totalCapa / 1024) << " KiB");
		}
		# endif

		// cleanup
		::free(stack);
		stack = nullptr;
		offset = 0;
		capacity = 0;
	}


	void Datasource::buildASTForNonEmptyContent()
	{
		// at this point, the parse was successful and something has been found
		assert(success);
		assert(offset > 0);
		assert(stack[0].rule == + (int) rgEOF and "invalid stack (should have called isParseComplete())");

		// all AST nodes, mapping the stack elements
		Node::Vector astNodes;
		astNodes.resize(offset + 1);

		// pseudo root node to avoid special cases for retrieving the parent node
		astNodes[0] = new Node();
		astNodes[0]->rule = rgUnknown;
		astNodes[0]->offset = 0;
		astNodes[0]->offsetEnd = 0;
		Node& pseudoRootNode = *astNodes[0];

		// starting from the first real frame
		for (uint i = 1; i != offset; ++i)
		{
			const Chunk& cursor = stack[i];

			# ifndef NDEBUG
			if (cursor.rule < 0)
			{
				std::cerr << "assert failed: invalid rule  ";
				PrintFrame(std::cerr, cursor) << std::endl;
				assert(cursor.rule >= 0 and "some rules are not commited - the parse should have failed");
			}
			# endif

			if (cursor.rule <= 0)
				continue;

			# ifndef NDEBUG
			if (not (cursor.parent < offset))
			{
				std::cerr << "assert failed: invalid parent: " << i << "/" << offset << ",  ";
				PrintFrame(std::cerr, cursor) << std::endl;
				assert(cursor.parent < offset and "invalid parent index");
			}
			assert(cursor.parent < astNodes.size());
			assert(!(!astNodes[cursor.parent]) and "invalid parent");
			# endif

			// The grammar rule for the current node
			enum Rule rule = (enum Rule) cursor.rule;

			assert(cursor.parent < astNodes.size());
			assert((uint64) i < astNodes.size());

			Node::Ptr newnode = new Node();
			astNodes[i] = newnode;
			astNodes[cursor.parent]->children.push_back(newnode);

			newnode->rule      = rule;
			newnode->offset    = cursor.offset;
			newnode->offsetEnd = cursor.offsetEnd;

			// Flag to determine whether the node has to capture the content or not
			bool attrCapture = RuleAttributeCapture(rule);
			if (attrCapture)
			{
				// Checking integrity of the captured content
				assert(
					cursor.offsetEnd >= cursor.offset                // valid end offset
					and cursor.offsetEnd != (uint) -1                // valid end offset
					and cursor.offset    != (uint) -1                // valid offset
					and cursor.urlindex  <  (uint)contents.size()    // valuid url index
					and cursor.offset    <  contents[cursor.urlindex].size()  // valid range
					and cursor.offsetEnd <= contents[cursor.urlindex].size() // valid range
					and "invalid offset for content capture");

				// size of the captured content
				uint size = cursor.offsetEnd - cursor.offset;
				// Captured content
				newnode->text.assign(contents[cursor.urlindex], size, cursor.offset);
			}
		}

		rootnode = (pseudoRootNode.children.size() == 1)
			? pseudoRootNode.children[0]
			: nullptr;
	}


	void Datasource::notify(const AnyString& message) const
	{
		Chunk& cursor    = stack[offset];
		const Clob& data = contents[cursor.urlindex];

		uint line = 1;
		uint x = 0;

		// trying to find the appropriate line index
		if (cursor.offset < data.size())
		{
			for (uint i = 0; i != cursor.offset; ++i)
			{
				if (data[i] == '\n')
				{
					++line;
					x = 0;
				}
				else
					++x;
			}
		}

		Notification* notification = new Notification();
		notification->offset     = ((0 != x) ? x : 1);
		notification->line       = line;
		notification->message    = message;
		notification->filename   = urls[cursor.urlindex];

		const_cast<Notification::Vector&>(notifications).push_back(notification);
	}


	inline bool Datasource::isParseComplete() const
	{
		if (offset > 1)
		{
			assert(offset < capacity);
			assert(stack[offset].urlindex < (uint) urls.size());
			assert(stack[0].rule == - (int) rgEOF and "invalid stack");

			// pseudo commit the root frame
			stack[0].rule = + (int) rgEOF;

			// trying to find the commit rule "start", which should be at index 1
			uint startoffset = 1;
			do
			{
				if (stack[startoffset].rule == rgStart)
				{
					Chunk& cursor = stack[startoffset];

					if (cursor.urlindex == 0) // the original input source
					{
						// the original content
						const Clob& data = contents[cursor.urlindex];

						if (cursor.offset < data.size() and cursor.offsetEnd <= data.size())
						{
							if (data.size() == cursor.offsetEnd)
								return true;
						}
					}

					break;
				}

				++startoffset;
			}
			while (startoffset <= offset);

			notify("failed to parse");
		}

		return false;
	}


	inline void Datasource::translateOffset(uint& column, uint& line, uint offset) const
	{
		uint fileindex = 0;

		if (fileindex < (uint) contents.size())
		{
			// alias to the content
			const Clob& content = contents[fileindex];
			// maximum size
			uint maxSize = std::max((uint) content.size(), offset);

			for (uint x = 0; x != maxSize; ++x)
			{
				if (content[x] == '\n')
				{
					column = 0;
					++line;
				}
				else
					++column;
			}
		}

		if (line != 0 and column == 0)
			column = 1;
	}




	// Forward declarations
	static inline bool yyrgExpr(Datasource& ctx);
	static inline bool yyrgExprAtom(Datasource& ctx);
	static inline bool yyrgExprGroup(Datasource& ctx);
	static inline bool yyrgExprProduct(Datasource& ctx);
	static inline bool yyrgExprSum(Datasource& ctx);
	static inline bool yyrgNumber(Datasource& ctx);
	static inline bool yyrgStart(Datasource& ctx);
	static inline bool yyrgWp(Datasource& ctx);




	//! Rule expr
	static inline bool yyrgExpr(Datasource& ctx)
	{
		TRACE("entering rgExpr");
		uint _ruleOffset = ctx.enterRule(rgExpr);

		ctx.push(); // 0 or 1
		if (not (yyrgExprSum(ctx))) // ?
			return false;

		ctx.commit(_ruleOffset, rgExpr);
		return true;
	}


	//! Rule expr-atom
	static inline bool yyrgExprAtom(Datasource& ctx)
	{
		TRACE("entering rgExprAtom");
		uint _ruleOffset = ctx.enterRule(rgExprAtom);

		uint sp2 = ctx.push();
		bool rt1 = false;
		// condition: first part
		do
		{
			uint sp3 = ctx.push(); // 0 or 1
			if (not (yyrgExprGroup(ctx))) // ?
			{
				assert(sp3 < ctx.offset and "invalid offset match min == 1");
				ctx.offset = sp3;
				break;
			}
			rt1 = true;
		}
		while (false);
		// condition: second part
		while (not rt1)
		{
			ctx.restart(sp2);
			uint sp4 = ctx.push(); // 0 or 1
			if (not (yyrgNumber(ctx))) // ?
			{
				assert(sp4 < ctx.offset and "invalid offset match min == 1");
				ctx.offset = sp4;
				break;
			}
			rt1 = true;
			break;
		}

		if (not rt1)
			return false;

		ctx.commit(_ruleOffset, rgExprAtom);
		return true;
	}


	//! Rule expr-group
	static inline bool yyrgExprGroup(Datasource& ctx)
	{
		TRACE("entering rgExprGroup");
		uint _ruleOffset = ctx.enterRule(rgExprGroup);

		if (not (ctx.matchSingleAsciiChar('('))) // ?
			return false;
		ctx.push(); // 0 or 1
		if (not (yyrgExpr(ctx))) // ?
			return false;
		if (not (ctx.matchSingleAsciiChar(')'))) // ?
			return false;

		ctx.commit(_ruleOffset, rgExprGroup);
		return true;
	}


	static const AnyString datatext8("*/%", 3);





	static inline bool __helper5(Datasource& ctx);
	static inline bool __helper6(Datasource& ctx);


	static inline bool __helper5(Datasource& ctx)
	{
		TRACE("    :: entering helper 5");
		if (not (ctx.matchOneOf(datatext8 /* * / % */))) // ?
			return false;
		return true;
	}

	static inline bool __helper6(Datasource& ctx)
	{
		TRACE("    :: entering helper 6");
		ctx.push(); // 0 or 1
		if (not (yyrgExprAtom(ctx))) // ?
			return false;
		return true;
	}

	//! Rule expr-product
	static inline bool yyrgExprProduct(Datasource& ctx)
	{
		TRACE("entering rgExprProduct");
		uint _ruleOffset = ctx.enterRule(rgExprProduct);

		ctx.push(); // 0 or 1
		if (not (yyrgExprAtom(ctx))) // ?
			return false;
		do // 0 or more
		{
			uint sp9 = ctx.push(); // 0 or 1
			if (not (__helper5(ctx) and __helper6(ctx)))
			{
				assert(sp9 < ctx.offset and "invalid offset match *");
				ctx.offset = sp9;
				break;
			}
		}
		while (true);

		ctx.commit(_ruleOffset, rgExprProduct);
		return true;
	}


	static const AnyString datatext13("+-", 2);





	static inline bool __helper10(Datasource& ctx);
	static inline bool __helper11(Datasource& ctx);


	static inline bool __helper10(Datasource& ctx)
	{
		TRACE("    :: entering helper 10");
		if (not (ctx.matchOneOf(datatext13 /* +- */))) // ?
			return false;
		return true;
	}

	static inline bool __helper11(Datasource& ctx)
	{
		TRACE("    :: entering helper 11");
		ctx.push(); // 0 or 1
		if (not (yyrgExprProduct(ctx))) // ?
			return false;
		return true;
	}

	//! Rule expr-sum
	static inline bool yyrgExprSum(Datasource& ctx)
	{
		TRACE("entering rgExprSum");
		uint _ruleOffset = ctx.enterRule(rgExprSum);

		ctx.push(); // 0 or 1
		if (not (yyrgExprProduct(ctx))) // ?
			return false;
		do // 0 or more
		{
			uint sp14 = ctx.push(); // 0 or 1
			if (not (__helper10(ctx) and __helper11(ctx)))
			{
				assert(sp14 < ctx.offset and "invalid offset match *");
				ctx.offset = sp14;
				break;
			}
		}
		while (true);

		ctx.commit(_ruleOffset, rgExprSum);
		return true;
	}


	static const AnyString datatext15("+-", 2);
	static const AnyString datatext16("0123456789", 10);
	static const AnyString datatext20("0123456789", 10);
	static const AnyString datatext26("eE", 2);
	static const AnyString datatext27("+-", 2);
	static const AnyString datatext28("0123456789", 10);





	static inline bool __helper17(Datasource& ctx);
	static inline bool __helper18(Datasource& ctx);
	static inline bool __helper22(Datasource& ctx);
	static inline bool __helper23(Datasource& ctx);
	static inline bool __helper24(Datasource& ctx);


	static inline bool __helper17(Datasource& ctx)
	{
		TRACE("    :: entering helper 17");
		if (not (ctx.matchSingleAsciiChar('.'))) // ?
			return false;
		return true;
	}

	static inline bool __helper18(Datasource& ctx)
	{
		TRACE("    :: entering helper 18");
		if (not (ctx.matchOneOf(datatext20 /* 0123456789 */))) // ?
			return false;
		while (ctx.matchOneOf(datatext20 /* 0123456789 */)) // 0-1 or more
		{}
		return true;
	}

	static inline bool __helper22(Datasource& ctx)
	{
		TRACE("    :: entering helper 22");
		if (not (ctx.matchOneOf(datatext26 /* eE */))) // ?
			return false;
		return true;
	}

	static inline bool __helper23(Datasource& ctx)
	{
		TRACE("    :: entering helper 23");
		(ctx.matchOneOf(datatext27 /* +- */)); // ? - ignore the result if it does not match
		return true;
	}

	static inline bool __helper24(Datasource& ctx)
	{
		TRACE("    :: entering helper 24");
		if (not (ctx.matchOneOf(datatext28 /* 0123456789 */))) // ?
			return false;
		while (ctx.matchOneOf(datatext28 /* 0123456789 */)) // 0-1 or more
		{}
		return true;
	}

	//! Rule number
	static inline bool yyrgNumber(Datasource& ctx)
	{
		TRACE("entering rgNumber");
		uint _ruleOffset = ctx.enterRule(rgNumber);

		(ctx.matchOneOf(datatext15 /* +- */)); // ? - ignore the result if it does not match
		if (not (ctx.matchOneOf(datatext16 /* 0123456789 */))) // ?
			return false;
		while (ctx.matchOneOf(datatext16 /* 0123456789 */)) // 0-1 or more
		{}
		uint sp21 = ctx.push(); // 0 or 1
		if (not (__helper17(ctx) and __helper18(ctx))) // ?
		{
			assert(sp21 < ctx.offset and "invalid offset match ?");
			ctx.offset = sp21; // ignore the results if it does not match
		}
		uint sp29 = ctx.push(); // 0 or 1
		if (not (__helper22(ctx) and __helper23(ctx) and __helper24(ctx))) // ?
		{
			assert(sp29 < ctx.offset and "invalid offset match ?");
			ctx.offset = sp29; // ignore the results if it does not match
		}

		ctx.commit(_ruleOffset, rgNumber);
		return true;
	}


	//! Rule start
	static bool yyrgStart(Datasource& ctx)
	{
		TRACE("entering rgStart");
		uint _ruleOffset = ctx.enterRule(rgStart);

		ctx.push(); // 0 or 1
		if (not (yyrgExpr(ctx))) // ?
			return false;

		ctx.commit(_ruleOffset, rgStart);
		return true;
	}


	static const AnyString datatext30(" \t\n\r", 4);



	//! Rule wp
	static inline bool yyrgWp(Datasource& ctx)
	{
		TRACE("entering rgWp [inline]");

		while (ctx.matchOneOf(datatext30 /*  \t\n\r */)) // 0-1 or more
		{}

		return true;
	}





} // anonymous namespace





	AnyString RuleToString(enum Rule ruleid)
	{
		switch (ruleid)
		{
			case rgUnknown:
				return "<unknown>";
			case rgExpr:
				return "expr";
			case rgExprAtom:
				return "expr-atom";
			case rgExprGroup:
				return "expr-group";
			case rgExprProduct:
				return "expr-product";
			case rgExprSum:
				return "expr-sum";
			case rgNumber:
				return "number";
			case rgStart:
				return "start";
			case rgWp:
				return "wp";
			case rgEOF:
				return "EOF";
		}
		return "<error>";
	}




	Parser::Parser()
		: pData()
	{
		onURILoading.bind(& StandardURILoaderHandler);
	}


	Parser::~Parser()
	{
		delete (Datasource*) pData;
	}


	void Parser::clear()
	{
		root = nullptr;
		delete (Datasource*) pData;
		pData = nullptr;
		if (not notifications.empty())
			Notification::Vector().swap(notifications);
	}


	bool Parser::loadFromFile(const AnyString& filename)
	{
		if (!pData)
			pData = new Datasource(notifications);

		Datasource& ctx = *((Datasource*) pData);
		ctx.open(filename);
		DATASOURCE_PARSE(ctx);
		return ctx.success;
	}


	bool Parser::load(const AnyString& content)
	{
		if (!pData)
			pData = new Datasource(notifications);

		Datasource& ctx = *((Datasource*) pData);
		ctx.openContent(content);
		DATASOURCE_PARSE(ctx);
		return ctx.success;
	}


	void Node::ExportToHTML(Clob& out, const Node& node)
	{
		assert(&node and "invalid reference to node");

		String tmp;
		String indent;
		InternalNodeExportHTML(out, node, indent, tmp);
	}


	void Node::Export(Clob& out, const Node& node, bool color)
	{
		assert(&node and "invalid reference to node");

		String tmp;
		String indent;
		if (not color)
			InternalNodeExportConsole<false>(out, node, false, indent, tmp);
		else
			InternalNodeExportConsole<true>(out, node, false, indent, tmp);
	}


	void Node::Export(Clob& out, const Node& node)
	{
		Export(out, node, ::Yuni::System::Console::IsStdoutTTY());
	}


	void Parser::translateOffset(uint& column, uint& line, const Node& node) const
	{
		column = 0;
		line = 0;
		if (YUNI_LIKELY(pData))
		{
			Datasource& ctx = *((Datasource*) pData);
			ctx.translateOffset(column, line, node.offset);
		}
	}


	void Parser::translateOffset(uint& column, uint& line, uint offset) const
	{
		column = 0;
		line = 0;
		if (YUNI_LIKELY(pData))
		{
			Datasource& ctx = *((Datasource*) pData);
			ctx.translateOffset(column, line, offset);
		}
	}


	uint Parser::translateOffsetToLine(const Node& node) const
	{
		uint column;
		uint line;
		translateOffset(column, line, node);
		return line;
	}


	Node& Node::operator = (const Node& rhs)
	{
		rule = rhs.rule;
		offset = rhs.offset;
		offsetEnd = rhs.offsetEnd;
		text = rhs.text;
		children = rhs.children;
		return *this;
	}


	void Node::clear()
	{
		children.clear();
		text.clear();
		offset = 0;
		offsetEnd = 0;
		rule = rgUnknown;
	}







} // namespace Calculator
} // namespace Demo
