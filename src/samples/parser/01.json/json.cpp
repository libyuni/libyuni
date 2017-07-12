/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** gitlab: https://gitlab.com/libyuni/libyuni/
** github: https://github.com/libyuni/libyuni/ {mirror}
*/
#include "json.h"
#include <cassert>
#include <yuni/core/string.h>
#include <yuni/io/file.h>
#include <yuni/io/directory.h>
#include <yuni/core/dictionary.h>
#include <yuni/core/noncopyable.h>
#include <yuni/datetime/timestamp.h>
#include <yuni/core/system/console/console.h>
#include <iostream>

using namespace Yuni;


namespace Demo
{
namespace JSON
{
namespace // anonymous
{


	static constexpr const bool _attrAttributeCapture[] =
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
		true, // rgInteger
		false, // rgKey
		false, // rgNull
		false, // rgNumber
		true, // rgNumberDef
		false, // rgNumberExponent
		true, // rgNumberHexa
		false, // rgNumberQualifier
		true, // rgNumberQualifierType
		true, // rgNumberSign
		true, // rgNumberValue
		false, // rgObject
		false, // rgObjectEntry
		true, // rgPunc
		false, // rgSingleChar
		false, // rgSp
		false, // rgStart
		false, // rgString
		true, // rgStringLiteral
		false, // rgTkBraceClose
		false, // rgTkBraceOpen
		false, // rgTkBracketClose
		false, // rgTkBracketOpen
		false, // rgTkColon
		false, // rgTkComma
		false, // rgTkCommentBlock
		true, // rgTkCommentBlockContent
		true, // rgTkCommentEndBlock
		false, // rgTkCommentLine
		true, // rgTkCommentLineContent
		true, // rgTkCommentStartBlock
		true, // rgTkCommentStartInline
		false, // rgTkCommentSubbk
		false, // rgTkDot
		false, // rgTkDoubleQuote
		false, // rgTrue
		false, // rgValue
		true, // rgWp
		false, // rgEOF
	};


	static bool ruleAttributeCapture(enum Rule ruleid)
	{
		assert((uint) ruleid < (uint) ruleCount);
		return _attrAttributeCapture[(uint) ruleid];
	}






	static constexpr const bool _attrAttributeImportant[] = {
		false, // rgUnknown
		false, // rgArray
		false, // rgCharExtended
		false, // rgDigit
		false, // rgError
		false, // rgErrorBracket
		false, // rgErrorEol
		false, // rgErrorParenthese
		false, // rgErrorSemicolon
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


	static inline bool ruleAttributeImportant(enum Rule ruleid)
	{
		assert((uint) ruleid < (uint) ruleCount);
		return _attrAttributeImportant[(uint) ruleid];
	}




	static const char* const _attrAttributeSimpleTextCapture[] =
	{
		nullptr, // rgUnknown
		nullptr, // rgArray
		nullptr, // rgCharExtended
		nullptr, // rgDigit
		nullptr, // rgError
		nullptr, // rgErrorBracket
		nullptr, // rgErrorEol
		nullptr, // rgErrorParenthese
		nullptr, // rgErrorSemicolon
		"false", // rgFalse
		nullptr, // rgHex
		nullptr, // rgInteger
		nullptr, // rgKey
		"null", // rgNull
		nullptr, // rgNumber
		nullptr, // rgNumberDef
		nullptr, // rgNumberExponent
		nullptr, // rgNumberHexa
		nullptr, // rgNumberQualifier
		nullptr, // rgNumberQualifierType
		nullptr, // rgNumberSign
		nullptr, // rgNumberValue
		nullptr, // rgObject
		nullptr, // rgObjectEntry
		nullptr, // rgPunc
		nullptr, // rgSingleChar
		nullptr, // rgSp
		nullptr, // rgStart
		nullptr, // rgString
		nullptr, // rgStringLiteral
		"}", // rgTkBraceClose
		"{", // rgTkBraceOpen
		"]", // rgTkBracketClose
		"[", // rgTkBracketOpen
		":", // rgTkColon
		",", // rgTkComma
		nullptr, // rgTkCommentBlock
		nullptr, // rgTkCommentBlockContent
		"*/", // rgTkCommentEndBlock
		nullptr, // rgTkCommentLine
		nullptr, // rgTkCommentLineContent
		"/*", // rgTkCommentStartBlock
		"//", // rgTkCommentStartInline
		nullptr, // rgTkCommentSubbk
		".", // rgTkDot
		"\"", // rgTkDoubleQuote
		"true", // rgTrue
		nullptr, // rgValue
		nullptr, // rgWp
		nullptr, // rgEOF
	};


	static AnyString ruleAttributeSimpleTextCapture(enum Rule ruleid)
	{
		assert((uint) ruleid < (uint) ruleCount);
		return _attrAttributeSimpleTextCapture[(uint) ruleid];
	}



	//! Add traces to the stdcout
	# define HAS_TRACES 0

	//! Enable time-consuming checks
	#ifndef NDEBUG
	# define MORE_CHECKS 1
	#else
	# define MORE_CHECKS 0
	#endif

	# define HAS_STATISTICS 0


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
		//! Rule ID - a negative value means that the rule has not been commited yet
		int32_t rule;
		//! hint about the parent frame (last uncommited)
		uint32_t lastUncommited;
		//! Parent rule
		uint32_t parent;
		//! Iterator on the source
		uint32_t offset;
		//! Index of the current source url (see `urls`)
		uint32_t urlindex;
		//! End offset - means nothing if rule == 0
		uint32_t offsetEnd;
	};




	class Datasource final
	{
	public:
		enum class OpenFlag
		{
			opened,
			error,
			ignore,
		};

		//! Size (in bytes), when increasing the stack capacity
		static constexpr uint32_t chunkGrowSize = 4096; // 1024 * sizeof(Chunk) -> 16KiB


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
		void pushInclude(uint32_t urlindex);
		uint32_t push();
		uint32_t enterRule(enum Rule rule);
		void restart(uint32_t from);
		void commit(uint32_t ruleOffset, enum Rule rule);
		//@}

		//! \name Filename manipulation
		//@{
		//! Open a new url
		OpenFlag open(const AnyString& newurl);
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
		void translateOffset(uint32_t& column, uint32_t& line, uint32_t offset) const;
		//@}


	public:
		//! Stack
		Chunk* stack;
		uint32_t offset;
		# if HAS_STATISTICS != 0 && !defined(NDEBUG)
		uint32_t maxOffset;
		uint64 duration;
		# endif
		uint32_t capacity;

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
		void findOptimalNewOffsetAfterCommit(uint32_t ruleOffset);

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
		uint32_t oldOffset;
		uint32_t oldFileOffset;
	};




	#if MORE_CHECKS != 0
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




	Datasource::Datasource(Notification::Vector& notifications) :
		stack(),
		offset(),
		capacity(chunkGrowSize),
		notifications(notifications)
	{
		stack = (Chunk*)::malloc(sizeof(Chunk) * chunkGrowSize);
	}


	Datasource::~Datasource()
	{
		// rootnode = nullptr
		::free(stack);
	}


	void Datasource::clear()
	{
		offset = 0;
		success = false;

		# if HAS_STATISTICS != 0 && !defined(NDEBUG)
		maxOffset = 0;
		duration  = 0;
		# endif

		rootnode = nullptr;

		// avoid too much memory consumption
		if (capacity > chunkGrowSize * 1024)
		{
			auto* newstack = (Chunk*)::realloc(stack, sizeof(Chunk) * capacity);
			if (YUNI_UNLIKELY(!newstack))
				throw std::bad_alloc();
			capacity = chunkGrowSize * 1024;
			stack = newstack;
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
		firstFrame.lastUncommited = 0;
	}


	void Datasource::grow()
	{
		if (not (offset < capacity))
		{
			uint32_t newcapacity = capacity + chunkGrowSize;
			// WARNING The variable 'offset' might be unreliable for displying
			// and/or checking values from the stack frames
			#if MORE_CHECKS != 0
			assert(newcapacity < ARBITRARY_HARD_LIMIT); // arbitrary
			#endif

			// grow the stack
			auto* newstack = (Chunk*) realloc(stack, newcapacity * sizeof(Chunk));
			if (YUNI_UNLIKELY(!newstack))
				throw std::bad_alloc();
			stack = newstack;
			capacity = newcapacity;

			#if MORE_CHECKS != 0  // post-checks
			assert(offset < capacity);
			#endif
		}
	}


	void Datasource::pushInclude(uint32_t urlindex)
	{
		#if MORE_CHECKS != 0
		assert(urlindex < ARBITRARY_HARD_LIMIT);
		assert(capacity + chunkGrowSize < ARBITRARY_HARD_LIMIT); // arbitrary
		assert(offset < ARBITRARY_HARD_LIMIT); // arbitrary
		assert(urls.size() == contents.size());
		assert(urls.size() == reverseUrlIndexes.size());
		#endif

		if (YUNI_UNLIKELY(not (++offset < capacity))) // grow
			grow();

		Chunk& cursor    = stack[offset];
		cursor.offset    = 0;
		cursor.urlindex  = urlindex;
		cursor.rule      = 0;
		cursor.offsetEnd = 0;
		cursor.parent    = (uint) -1;
		cursor.lastUncommited = (uint) -1;
	}


	inline uint32_t Datasource::push()
	{
		#if MORE_CHECKS != 0
		assert(capacity + chunkGrowSize < ARBITRARY_HARD_LIMIT); // arbitrary
		assert(offset < ARBITRARY_HARD_LIMIT); // arbitrary
		#endif

		uint32_t oldoffset = offset;
		uint32_t newoffset = oldoffset + 1;
		offset = newoffset;
		if (YUNI_UNLIKELY(0 == (newoffset % chunkGrowSize))) // grow
			grow();

		assert(offset < capacity);
		# if HAS_STATISTICS != 0 && !defined(NDEBUG)
		if (newoffset > maxOffset)
			maxOffset = newoffset;
		# endif

		Chunk* const prev   = &(stack[oldoffset]);
		Chunk* const cursor = prev + 1;

		#if MORE_CHECKS != 0
		assert(prev->offset   < ARBITRARY_HARD_LIMIT);
		assert(prev->urlindex < ARBITRARY_HARD_LIMIT);
		#endif

		cursor->rule      = 0;
		cursor->offset    = prev->offset;
		cursor->urlindex  = prev->urlindex;
		// cursor->offsetEnd = 0;  means nothing if rule == 0
		cursor->lastUncommited = prev->lastUncommited;

		// Since TRACE_LOCAL uses cursor.offset, this macro must be called after
		// the previous initialization
		TRACE_LOCAL("    push at offset " << prev->offset);
		return oldoffset;
	}


	inline uint32_t Datasource::enterRule(enum Rule rule)
	{
		#if MORE_CHECKS != 0
		assert(capacity + chunkGrowSize < ARBITRARY_HARD_LIMIT); // arbitrary
		assert(offset < ARBITRARY_HARD_LIMIT); // arbitrary
		assert((uint) rule < (uint) ruleCount);
		#endif

		// This method is quite similar to `push`
		uint32_t oldoffset = offset;
		uint32_t newoffset = oldoffset + 1;
		offset = newoffset;
		if (YUNI_UNLIKELY(0 == (newoffset % chunkGrowSize)))
			grow();

		# if HAS_STATISTICS != 0 && !defined(NDEBUG)
		if (newoffset > maxOffset)
			maxOffset = newoffset;
		# endif

		#if MORE_CHECKS != 0
		assert(newoffset > 0);
		assert(capacity < ARBITRARY_HARD_LIMIT);
		#endif

		Chunk* const prev   = &(stack[oldoffset]);
		Chunk* const cursor = prev + 1;

		cursor->rule      = - (static_cast<int>(rule));
		cursor->offset    = prev->offset;
		cursor->urlindex  = prev->urlindex;
		cursor->lastUncommited = offset;
		cursor->offsetEnd = cursor->offset; // store offset for reuse at commit*/

		TRACE_LOCAL("    enter at offset " << cursor->offset);
		return newoffset;
	}


	inline void Datasource::findOptimalNewOffsetAfterCommit(uint32_t ruleOffset)
	{
		// trying to reduce the stack size by removing all stack frames which are
		// not marked as a real rule this optimization can reduce by 3 the size of
		// the stack and reduce by 4 the time required to parse some big input

		// if a stack frame with the variable 'rule' not null (aka a stack frame
		// dedicated to create a node in the AST), then it would not be safe to
		// get rid of the end of the stack
		const Chunk* const end = &(stack[offset]);
		const Chunk* it = &(stack[ruleOffset + 1]);
		for (; it <= end; ++it)
		{
			if (it->rule != 0)
			{
				++offset;
				return;
			}
		}
		// it seems that it is safe to ignore the end of the stack - hourray \o/
		offset = ruleOffset + 1;
	}


	void Datasource::commit(uint32_t ruleOffset, enum Rule rule)
	{
		// committing the current stack frame
		{
			Chunk* const ruleCursor = &(stack[ruleOffset]);

			#if MORE_CHECKS != 0
			assert(ruleCursor->rule == - (int) rule and "inconsistency ruleid in the stack");
			assert(offset >= ruleOffset and "invalid stack entre/commit");
			assert(stack[offset].offset >= ruleCursor->offset and "Huh? Should go forward...");
			#endif

			// restore previous offset - a temporary variable must be used
			// it may happen that `offset` and `ruleOffset` are the same
			uint32_t newEndOffset     = stack[offset].offset;
			ruleCursor->offset    = ruleCursor->offsetEnd;
			ruleCursor->offsetEnd = newEndOffset;
			ruleCursor->rule      = static_cast<int>(rule);

			#if MORE_CHECKS != 0
			assert(ruleCursor->offset <= ruleCursor->offsetEnd and "invalid boundaries");
			#endif

			// try to find the lastest uncommited rule
			{
				uint32_t lastUncommited = ruleOffset - 1;
				do
				{
					assert(lastUncommited < capacity);
					Chunk* element = &(stack[lastUncommited]);
					if (element->rule < 0)
					{
						ruleCursor->parent = lastUncommited;
						if (lastUncommited > 0)
							element->lastUncommited = (element - 1)->lastUncommited;
						break;
					}

					assert(lastUncommited > 0);
					lastUncommited = (element - 1)->lastUncommited;
				}
				while (true);
			}

			# if HAS_TRACES != 0
			uint32_t csize = ruleCursor.offsetEnd - ruleCursor.offset;
			AnyString content(contents[ruleCursor.urlindex], ruleCursor.offset, csize);
			TRACE_LOCAL("    COMMIT " << ruleToString(rule) << ", offset " << newEndOffset << ": " << content);
			# endif
		}

		// new stack frame - push - to keep this element in the stack
		// calculating the new offset: [optimization]
		findOptimalNewOffsetAfterCommit(ruleOffset);

		# if HAS_STATISTICS != 0 && !defined(NDEBUG)
		if (offset > maxOffset)
			maxOffset = offset;
		# endif

		if (YUNI_UNLIKELY(not (offset < capacity))) // grow !
			grow();

		// the item at `ruleOffset` may have changed - re-acquiring a new ref on it
		// (cf `grow()`)
		assert(ruleOffset != 0 and ruleOffset < capacity);
		const Chunk* ruleCursor = &(stack[ruleOffset]);
		Chunk& cursor     = stack[offset];
		cursor.offset     = ruleCursor->offsetEnd;
		cursor.urlindex   = ruleCursor->urlindex;
		cursor.rule       = 0;
		cursor.lastUncommited = (ruleCursor - 1)->lastUncommited;
	}


	inline void Datasource::restart(uint32_t from)
	{
		// the method `push` returns `offset - 1`
		#if MORE_CHECKS != 0
		assert(from + 1 < capacity);
		assert(from < offset and "can not restart from a non existing frame");
		#endif

		offset = from + 1;

		Chunk* const cursor     = &(stack[offset]);
		const Chunk* const prev = cursor - 1;

		cursor->offset   = prev->offset;
		cursor->urlindex = prev->urlindex;
		cursor->rule     = 0;
		cursor->lastUncommited = prev->lastUncommited;
		TRACE_LOCAL("    restart at offset " << cursor->offset);
	}


	Datasource::OpenFlag Datasource::open(const AnyString& newurl)
	{
		if (YUNI_UNLIKELY(newurl.empty()))
			return OpenFlag::error;
		// getting the root directory once and for all if not already done
		// the operation is not done in the constructor to let the caller
		// initialize this variable if needed
		if (root.empty())
			::Yuni::IO::Directory::Current::Get(root, false);

		// canonicalizing the filename
		::Yuni::String filename;
		::Yuni::IO::Canonicalize(filename, newurl, root);

		// filename index
		uint32_t index;
		::Yuni::Dictionary<String, uint>::Unordered::const_iterator knownIndex = reverseUrlIndexes.find(filename);
		if (YUNI_LIKELY(knownIndex == reverseUrlIndexes.end()))
		{
			// load the entire content in memory
			Clob newContent;
			if (YUNI_UNLIKELY(::Yuni::IO::errNone != ::Yuni::IO::File::LoadFromFile(newContent, filename)))
				return OpenFlag::error;

			newContent.trimRight();
			if (YUNI_UNLIKELY(newContent.empty()))
				return OpenFlag::ignore;

			// adding an artifial line feed to make sure the parser will be able to end
			newContent.append('\n');

			assert(contents.size() == urls.size());
			// indexes
			index = (uint) contents.size();
			reverseUrlIndexes[filename] = index;

			contents.emplace_back();
			contents.back().swap(newContent);

			urls.emplace_back();
			urls.back().swap(filename);
		}
		else
			index = knownIndex->second;

		// new item in the stack
		pushInclude(index);
		return OpenFlag::opened;
	}


	void Datasource::openContent(const AnyString& content)
	{
		if (not content.empty())
		{
			const String filename = "<memory>";
			// filename index
			uint32_t index;
			auto knownIndex = reverseUrlIndexes.find(filename);
			if (YUNI_LIKELY(knownIndex == reverseUrlIndexes.end()))
			{
				assert(contents.size() == urls.size());
				// indexes
				index = static_cast<uint>(contents.size());
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
	}


	inline bool Datasource::matchSingleAsciiChar(char c)
	{
		assert(offset < capacity);
		Chunk& cursor    = stack[offset];
		assert(cursor.urlindex < contents.size());
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
		assert(offset < capacity);
		Chunk& cursor = stack[offset];
		assert(cursor.urlindex < contents.size());
		if (AnyString(contents[cursor.urlindex], cursor.offset).startsWith(text))
		{
			cursor.offset += text.size();
			return true;
		}
		return false;
	}


	inline bool Datasource::matchOneOf(const AnyString& text)
	{
		assert(offset < capacity);
		Chunk& cursor    = stack[offset];
		assert(cursor.urlindex < contents.size());
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
		assert(offset < capacity);
		Chunk& cursor    = stack[offset];
		assert(cursor.urlindex < contents.size());
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
		assert(offset < capacity);
		Chunk& cursor    = stack[offset];
		assert(cursor.urlindex < contents.size());
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
		out << indent << "<div class=\"node\">";
		out << "<span class=\"rule\">" << ruleToString(node.rule) << "</span>";

		bool attrCapture = ruleAttributeCapture(node.rule);
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
			for (uint32_t i = 0; i != node.children.size(); ++i)
			{
				out << indent << "<li>\n";
				InternalNodeExportHTML(out, node.children[i], indent, tmp);
				out << indent << "</li>\n";
			}

			indent.chop(4);
			out << indent << "</ul>\n" << indent;
		}
		out << "</div>\n";
	}


	static void InternalNodeExportJSON(Clob& out, const Node& node, bool hasSibling, String& indent, String& tmp,
		void (*callback)(Yuni::Dictionary<AnyString, YString>::Unordered&, const Node&))
	{
		using namespace ::Yuni::System::Console;

		typedef Yuni::Dictionary<AnyString, YString>::Unordered  DictType;
		DictType dict;
		dict["rule"] = ruleToString(node.rule);
		dict["prefix"] = indent;

		if (YUNI_UNLIKELY(ruleIsError(node.rule)))
		{
			dict["rule-type"] = "error";
		}
		else
		{
			if (YUNI_UNLIKELY(ruleAttributeImportant(node.rule)))
				dict["rule-type"] = "important";
			else
				dict["rule-type"] = nullptr;
		}

		bool attrCapture = ruleAttributeCapture(node.rule);
		if (attrCapture)
		{
			dict["text"] = node.text;
			dict["text-capture"] = nullptr;
		}
		else
		{
			// it can be interresting to print the text itself when the node
			// is a simple text capture
			AnyString textCapture = ruleAttributeSimpleTextCapture(node.rule);
			if (not textCapture.empty())
			{
				dict["text"] = nullptr;
				dict["text-capture"] = textCapture;
			}
			else
			{
				dict["text"] = nullptr;
				dict["text-capture"] = nullptr;
			}
		}

		if (callback)
			callback(dict, node);

		// exporting the dict
		{
			out << "\t{";
			bool  first = true;
			for (DictType::const_iterator i = dict.begin(); i != dict.end(); ++i)
			{
				tmp = i->second;
				tmp.replace("\"", "\\\"");
				tmp.replace("\n", "\\n");
				tmp.replace("\t", "\\t");
				tmp.replace("\r", "\\r");

				if (not first)
					out << ",\n";
				else
					out << '\n';
				out << "\t\t\"" << i->first << "\": \"" << tmp << "\"";
				first = false;
			}
			out << "\n\t},\n";
		}

		// sub nodes
		if (not node.children.empty())
		{
			if (hasSibling)
				indent.append("|   ", 4);
			else
				indent.append("    ", 4);

			for (uint32_t i = 0; i != node.children.size(); ++i)
			{
				bool hasSibling = (i != node.children.size() - 1);
				InternalNodeExportJSON(out, node.children[i], hasSibling, indent, tmp, callback);
			}

			indent.chop(4);
		}
	}


	template<bool ColorT>
	static void InternalNodeExportConsole(Clob& out, const Node& node, bool hasSibling, String& indent,
		String& tmp, Node::ExportCallback callback)
	{
		using namespace ::Yuni::System::Console;

		if (ColorT)
			::Yuni::System::Console::SetTextColor(out, blue);
		out << indent;

		if (not ColorT)
		{
			out << ruleToString(node.rule);
		}
		else
		{
			if (YUNI_UNLIKELY(ruleIsError(node.rule)))
			{
				::Yuni::System::Console::SetTextColor(out, red);
				out << ruleToString(node.rule);
				::Yuni::System::Console::ResetTextColor(out);
			}
			else
			{
				if (YUNI_UNLIKELY(ruleAttributeImportant(node.rule)))
				{
					::Yuni::System::Console::SetTextColor(out, purple);
					out << ruleToString(node.rule);
					::Yuni::System::Console::ResetTextColor(out);
				}
				else
				{
					::Yuni::System::Console::ResetTextColor(out);
					out << ruleToString(node.rule);
				}
			}
		}

		bool attrCapture = ruleAttributeCapture(node.rule);
		if (attrCapture)
		{
			tmp = node.text;
			tmp.replace("\n", "\\n");
			tmp.replace("\t", "\\t");
			tmp.replace("\r", "\\r");
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
			AnyString textCapture = ruleAttributeSimpleTextCapture(node.rule);
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
			if (ColorT)
				::Yuni::System::Console::ResetTextColor(out);
		}

		out << '\n';
		//if (!callback)
		//	out << '\n';
		//else
		//{
			//if (ColorT)
			//	::Yuni::System::Console::SetTextColor(out, lightblue);
			//out << " [" << (void*) &node << "]\n";
			//if (ColorT)
			//	::Yuni::System::Console::ResetTextColor(out);
		//}

		if (callback) // callback for additional information ?
		{
			tmp.clear();
			callback(node, tmp);
			if (not tmp.empty())
			{
				AnyString prefix = ":: ";
				String newIndent;
				newIndent += '\n';
				newIndent += indent;
				newIndent.append(prefix);

				tmp.trimRight('\n');
				tmp.replace("\n", newIndent);

				if (ColorT)
					::Yuni::System::Console::SetTextColor(out, blue);
				out << indent;
				if (ColorT)
					::Yuni::System::Console::SetTextColor(out, yellow);
				out << prefix;
				if (ColorT)
					::Yuni::System::Console::ResetTextColor(out);
				out << tmp << '\n';
			}
		}

		// sub nodes
		if (not node.children.empty())
		{
			if (hasSibling)
				indent.append("|   ", 4);
			else
				indent.append("    ", 4);

			for (uint32_t i = 0; i != node.children.size(); ++i)
			{
				bool hasSibling = (i != node.children.size() - 1);
				InternalNodeExportConsole<ColorT>(out, node.children[i], hasSibling, indent, tmp, callback);
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
			uint32_t realRuleCount = 0;

			for (uint32_t i = 0; i != offset; ++i)
			{
				if (stack[i].rule > 0)
					++realRuleCount;
			}

			TRACE_INFO("STATISTICS");
			TRACE_INFO("result: " << (success ? "OK" : "FAILED") << "  (" << duration << "ms)");
			uint32_t ratio = (uint)((double) realRuleCount * 100. / offset);
			TRACE_INFO("stack: " << realRuleCount << " rules for " << offset << " stack frames, ratio: "
				<< ratio << "% (higher is better), max depth: " << maxOffset);
			TRACE_INFO("stack: " << ((capacity * sizeof(Chunk)) / 1024) << " KiB (capacity: " << capacity
				<< ", " << sizeof(Chunk) << " bytes per frame)");
			uint64 totalCapa = 0;
			for (uint32_t i = 0; i != (uint) contents.size(); ++i)
				totalCapa += contents[i].capacity();
			TRACE_INFO("content from urls: " << contents.size() << ", total " << (totalCapa / 1024) << " KiB in memory");

			totalCapa += (capacity * sizeof(Chunk));
			for (uint32_t i = 0; i != (uint) urls.size(); ++i)
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
		uint32_t astNodesSize = offset + 1;
		Node** astNodes = (Node**)::calloc(astNodesSize, sizeof(void*));

		// pseudo root node to avoid special cases for retrieving the parent node
		astNodes[0] = new Node();
		astNodes[0]->rule = rgUnknown;
		astNodes[0]->offset = 0;
		astNodes[0]->offsetEnd = 0;

		const Chunk* cursor = &(stack[0]);
		const Chunk* const end = cursor + offset;
		uint32_t i = 0;

		// starting from the first real frame
		while (++cursor != end)
		{
			++i;
			#if MORE_CHECKS != 0
			if (YUNI_UNLIKELY(cursor->rule < 0))
			{
				std::cerr << "assert failed: invalid rule  ";
				PrintFrame(std::cerr, *cursor) << std::endl;
				assert(cursor->rule >= 0 and "some rules are not commited - the parse should have failed");
			}
			# endif

			if (not (cursor->rule > 0)) // only commited rules
				continue;


			#if MORE_CHECKS != 0
			if (YUNI_UNLIKELY(not (cursor->parent < offset)))
			{
				std::cerr << "assert failed: invalid parent: " << i << "/" << offset << ",  ";
				PrintFrame(std::cerr, *cursor) << std::endl;
				assert(cursor->parent < offset and "invalid parent index");
			}
			assert(cursor->parent < astNodesSize);
			assert(!(!astNodes[cursor->parent]) and "invalid parent");
			# endif

			// The grammar rule for the current node
			auto rule = (enum Rule) cursor->rule;

			assert(cursor->parent < astNodesSize);

			Node* newnode = new Node();
			newnode->rule      = rule;
			newnode->offset    = cursor->offset;
			newnode->offsetEnd = cursor->offsetEnd;

			// Flag to determine whether the node has to capture the content or not
			if (_attrAttributeCapture[static_cast<uint32_t>(rule)])
			{
				// Checking integrity of the captured content
				assert(
					cursor->offsetEnd >= cursor->offset                // valid end offset
					and cursor->offsetEnd != (uint) -1                // valid end offset
					and cursor->offset    != (uint) -1                // valid offset
					and cursor->urlindex  <  (uint)contents.size()    // valuid url index
					and cursor->offset    <  contents[cursor->urlindex].size()  // valid range
					and cursor->offsetEnd <= contents[cursor->urlindex].size() // valid range
					and "invalid offset for content capture");

				// size of the captured content
				uint32_t size = cursor->offsetEnd - cursor->offset;
				// Captured content
				newnode->text.assign(contents[cursor->urlindex], size, cursor->offset);
			}

			astNodes[i] = newnode;
			astNodes[cursor->parent]->children.push_back(newnode);
		}

		rootnode = YUNI_LIKELY((astNodes[0]->children.size() == 1))
			? &(astNodes[0]->children.front()) : nullptr;

		delete astNodes[0];
		free(astNodes);
	}


	void Datasource::notify(const AnyString& message) const
	{
		assert(offset < capacity);
		Chunk& cursor    = stack[offset];
		assert(cursor.urlindex < contents.size());
		const Clob& data = contents[cursor.urlindex];

		uint32_t line = 1;
		uint32_t x = 0;

		// trying to find the appropriate line index
		if (cursor.offset < data.size())
		{
			for (uint32_t i = 0; i != cursor.offset; ++i)
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


	bool Datasource::isParseComplete() const
	{
		if (offset > 1)
		{
			assert(offset < capacity);
			assert(stack[offset].urlindex < (uint) urls.size());
			assert(stack[0].rule == - (int) rgEOF and "invalid stack");

			// pseudo commit the root frame
			stack[0].rule = + (int) rgEOF;

			// trying to find the commit rule "start", which should be at index 1
			const Chunk* const end = &(stack[offset]);
			const Chunk* cursor = &(stack[1]);
			do
			{
				if (cursor->rule == rgStart)
				{
					if (cursor->urlindex == 0) // the original input source
					{
						// the original content
						const Clob& data = contents[cursor->urlindex];

						if (cursor->offset < data.size() and cursor->offsetEnd <= data.size())
						{
							if (data.size() == cursor->offsetEnd)
								return true;
						}
					}
					break;
				}

				++cursor;
			}
			while (cursor <= end);

			notify("failed to parse");
		}
		return false;
	}


	inline void Datasource::translateOffset(uint32_t& column, uint32_t& line, uint32_t offset) const
	{
		uint32_t fileindex = 0;

		if (fileindex < (uint) contents.size())
		{
			// alias to the content
			const Clob& content = contents[fileindex];
			// maximum size
			uint32_t maxSize = std::min((uint) content.size(), offset);

			column = 0;
			line = 1;

			const char* c = &(content[0]);
			const char* const end = c + maxSize;

			for (; c != end; ++c)
			{
				if (*c == '\n')
				{
					column = 0;
					++line;
				}
				else
					++column;
			}

			if (column == 0)
				column = 1;
		}
		else
		{
			line = 0;
			column = 0;
		}
	}




	// Forward declarations
	static inline bool yyrgArray(Datasource& ctx);
	static inline bool yyrgCharExtended(Datasource& ctx);
	static inline bool yyrgDigit(Datasource& ctx);
	static inline bool yyrgError(Datasource& ctx);
	static inline bool yyrgErrorBracket(Datasource& ctx);
	static inline bool yyrgErrorEol(Datasource& ctx);
	static inline bool yyrgErrorParenthese(Datasource& ctx);
	static inline bool yyrgErrorSemicolon(Datasource& ctx);
	static inline bool yyrgFalse(Datasource& ctx);
	static inline bool yyrgHex(Datasource& ctx);
	static inline bool yyrgInteger(Datasource& ctx);
	static inline bool yyrgKey(Datasource& ctx);
	static inline bool yyrgNull(Datasource& ctx);
	static inline bool yyrgNumber(Datasource& ctx);
	static inline bool yyrgNumberDef(Datasource& ctx);
	static inline bool yyrgNumberExponent(Datasource& ctx);
	static inline bool yyrgNumberHexa(Datasource& ctx);
	static inline bool yyrgNumberQualifier(Datasource& ctx);
	static inline bool yyrgNumberQualifierType(Datasource& ctx);
	static inline bool yyrgNumberSign(Datasource& ctx);
	static inline bool yyrgNumberValue(Datasource& ctx);
	static inline bool yyrgObject(Datasource& ctx);
	static inline bool yyrgObjectEntry(Datasource& ctx);
	static inline bool yyrgPunc(Datasource& ctx);
	static inline bool yyrgSingleChar(Datasource& ctx);
	static inline bool yyrgSp(Datasource& ctx);
	static inline bool yyrgStart(Datasource& ctx);
	static inline bool yyrgString(Datasource& ctx);
	static inline bool yyrgStringLiteral(Datasource& ctx);
	static inline bool yyrgTkBraceClose(Datasource& ctx);
	static inline bool yyrgTkBraceOpen(Datasource& ctx);
	static inline bool yyrgTkBracketClose(Datasource& ctx);
	static inline bool yyrgTkBracketOpen(Datasource& ctx);
	static inline bool yyrgTkColon(Datasource& ctx);
	static inline bool yyrgTkComma(Datasource& ctx);
	static inline bool yyrgTkCommentBlock(Datasource& ctx);
	static inline bool yyrgTkCommentBlockContent(Datasource& ctx);
	static inline bool yyrgTkCommentEndBlock(Datasource& ctx);
	static inline bool yyrgTkCommentLine(Datasource& ctx);
	static inline bool yyrgTkCommentLineContent(Datasource& ctx);
	static inline bool yyrgTkCommentStartBlock(Datasource& ctx);
	static inline bool yyrgTkCommentStartInline(Datasource& ctx);
	static inline bool yyrgTkCommentSubbk(Datasource& ctx);
	static inline bool yyrgTkDot(Datasource& ctx);
	static inline bool yyrgTkDoubleQuote(Datasource& ctx);
	static inline bool yyrgTrue(Datasource& ctx);
	static inline bool yyrgValue(Datasource& ctx);
	static inline bool yyrgWp(Datasource& ctx);






	static inline bool __helper1(Datasource& ctx);
	static inline bool __helper2(Datasource& ctx);
	static inline bool __helper3(Datasource& ctx);
	static inline bool __helper4(Datasource& ctx);


	static inline bool __helper1(Datasource& ctx)
	{
		TRACE("    :: entering helper 1");
		ctx.push(); // 0 or 1
		if (not (yyrgWp(ctx))) // ?
			return false;
		return true;
	}

	static inline bool __helper2(Datasource& ctx)
	{
		TRACE("    :: entering helper 2");
		ctx.push(); // 0 or 1
		if (not (yyrgTkComma(ctx))) // ?
			return false;
		return true;
	}

	static inline bool __helper3(Datasource& ctx)
	{
		TRACE("    :: entering helper 3");
		ctx.push(); // 0 or 1
		if (not (yyrgWp(ctx))) // ?
			return false;
		return true;
	}

	static inline bool __helper4(Datasource& ctx)
	{
		TRACE("    :: entering helper 4");
		ctx.push(); // 0 or 1
		if (not (yyrgValue(ctx))) // ?
			return false;
		return true;
	}

	//! Rule array
	static inline bool yyrgArray(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgArray");
		uint32_t _ruleOffset = ctx.enterRule(rgArray);

		ctx.push(); // 0 or 1
		if (not (yyrgTkBracketOpen(ctx))) // ?
			return false;
		ctx.push(); // 0 or 1
		if (not (yyrgWp(ctx))) // ?
			return false;
		ctx.push(); // 0 or 1
		if (not (yyrgValue(ctx))) // ?
			return false;
		do // 0 or more
		{
			uint sp6 = ctx.push(); // 0 or 1
			if (not (__helper1(ctx) and __helper2(ctx) and __helper3(ctx) and __helper4(ctx)))
			{
				assert(sp6 < ctx.offset and "invalid offset match *");
				ctx.offset = sp6;
				break;
			}
		}
		while (true);
		ctx.push(); // 0 or 1
		if (not (yyrgWp(ctx))) // ?
			return false;
		ctx.push(); // 0 or 1
		if (not (yyrgTkBracketClose(ctx))) // ?
			return false;

		ctx.commit(_ruleOffset, rgArray);
		return true;
	}


	static const AnyString datatext11("abcdefghijklmnopqrstuvwxyz\\\"%$", 30);
	static const AnyString datatext12("xuU", 3);



	//! Rule char-extended
	static inline bool yyrgCharExtended(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgCharExtended [inline]");

		uint sp8 = ctx.push();
		bool rt7 = false;
		// condition: first part
		do
		{
			uint sp10 = ctx.push();
			bool rt9 = false;
			// condition: first part
			do
			{
				if (not (ctx.matchOneOf(datatext11 /* abcdefghijklmnopqrstuvwxyz\\\"%$ */))) // ?
					break;
				rt9 = true;
			}
			while (false);
			// condition: second part
			while (not rt9)
			{
				ctx.restart(sp10);
				if (not (ctx.matchOneOf(datatext12 /* xuU */))) // ?
					break;
				uint sp13 = ctx.push(); // 0 or 1
				if (not (yyrgHex(ctx))) // ?
				{
					assert(sp13 < ctx.offset and "invalid offset match min == 1");
					ctx.offset = sp13;
					break;
				}
				do // 1 or more
				{
					uint sp14 = ctx.push(); // 0 or 1
					if (not (yyrgHex(ctx)))
					{
						assert(sp14 < ctx.offset and "invalid offset match *");
						ctx.offset = sp14;
						break;
					}
				}
				while (true);
				rt9 = true;
				break;
			}

			if (not rt9)
			{
				assert(sp10 < ctx.offset and "invalid offset if");
				ctx.offset = sp10; // restore context before if
				break;
			}
			rt7 = true;
		}
		while (false);
		// condition: second part
		while (not rt7)
		{
			ctx.restart(sp8);
			uint sp15 = ctx.push(); // 0 or 1
			if (not (yyrgInteger(ctx))) // ?
			{
				assert(sp15 < ctx.offset and "invalid offset match min == 1");
				ctx.offset = sp15;
				break;
			}
			rt7 = true;
			break;
		}

		if (not rt7)
			return false;

		return true;
	}


	static const AnyString datatext16("0123456789", 10);



	//! Rule digit
	static inline bool yyrgDigit(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgDigit [inline]");

		if (not (ctx.matchOneOf(datatext16 /* 0123456789 */))) // ?
			return false;

		return true;
	}


	static const AnyString datatext17(" \t\r\n", 4);



	//! Rule error
	static inline bool yyrgError(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgError");
		uint32_t _ruleOffset = ctx.enterRule(rgError);

		while (ctx.notMatchOneOf(datatext17 /*  \t\r\n */)) // 0-1 or more
		{}

		ctx.commit(_ruleOffset, rgError);
		return true;
	}


	static const AnyString datatext18("[]\r\n", 4);



	//! Rule error-bracket
	static inline bool yyrgErrorBracket(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgErrorBracket");
		uint32_t _ruleOffset = ctx.enterRule(rgErrorBracket);

		while (ctx.notMatchOneOf(datatext18 /* []\r\n */)) // 0-1 or more
		{}

		ctx.commit(_ruleOffset, rgErrorBracket);
		return true;
	}


	static const AnyString datatext19("\r\n", 2);
	static const AnyString datatext20("\r\n", 2);



	//! Rule error-eol
	static inline bool yyrgErrorEol(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgErrorEol");
		uint32_t _ruleOffset = ctx.enterRule(rgErrorEol);

		while (ctx.notMatchOneOf(datatext19 /* \r\n */)) // 0-1 or more
		{}
		while (ctx.matchOneOf(datatext20 /* \r\n */)) // 0-1 or more
		{}

		ctx.commit(_ruleOffset, rgErrorEol);
		return true;
	}


	static const AnyString datatext21(")\r\n", 3);



	//! Rule error-parenthese
	static inline bool yyrgErrorParenthese(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgErrorParenthese");
		uint32_t _ruleOffset = ctx.enterRule(rgErrorParenthese);

		while (ctx.notMatchOneOf(datatext21 /* )\r\n */)) // 0-1 or more
		{}

		ctx.commit(_ruleOffset, rgErrorParenthese);
		return true;
	}


	static const AnyString datatext22(";\r\n", 3);
	static const AnyString datatext23(";\r\n", 3);



	//! Rule error-semicolon
	static inline bool yyrgErrorSemicolon(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgErrorSemicolon");
		uint32_t _ruleOffset = ctx.enterRule(rgErrorSemicolon);

		while (ctx.notMatchOneOf(datatext22 /* ;\r\n */)) // 0-1 or more
		{}
		while (ctx.matchOneOf(datatext23 /* ;\r\n */)) // 0-1 or more
		{}

		ctx.commit(_ruleOffset, rgErrorSemicolon);
		return true;
	}


	static const AnyString datatext24("false", 5);



	//! Rule false
	static inline bool yyrgFalse(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgFalse");
		uint32_t _ruleOffset = ctx.enterRule(rgFalse);

		if (not (ctx.matchString(datatext24 /* false */))) // ?
			return false;

		ctx.commit(_ruleOffset, rgFalse);
		return true;
	}


	static const AnyString datatext25("0123456789ABCDEFabcdef", 22);



	//! Rule hex
	static inline bool yyrgHex(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgHex [inline]");

		if (not (ctx.matchOneOf(datatext25 /* 0123456789ABCDEFabcdef */))) // ?
			return false;

		return true;
	}


	//! Rule integer
	static inline bool yyrgInteger(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgInteger");
		uint32_t _ruleOffset = ctx.enterRule(rgInteger);

		ctx.push(); // 0 or 1
		if (not (yyrgDigit(ctx))) // ?
			return false;
		do // 1 or more
		{
			uint sp26 = ctx.push(); // 0 or 1
			if (not (yyrgDigit(ctx)))
			{
				assert(sp26 < ctx.offset and "invalid offset match *");
				ctx.offset = sp26;
				break;
			}
		}
		while (true);

		ctx.commit(_ruleOffset, rgInteger);
		return true;
	}


	//! Rule key
	static inline bool yyrgKey(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgKey");
		uint32_t _ruleOffset = ctx.enterRule(rgKey);

		ctx.push(); // 0 or 1
		if (not (yyrgString(ctx))) // ?
			return false;

		ctx.commit(_ruleOffset, rgKey);
		return true;
	}


	static const AnyString datatext27("null", 4);



	//! Rule null
	static inline bool yyrgNull(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgNull");
		uint32_t _ruleOffset = ctx.enterRule(rgNull);

		if (not (ctx.matchString(datatext27 /* null */))) // ?
			return false;

		ctx.commit(_ruleOffset, rgNull);
		return true;
	}




	static inline bool __helper28(Datasource& ctx);
	static inline bool __helper29(Datasource& ctx);


	static inline bool __helper28(Datasource& ctx)
	{
		TRACE("    :: entering helper 28");
		ctx.push(); // 0 or 1
		if (not (yyrgNumberSign(ctx))) // ?
			return false;
		return true;
	}

	static inline bool __helper29(Datasource& ctx)
	{
		TRACE("    :: entering helper 29");
		ctx.push(); // 0 or 1
		if (not (yyrgWp(ctx))) // ?
			return false;
		return true;
	}

	//! Rule number
	static inline bool yyrgNumber(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgNumber");
		uint32_t _ruleOffset = ctx.enterRule(rgNumber);

		uint sp31 = ctx.push(); // 0 or 1
		if (not (__helper28(ctx) and __helper29(ctx))) // ?
		{
			assert(sp31 < ctx.offset and "invalid offset match ?");
			ctx.offset = sp31; // ignore the results if it does not match
		}
		uint sp33 = ctx.push();
		bool rt32 = false;
		// condition: first part
		do
		{
			uint sp34 = ctx.push(); // 0 or 1
			if (not (yyrgNumberHexa(ctx))) // ?
			{
				assert(sp34 < ctx.offset and "invalid offset match min == 1");
				ctx.offset = sp34;
				break;
			}
			rt32 = true;
		}
		while (false);
		// condition: second part
		while (not rt32)
		{
			ctx.restart(sp33);
			uint sp35 = ctx.push(); // 0 or 1
			if (not (yyrgNumberValue(ctx))) // ?
			{
				assert(sp35 < ctx.offset and "invalid offset match min == 1");
				ctx.offset = sp35;
				break;
			}
			rt32 = true;
			break;
		}

		if (not rt32)
			return false;
		uint sp36 = ctx.push(); // 0 or 1
		if (not (yyrgNumberQualifier(ctx))) // ?
		{
			assert(sp36 < ctx.offset and "invalid offset match ?");
			ctx.offset = sp36; // ignore the results if it does not match
		}

		ctx.commit(_ruleOffset, rgNumber);
		return true;
	}


	//! Rule number-def
	static inline bool yyrgNumberDef(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgNumberDef [inline]");

		ctx.push(); // 0 or 1
		if (not (yyrgNumber(ctx))) // ?
			return false;
		ctx.push(); // 0 or 1
		if (not (yyrgSp(ctx))) // ?
			return false;

		return true;
	}


	//! Rule number-exponent
	static inline bool yyrgNumberExponent(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgNumberExponent");
		uint32_t _ruleOffset = ctx.enterRule(rgNumberExponent);

		uint sp37 = ctx.push(); // 0 or 1
		if (not (yyrgNumberSign(ctx))) // ?
		{
			assert(sp37 < ctx.offset and "invalid offset match ?");
			ctx.offset = sp37; // ignore the results if it does not match
		}
		ctx.push(); // 0 or 1
		if (not (yyrgInteger(ctx))) // ?
			return false;

		ctx.commit(_ruleOffset, rgNumberExponent);
		return true;
	}


	static const AnyString datatext38("0x", 2);



	//! Rule number-hexa
	static inline bool yyrgNumberHexa(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgNumberHexa");
		uint32_t _ruleOffset = ctx.enterRule(rgNumberHexa);

		if (not (ctx.matchString(datatext38 /* 0x */))) // ?
			return false;
		ctx.push(); // 0 or 1
		if (not (yyrgHex(ctx))) // ?
			return false;
		do // 1 or more
		{
			uint sp39 = ctx.push(); // 0 or 1
			if (not (yyrgHex(ctx)))
			{
				assert(sp39 < ctx.offset and "invalid offset match *");
				ctx.offset = sp39;
				break;
			}
		}
		while (true);

		ctx.commit(_ruleOffset, rgNumberHexa);
		return true;
	}


	//! Rule number-qualifier
	static inline bool yyrgNumberQualifier(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgNumberQualifier");
		uint32_t _ruleOffset = ctx.enterRule(rgNumberQualifier);

		ctx.push(); // 0 or 1
		if (not (yyrgNumberQualifierType(ctx))) // ?
			return false;
		uint sp40 = ctx.push(); // 0 or 1
		if (not (yyrgInteger(ctx))) // ?
		{
			assert(sp40 < ctx.offset and "invalid offset match ?");
			ctx.offset = sp40; // ignore the results if it does not match
		}

		ctx.commit(_ruleOffset, rgNumberQualifier);
		return true;
	}


	static const AnyString datatext41("iuf", 3);



	//! Rule number-qualifier-type
	static inline bool yyrgNumberQualifierType(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgNumberQualifierType");
		uint32_t _ruleOffset = ctx.enterRule(rgNumberQualifierType);

		if (not (ctx.matchOneOf(datatext41 /* iuf */))) // ?
			return false;

		ctx.commit(_ruleOffset, rgNumberQualifierType);
		return true;
	}


	static const AnyString datatext42("+-", 2);



	//! Rule number-sign
	static inline bool yyrgNumberSign(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgNumberSign");
		uint32_t _ruleOffset = ctx.enterRule(rgNumberSign);

		if (not (ctx.matchOneOf(datatext42 /* +- */))) // ?
			return false;

		ctx.commit(_ruleOffset, rgNumberSign);
		return true;
	}


	static const AnyString datatext50("eE", 2);





	static inline bool __helper43(Datasource& ctx);
	static inline bool __helper44(Datasource& ctx);
	static inline bool __helper47(Datasource& ctx);
	static inline bool __helper48(Datasource& ctx);


	static inline bool __helper43(Datasource& ctx)
	{
		TRACE("    :: entering helper 43");
		ctx.push(); // 0 or 1
		if (not (yyrgTkDot(ctx))) // ?
			return false;
		return true;
	}

	static inline bool __helper44(Datasource& ctx)
	{
		TRACE("    :: entering helper 44");
		ctx.push(); // 0 or 1
		if (not (yyrgInteger(ctx))) // ?
			return false;
		return true;
	}

	static inline bool __helper47(Datasource& ctx)
	{
		TRACE("    :: entering helper 47");
		if (not (ctx.matchOneOf(datatext50 /* eE */))) // ?
			return false;
		return true;
	}

	static inline bool __helper48(Datasource& ctx)
	{
		TRACE("    :: entering helper 48");
		ctx.push(); // 0 or 1
		if (not (yyrgNumberExponent(ctx))) // ?
			return false;
		return true;
	}

	//! Rule number-value
	static inline bool yyrgNumberValue(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgNumberValue");
		uint32_t _ruleOffset = ctx.enterRule(rgNumberValue);

		ctx.push(); // 0 or 1
		if (not (yyrgInteger(ctx))) // ?
			return false;
		uint sp46 = ctx.push(); // 0 or 1
		if (not (__helper43(ctx) and __helper44(ctx))) // ?
		{
			assert(sp46 < ctx.offset and "invalid offset match ?");
			ctx.offset = sp46; // ignore the results if it does not match
		}
		uint sp51 = ctx.push(); // 0 or 1
		if (not (__helper47(ctx) and __helper48(ctx))) // ?
		{
			assert(sp51 < ctx.offset and "invalid offset match ?");
			ctx.offset = sp51; // ignore the results if it does not match
		}

		ctx.commit(_ruleOffset, rgNumberValue);
		return true;
	}




	static inline bool __helper52(Datasource& ctx);
	static inline bool __helper53(Datasource& ctx);
	static inline bool __helper54(Datasource& ctx);
	static inline bool __helper55(Datasource& ctx);


	static inline bool __helper52(Datasource& ctx)
	{
		TRACE("    :: entering helper 52");
		ctx.push(); // 0 or 1
		if (not (yyrgWp(ctx))) // ?
			return false;
		return true;
	}

	static inline bool __helper53(Datasource& ctx)
	{
		TRACE("    :: entering helper 53");
		ctx.push(); // 0 or 1
		if (not (yyrgTkComma(ctx))) // ?
			return false;
		return true;
	}

	static inline bool __helper54(Datasource& ctx)
	{
		TRACE("    :: entering helper 54");
		ctx.push(); // 0 or 1
		if (not (yyrgWp(ctx))) // ?
			return false;
		return true;
	}

	static inline bool __helper55(Datasource& ctx)
	{
		TRACE("    :: entering helper 55");
		ctx.push(); // 0 or 1
		if (not (yyrgObjectEntry(ctx))) // ?
			return false;
		return true;
	}

	//! Rule object
	static inline bool yyrgObject(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgObject");
		uint32_t _ruleOffset = ctx.enterRule(rgObject);

		ctx.push(); // 0 or 1
		if (not (yyrgTkBraceOpen(ctx))) // ?
			return false;
		ctx.push(); // 0 or 1
		if (not (yyrgWp(ctx))) // ?
			return false;
		ctx.push(); // 0 or 1
		if (not (yyrgObjectEntry(ctx))) // ?
			return false;
		do // 0 or more
		{
			uint sp57 = ctx.push(); // 0 or 1
			if (not (__helper52(ctx) and __helper53(ctx) and __helper54(ctx) and __helper55(ctx)))
			{
				assert(sp57 < ctx.offset and "invalid offset match *");
				ctx.offset = sp57;
				break;
			}
		}
		while (true);
		ctx.push(); // 0 or 1
		if (not (yyrgWp(ctx))) // ?
			return false;
		ctx.push(); // 0 or 1
		if (not (yyrgTkBraceClose(ctx))) // ?
			return false;

		ctx.commit(_ruleOffset, rgObject);
		return true;
	}


	//! Rule object-entry
	static inline bool yyrgObjectEntry(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgObjectEntry");
		uint32_t _ruleOffset = ctx.enterRule(rgObjectEntry);

		ctx.push(); // 0 or 1
		if (not (yyrgKey(ctx))) // ?
			return false;
		ctx.push(); // 0 or 1
		if (not (yyrgWp(ctx))) // ?
			return false;
		ctx.push(); // 0 or 1
		if (not (yyrgTkColon(ctx))) // ?
			return false;
		ctx.push(); // 0 or 1
		if (not (yyrgWp(ctx))) // ?
			return false;
		ctx.push(); // 0 or 1
		if (not (yyrgValue(ctx))) // ?
			return false;

		ctx.commit(_ruleOffset, rgObjectEntry);
		return true;
	}


	static const AnyString datatext58("!\"#$%&'()*+,\\-./:;<=>?@[]^_`{|}~", 32);



	//! Rule punc
	static inline bool yyrgPunc(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgPunc [inline]");

		if (not (ctx.matchOneOf(datatext58 /* !\"#$%&'()*+,\\-. / :;<=>?@[]^_`{|}~ */))) // ?
			return false;

		return true;
	}


	static const AnyString datatext62("\"\n", 2);



	//! Rule single-char
	static inline bool yyrgSingleChar(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgSingleChar [inline]");

		uint sp60 = ctx.push();
		bool rt59 = false;
		// condition: first part
		do
		{
			if (not (ctx.matchSingleAsciiChar('\\'))) // ?
				break;
			uint sp61 = ctx.push(); // 0 or 1
			if (not (yyrgCharExtended(ctx))) // ?
			{
				assert(sp61 < ctx.offset and "invalid offset match min == 1");
				ctx.offset = sp61;
				break;
			}
			rt59 = true;
		}
		while (false);
		// condition: second part
		while (not rt59)
		{
			ctx.restart(sp60);
			if (not (ctx.notMatchOneOf(datatext62 /* \"\n */))) // ?
				break;
			rt59 = true;
			break;
		}

		if (not rt59)
			return false;

		return true;
	}


	static const AnyString datatext65(" \t\r\n", 4);



	//! Rule sp
	static inline bool yyrgSp(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgSp [inline]");

		uint sp64 = ctx.push();
		bool rt63 = false;
		// condition: first part
		do
		{
			if (not (ctx.matchOneOf(datatext65 /*  \t\r\n */))) // ?
				break;
			rt63 = true;
		}
		while (false);
		// condition: second part
		while (not rt63)
		{
			ctx.restart(sp64);

			{
				// not allowed to eat :(
				OffsetAutoReset autoreset(ctx);

				if (not (yyrgPunc(ctx))) // ?
					break;
				}
			rt63 = true;
			break;
		}

		if (not rt63)
			return false;
		ctx.push(); // 0 or 1
		if (not (yyrgWp(ctx))) // ?
			return false;

		return true;
	}




	static inline bool __helper67(Datasource& ctx);
	static inline bool __helper68(Datasource& ctx);


	static inline bool __helper67(Datasource& ctx)
	{
		TRACE("    :: entering helper 67");
		ctx.push(); // 0 or 1
		if (not (yyrgWp(ctx))) // ?
			return false;
		return true;
	}

	static inline bool __helper68(Datasource& ctx)
	{
		TRACE("    :: entering helper 68");
		ctx.push(); // 0 or 1
		if (not (yyrgValue(ctx))) // ?
			return false;
		return true;
	}

	//! Rule start
	static inline bool yyrgStart(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgStart");
		uint32_t _ruleOffset = ctx.enterRule(rgStart);

		do // 0 or more
		{
			uint sp70 = ctx.push(); // 0 or 1
			if (not (__helper67(ctx) and __helper68(ctx)))
			{
				assert(sp70 < ctx.offset and "invalid offset match *");
				ctx.offset = sp70;
				break;
			}
		}
		while (true);
		ctx.push(); // 0 or 1
		if (not (yyrgWp(ctx))) // ?
			return false;

		ctx.commit(_ruleOffset, rgStart);
		return true;
	}


	//! Rule string
	static inline bool yyrgString(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgString");
		uint32_t _ruleOffset = ctx.enterRule(rgString);

		ctx.push(); // 0 or 1
		if (not (yyrgTkDoubleQuote(ctx))) // ?
			return false;
		uint sp72 = ctx.push();
		bool rt71 = false;
		// condition: first part
		do
		{
			uint sp73 = ctx.push(); // 0 or 1
			if (not (yyrgStringLiteral(ctx))) // ?
			{
				assert(sp73 < ctx.offset and "invalid offset match min == 1");
				ctx.offset = sp73;
				break;
			}
			uint sp74 = ctx.push(); // 0 or 1
			if (not (yyrgTkDoubleQuote(ctx))) // ?
			{
				assert(sp74 < ctx.offset and "invalid offset match min == 1");
				ctx.offset = sp74;
				break;
			}
			rt71 = true;
		}
		while (false);
		// condition: second part
		while (not rt71)
		{
			ctx.restart(sp72);
			uint sp75 = ctx.push(); // 0 or 1
			if (not (yyrgErrorEol(ctx))) // ?
			{
				assert(sp75 < ctx.offset and "invalid offset match min == 1");
				ctx.offset = sp75;
				break;
			}
			rt71 = true;
			break;
		}

		if (not rt71)
			return false;

		ctx.commit(_ruleOffset, rgString);
		return true;
	}


	//! Rule string-literal
	static inline bool yyrgStringLiteral(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgStringLiteral");
		uint32_t _ruleOffset = ctx.enterRule(rgStringLiteral);

		do // 0 or more
		{
			uint sp76 = ctx.push(); // 0 or 1
			if (not (yyrgSingleChar(ctx)))
			{
				assert(sp76 < ctx.offset and "invalid offset match *");
				ctx.offset = sp76;
				break;
			}
		}
		while (true);

		ctx.commit(_ruleOffset, rgStringLiteral);
		return true;
	}


	//! Rule tk-brace-close
	static inline bool yyrgTkBraceClose(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgTkBraceClose");
		uint32_t _ruleOffset = ctx.enterRule(rgTkBraceClose);

		if (not (ctx.matchSingleAsciiChar('}'))) // ?
			return false;

		ctx.commit(_ruleOffset, rgTkBraceClose);
		return true;
	}


	//! Rule tk-brace-open
	static inline bool yyrgTkBraceOpen(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgTkBraceOpen");
		uint32_t _ruleOffset = ctx.enterRule(rgTkBraceOpen);

		if (not (ctx.matchSingleAsciiChar('{'))) // ?
			return false;

		ctx.commit(_ruleOffset, rgTkBraceOpen);
		return true;
	}


	//! Rule tk-bracket-close
	static inline bool yyrgTkBracketClose(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgTkBracketClose");
		uint32_t _ruleOffset = ctx.enterRule(rgTkBracketClose);

		if (not (ctx.matchSingleAsciiChar(']'))) // ?
			return false;

		ctx.commit(_ruleOffset, rgTkBracketClose);
		return true;
	}


	//! Rule tk-bracket-open
	static inline bool yyrgTkBracketOpen(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgTkBracketOpen");
		uint32_t _ruleOffset = ctx.enterRule(rgTkBracketOpen);

		if (not (ctx.matchSingleAsciiChar('['))) // ?
			return false;

		ctx.commit(_ruleOffset, rgTkBracketOpen);
		return true;
	}


	//! Rule tk-colon
	static inline bool yyrgTkColon(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgTkColon");
		uint32_t _ruleOffset = ctx.enterRule(rgTkColon);

		if (not (ctx.matchSingleAsciiChar(':'))) // ?
			return false;

		ctx.commit(_ruleOffset, rgTkColon);
		return true;
	}


	//! Rule tk-comma
	static inline bool yyrgTkComma(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgTkComma");
		uint32_t _ruleOffset = ctx.enterRule(rgTkComma);

		if (not (ctx.matchSingleAsciiChar(','))) // ?
			return false;

		ctx.commit(_ruleOffset, rgTkComma);
		return true;
	}


	//! Rule tk-comment-block
	static inline bool yyrgTkCommentBlock(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgTkCommentBlock");
		uint32_t _ruleOffset = ctx.enterRule(rgTkCommentBlock);

		ctx.push(); // 0 or 1
		if (not (yyrgTkCommentStartBlock(ctx))) // ?
			return false;
		ctx.push(); // 0 or 1
		if (not (yyrgTkCommentBlockContent(ctx))) // ?
			return false;
		ctx.push(); // 0 or 1
		if (not (yyrgTkCommentEndBlock(ctx))) // ?
			return false;

		ctx.commit(_ruleOffset, rgTkCommentBlock);
		return true;
	}


	//! Rule tk-comment-block-content
	static inline bool yyrgTkCommentBlockContent(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgTkCommentBlockContent");
		uint32_t _ruleOffset = ctx.enterRule(rgTkCommentBlockContent);

		ctx.push(); // 0 or 1
		if (not (yyrgTkCommentSubbk(ctx))) // ?
			return false;

		ctx.commit(_ruleOffset, rgTkCommentBlockContent);
		return true;
	}


	static const AnyString datatext77("*/", 2);



	//! Rule tk-comment-end-block
	static inline bool yyrgTkCommentEndBlock(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgTkCommentEndBlock");
		uint32_t _ruleOffset = ctx.enterRule(rgTkCommentEndBlock);

		if (not (ctx.matchString(datatext77 /* * /  */))) // ?
			return false;

		ctx.commit(_ruleOffset, rgTkCommentEndBlock);
		return true;
	}


	//! Rule tk-comment-line
	static inline bool yyrgTkCommentLine(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgTkCommentLine");
		uint32_t _ruleOffset = ctx.enterRule(rgTkCommentLine);

		ctx.push(); // 0 or 1
		if (not (yyrgTkCommentStartInline(ctx))) // ?
			return false;
		ctx.push(); // 0 or 1
		if (not (yyrgTkCommentLineContent(ctx))) // ?
			return false;

		ctx.commit(_ruleOffset, rgTkCommentLine);
		return true;
	}


	static const AnyString datatext78("\n\r", 2);



	//! Rule tk-comment-line-content
	static inline bool yyrgTkCommentLineContent(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgTkCommentLineContent");
		uint32_t _ruleOffset = ctx.enterRule(rgTkCommentLineContent);

		while (ctx.notMatchOneOf(datatext78 /* \n\r */)) // 0-1 or more
		{}

		ctx.commit(_ruleOffset, rgTkCommentLineContent);
		return true;
	}


	static const AnyString datatext79("/*", 2);



	//! Rule tk-comment-start-block
	static inline bool yyrgTkCommentStartBlock(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgTkCommentStartBlock");
		uint32_t _ruleOffset = ctx.enterRule(rgTkCommentStartBlock);

		if (not (ctx.matchString(datatext79 /*  / * */))) // ?
			return false;

		ctx.commit(_ruleOffset, rgTkCommentStartBlock);
		return true;
	}


	static const AnyString datatext80("//", 2);



	//! Rule tk-comment-start-inline
	static inline bool yyrgTkCommentStartInline(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgTkCommentStartInline");
		uint32_t _ruleOffset = ctx.enterRule(rgTkCommentStartInline);

		if (not (ctx.matchString(datatext80 /*  /  /  */))) // ?
			return false;

		ctx.commit(_ruleOffset, rgTkCommentStartInline);
		return true;
	}


	static const AnyString datatext88("*/", 2);



	//! Rule tk-comment-subbk
	static inline bool yyrgTkCommentSubbk(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgTkCommentSubbk [inline]");

		uint sp82 = ctx.push();
		bool rt81 = false;
		// condition: first part
		do
		{
			uint sp84 = ctx.push();
			bool rt83 = false;
			// condition: first part
			do
			{
				uint sp85 = ctx.push(); // 0 or 1
				if (not (yyrgTkCommentBlock(ctx))) // ?
				{
					assert(sp85 < ctx.offset and "invalid offset match min == 1");
					ctx.offset = sp85;
					break;
				}
				uint sp86 = ctx.push(); // 0 or 1
				if (not (yyrgTkCommentSubbk(ctx))) // ?
				{
					assert(sp86 < ctx.offset and "invalid offset match min == 1");
					ctx.offset = sp86;
					break;
				}
				rt83 = true;
			}
			while (false);
			// condition: second part
			while (not rt83)
			{
				ctx.restart(sp84);

				{
					// not allowed to eat :(
					OffsetAutoReset autoreset(ctx);

					if (not (yyrgTkCommentEndBlock(ctx))) // ?
						break;
					}
				rt83 = true;
				break;
			}

			if (not rt83)
			{
				assert(sp84 < ctx.offset and "invalid offset if");
				ctx.offset = sp84; // restore context before if
				break;
			}
			rt81 = true;
		}
		while (false);
		// condition: second part
		while (not rt81)
		{
			ctx.restart(sp82);
			if (not (ctx.notMatchOneOf(""))) // ?
				break;
			while (ctx.notMatchOneOf(datatext88 /* * /  */)) // 0-1 or more
			{}
			uint sp89 = ctx.push(); // 0 or 1
			if (not (yyrgTkCommentSubbk(ctx))) // ?
			{
				assert(sp89 < ctx.offset and "invalid offset match min == 1");
				ctx.offset = sp89;
				break;
			}
			rt81 = true;
			break;
		}

		if (not rt81)
			return false;

		return true;
	}


	//! Rule tk-dot
	static inline bool yyrgTkDot(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgTkDot");
		uint32_t _ruleOffset = ctx.enterRule(rgTkDot);

		if (not (ctx.matchSingleAsciiChar('.'))) // ?
			return false;

		ctx.commit(_ruleOffset, rgTkDot);
		return true;
	}


	//! Rule tk-double-quote
	static inline bool yyrgTkDoubleQuote(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgTkDoubleQuote");
		uint32_t _ruleOffset = ctx.enterRule(rgTkDoubleQuote);

		if (not (ctx.matchSingleAsciiChar('"'))) // ?
			return false;

		ctx.commit(_ruleOffset, rgTkDoubleQuote);
		return true;
	}


	static const AnyString datatext90("true", 4);



	//! Rule true
	static inline bool yyrgTrue(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgTrue");
		uint32_t _ruleOffset = ctx.enterRule(rgTrue);

		if (not (ctx.matchString(datatext90 /* true */))) // ?
			return false;

		ctx.commit(_ruleOffset, rgTrue);
		return true;
	}


	//! Rule value
	static inline bool yyrgValue(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgValue");
		uint32_t _ruleOffset = ctx.enterRule(rgValue);

		uint sp92 = ctx.push();
		bool rt91 = false;
		// condition: first part
		do
		{
			uint sp94 = ctx.push();
			bool rt93 = false;
			// condition: first part
			do
			{
				uint sp96 = ctx.push();
				bool rt95 = false;
				// condition: first part
				do
				{
					uint sp98 = ctx.push();
					bool rt97 = false;
					// condition: first part
					do
					{
						uint sp100 = ctx.push();
						bool rt99 = false;
						// condition: first part
						do
						{
							uint sp102 = ctx.push();
							bool rt101 = false;
							// condition: first part
							do
							{
								uint sp103 = ctx.push(); // 0 or 1
								if (not (yyrgString(ctx))) // ?
								{
									assert(sp103 < ctx.offset and "invalid offset match min == 1");
									ctx.offset = sp103;
									break;
								}
								rt101 = true;
							}
							while (false);
							// condition: second part
							while (not rt101)
							{
								ctx.restart(sp102);
								uint sp104 = ctx.push(); // 0 or 1
								if (not (yyrgObject(ctx))) // ?
								{
									assert(sp104 < ctx.offset and "invalid offset match min == 1");
									ctx.offset = sp104;
									break;
								}
								rt101 = true;
								break;
							}

							if (not rt101)
							{
								assert(sp102 < ctx.offset and "invalid offset if");
								ctx.offset = sp102; // restore context before if
								break;
							}
							rt99 = true;
						}
						while (false);
						// condition: second part
						while (not rt99)
						{
							ctx.restart(sp100);
							uint sp105 = ctx.push(); // 0 or 1
							if (not (yyrgArray(ctx))) // ?
							{
								assert(sp105 < ctx.offset and "invalid offset match min == 1");
								ctx.offset = sp105;
								break;
							}
							rt99 = true;
							break;
						}

						if (not rt99)
						{
							assert(sp100 < ctx.offset and "invalid offset if");
							ctx.offset = sp100; // restore context before if
							break;
						}
						rt97 = true;
					}
					while (false);
					// condition: second part
					while (not rt97)
					{
						ctx.restart(sp98);
						uint sp106 = ctx.push(); // 0 or 1
						if (not (yyrgNumberDef(ctx))) // ?
						{
							assert(sp106 < ctx.offset and "invalid offset match min == 1");
							ctx.offset = sp106;
							break;
						}
						rt97 = true;
						break;
					}

					if (not rt97)
					{
						assert(sp98 < ctx.offset and "invalid offset if");
						ctx.offset = sp98; // restore context before if
						break;
					}
					rt95 = true;
				}
				while (false);
				// condition: second part
				while (not rt95)
				{
					ctx.restart(sp96);
					uint sp107 = ctx.push(); // 0 or 1
					if (not (yyrgTrue(ctx))) // ?
					{
						assert(sp107 < ctx.offset and "invalid offset match min == 1");
						ctx.offset = sp107;
						break;
					}
					rt95 = true;
					break;
				}

				if (not rt95)
				{
					assert(sp96 < ctx.offset and "invalid offset if");
					ctx.offset = sp96; // restore context before if
					break;
				}
				rt93 = true;
			}
			while (false);
			// condition: second part
			while (not rt93)
			{
				ctx.restart(sp94);
				uint sp108 = ctx.push(); // 0 or 1
				if (not (yyrgFalse(ctx))) // ?
				{
					assert(sp108 < ctx.offset and "invalid offset match min == 1");
					ctx.offset = sp108;
					break;
				}
				rt93 = true;
				break;
			}

			if (not rt93)
			{
				assert(sp94 < ctx.offset and "invalid offset if");
				ctx.offset = sp94; // restore context before if
				break;
			}
			rt91 = true;
		}
		while (false);
		// condition: second part
		while (not rt91)
		{
			ctx.restart(sp92);
			uint sp109 = ctx.push(); // 0 or 1
			if (not (yyrgNull(ctx))) // ?
			{
				assert(sp109 < ctx.offset and "invalid offset match min == 1");
				ctx.offset = sp109;
				break;
			}
			rt91 = true;
			break;
		}

		if (not rt91)
			return false;

		ctx.commit(_ruleOffset, rgValue);
		return true;
	}


	static const AnyString datatext110(" \t\n\r", 4);
	static const AnyString datatext118(" \t\n\r", 4);





	static inline bool __helper111(Datasource& ctx);
	static inline bool __helper112(Datasource& ctx);


	static inline bool __helper111(Datasource& ctx)
	{
		TRACE("    :: entering helper 111");
		uint sp115 = ctx.push();
		bool rt114 = false;
		// condition: first part
		do
		{
			uint sp116 = ctx.push(); // 0 or 1
			if (not (yyrgTkCommentLine(ctx))) // ?
			{
				assert(sp116 < ctx.offset and "invalid offset match min == 1");
				ctx.offset = sp116;
				break;
			}
			rt114 = true;
		}
		while (false);
		// condition: second part
		while (not rt114)
		{
			ctx.restart(sp115);
			uint sp117 = ctx.push(); // 0 or 1
			if (not (yyrgTkCommentBlock(ctx))) // ?
			{
				assert(sp117 < ctx.offset and "invalid offset match min == 1");
				ctx.offset = sp117;
				break;
			}
			rt114 = true;
			break;
		}

		if (not rt114)
			return false;
		return true;
	}

	static inline bool __helper112(Datasource& ctx)
	{
		TRACE("    :: entering helper 112");
		while (ctx.matchOneOf(datatext118 /*  \t\n\r */)) // 0-1 or more
		{}
		return true;
	}

	//! Rule wp
	static inline bool yyrgWp(Datasource& ctx)
	{
		(void) ctx;
		TRACE("entering rgWp [inline]");

		while (ctx.matchOneOf(datatext110 /*  \t\n\r */)) // 0-1 or more
		{}
		do // 0 or more
		{
			uint sp119 = ctx.push(); // 0 or 1
			if (not (__helper111(ctx) and __helper112(ctx)))
			{
				assert(sp119 < ctx.offset and "invalid offset match *");
				ctx.offset = sp119;
				break;
			}
		}
		while (true);

		return true;
	}





} // anonymous namespace







	NodeVector::~NodeVector()
	{
		uint32_t size = m_size;
		for (uint32_t i = 0; i != size; ++i)
			deleteElement(m_pointer[i]);
		if (m_capacity != preAllocatedCount)
			free(m_pointer);
	}

	void NodeVector::shrink_to_fit() noexcept
	{
		if (m_capacity != preAllocatedCount)
		{
			if (m_size == 0)
			{
				free(m_pointer);
				m_pointer = &(m_innerstorage[0]);
				m_capacity = preAllocatedCount;
			}
		}
	}


	void NodeVector::push_front(NodeVector::T* const element)
	{
		assert(element != nullptr);
		uint32_t size = m_size;
		if (size == 0)
			push_back(element);

		uint32_t newsize = size + 1;
		if (not (newsize < m_capacity))
			grow();
		uint32_t i = size;
		while (i-- > 0)
			m_pointer[i + 1] = m_pointer[i];
		m_size = newsize;
		element->addRef();
		m_pointer[0] = element;
	}


	void NodeVector::resize(uint32_t count)
	{
		uint32_t size = m_size;
		if (count != size)
		{
			if (count > size)
			{
				for (uint32_t i = size; i != count; ++i)
					push_back(new T);
			}
			else
			{
				for (uint32_t i = count; i != size; ++i)
					deleteElement(m_pointer[i]);
			}
			m_size = count;
		}
	}


	void NodeVector::erase(uint32_t index) noexcept
	{
		uint32_t size = m_size;
		if (index < size)
		{
			deleteElement(m_pointer[index]);
			for (uint32_t i = index + 1; i < size; ++i)
				m_pointer[i - 1] = m_pointer[i];
			m_size = --size;
		}
	}


	void NodeVector::clear() noexcept
	{
		uint32_t size = m_size;
		if (size)
		{
			for (uint32_t i = 0; i != size; ++i)
				deleteElement(m_pointer[i]);
			m_size = 0;
		}
	}


	void NodeVector::pop_back() noexcept
	{
		uint32_t size = m_size;
		if (size != 0)
		{
			--size;
			deleteElement(m_pointer[size]);
			m_size = size;
		}
	}


	void NodeVector::push_back(NodeVector::T* const element)
	{
		assert(element != nullptr);
		uint32_t oldsize = m_size;
		uint32_t newsize = oldsize + 1;
		if (not (newsize < m_capacity))
			grow();
		m_size = newsize;
		element->addRef();
		m_pointer[oldsize] = element;
	}


	void NodeVector::grow()
	{
		uint32_t newcapacity;
		T** newptr;
		if (m_capacity == preAllocatedCount)
		{
			newcapacity = 8;
			newptr = (T**) malloc(sizeof(T*) * newcapacity);
			if (!newptr)
			{
				#if __cpp_exceptions >= 199711
				throw std::bad_alloc();
				#else
				abort();
				#endif
			}
			newptr[0] = m_innerstorage[0];
			newptr[1] = m_innerstorage[1];
		}
		else
		{
			newcapacity = m_capacity + 8;
			newptr = (T**) realloc(m_pointer, sizeof(T*) * newcapacity);
			if (!newptr)
			{
				#if __cpp_exceptions >= 199711
				throw std::bad_alloc();
				#else
				abort();
				#endif
			}
		}
		m_capacity = newcapacity;
		m_pointer = newptr;
	}




	AnyString ruleToString(Rule ruleid)
	{
		switch (ruleid)
		{
			case rgUnknown:
				return "<unknown>";
			case rgArray:
				return "array";
			case rgCharExtended:
				return "char-extended";
			case rgDigit:
				return "digit";
			case rgError:
				return "error";
			case rgErrorBracket:
				return "error-bracket";
			case rgErrorEol:
				return "error-eol";
			case rgErrorParenthese:
				return "error-parenthese";
			case rgErrorSemicolon:
				return "error-semicolon";
			case rgFalse:
				return "false";
			case rgHex:
				return "hex";
			case rgInteger:
				return "integer";
			case rgKey:
				return "key";
			case rgNull:
				return "null";
			case rgNumber:
				return "number";
			case rgNumberDef:
				return "number-def";
			case rgNumberExponent:
				return "number-exponent";
			case rgNumberHexa:
				return "number-hexa";
			case rgNumberQualifier:
				return "number-qualifier";
			case rgNumberQualifierType:
				return "number-qualifier-type";
			case rgNumberSign:
				return "number-sign";
			case rgNumberValue:
				return "number-value";
			case rgObject:
				return "object";
			case rgObjectEntry:
				return "object-entry";
			case rgPunc:
				return "punc";
			case rgSingleChar:
				return "single-char";
			case rgSp:
				return "sp";
			case rgStart:
				return "start";
			case rgString:
				return "string";
			case rgStringLiteral:
				return "string-literal";
			case rgTkBraceClose:
				return "tk-brace-close";
			case rgTkBraceOpen:
				return "tk-brace-open";
			case rgTkBracketClose:
				return "tk-bracket-close";
			case rgTkBracketOpen:
				return "tk-bracket-open";
			case rgTkColon:
				return "tk-colon";
			case rgTkComma:
				return "tk-comma";
			case rgTkCommentBlock:
				return "tk-comment-block";
			case rgTkCommentBlockContent:
				return "tk-comment-block-content";
			case rgTkCommentEndBlock:
				return "tk-comment-end-block";
			case rgTkCommentLine:
				return "tk-comment-line";
			case rgTkCommentLineContent:
				return "tk-comment-line-content";
			case rgTkCommentStartBlock:
				return "tk-comment-start-block";
			case rgTkCommentStartInline:
				return "tk-comment-start-inline";
			case rgTkCommentSubbk:
				return "tk-comment-subbk";
			case rgTkDot:
				return "tk-dot";
			case rgTkDoubleQuote:
				return "tk-double-quote";
			case rgTrue:
				return "true";
			case rgValue:
				return "value";
			case rgWp:
				return "wp";
			case rgEOF:
				return "EOF";
		}
		return "<error>";
	}




	Parser::Parser()
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
		switch (ctx.open(filename))
		{
			case Datasource::OpenFlag::opened: {
				DATASOURCE_PARSE(ctx);
				return ctx.success;
			}
			case Datasource::OpenFlag::ignore: {
				return true;			}
			case Datasource::OpenFlag::error: {
				return false;			}
		}
		return false;
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


	void Parser::translateOffset(uint& column, uint& line, uint32_t offset) const
	{
		column = 0;
		line = 0;
		if (YUNI_LIKELY(pData))
		{
			Datasource& ctx = *((Datasource*) pData);
			ctx.translateOffset(column, line, offset);
		}
	}


	uint32_t Parser::translateOffsetToLine(const Node& node) const
	{
		uint32_t column;
		uint32_t line;
		translateOffset(column, line, node);
		return line;
	}













	void Node::ExportToHTML(Clob& out, const Node& node)
	{
		String tmp;
		String indent;
		InternalNodeExportHTML(out, node, indent, tmp);
	}


	void Node::Export(Clob& out, const Node& node, bool color, ExportCallback callback)
	{
		String tmp;
		String indent;
		if (not color)
			InternalNodeExportConsole<false>(out, node, false, indent, tmp, callback);
		else
			InternalNodeExportConsole<true>(out, node, false, indent, tmp, callback);
	}


	void Node::ExportToJSON(Yuni::Clob& out, const Node& node, void (*callback)(Yuni::Dictionary<AnyString, YString>::Unordered&, const Node&))
	{
		String tmp;
		String indent;
		out << "{ \"data\": [\n";
		InternalNodeExportJSON(out, node, false, indent, tmp, callback);
		out << "	{}\n";
		out << "] }\n";
	}


	void Node::ExportToJSON(Yuni::Clob& out, const Node& node)
	{
		ExportToJSON(out, node, nullptr);
	}


	void Node::Export(Clob& out, const Node& node)
	{
		Export(out, node, ::Yuni::System::Console::IsStdoutTTY());
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


	void Node::toText(YString& out) const
	{
		if (not text.empty())
		{
			if (not out.empty())
				out += ' ';
			out += text;
			out.trimRight();
		}
		for (auto& child: children)
			child.toText(out);
	}


	void Node::clear()
	{
		children.clear();
		text.clear();
		offset = 0;
		offsetEnd = 0;
		rule = rgUnknown;
	}









} // namespace JSON
} // namespace Demo
