
#include "calculator-grammar.h"
#include <cassert>
#include <yuni/core/string.h>
#include <yuni/io/file.h>
#include <yuni/io/directory.h>
#include <yuni/core/dictionary.h>

using namespace Yuni;


namespace Demo
{
namespace Calculator
{
namespace // anonymous
{


	//! A single instruction type (its size in bits is based on the number of rules)
	typedef ::Yuni::uint8  Instruction;  // [uint8, uint16, uint32, uint64]





	// Forward declarations
	template<class ContextT> static bool yyrgExpr(ContextT& ctx);
	template<class ContextT> static bool yyrgExprAtom(ContextT& ctx);
	template<class ContextT> static bool yyrgExprProduct(ContextT& ctx);
	template<class ContextT> static bool yyrgExprSum(ContextT& ctx);
	template<class ContextT> static bool yyrgNumber(ContextT& ctx);
	template<class ContextT> static bool yyrgStart(ContextT& ctx);
	template<class ContextT> static bool yyrgWp(ContextT& ctx);




	# define TRACE(X)  std::cout << "  :: " << ctx.urls[ctx.stack[ctx.offset].urlindex] \
		<< ", depth:" << ctx.offset << ": " << X << std::endl

	# define TRACE_LOCAL(X)  std::cout << "  :: " << urls[stack[offset].urlindex] \
		<< ", depth:" << offset << ": " << X << std::endl


	struct Chunk
	{
		//! Iterator on the source
		uint offset;
		//! Index of the current source url (see `urls`)
		uint urlindex;
	};



	class Datasource final
	{
	public:

	public:
		//! \name Constructors
		//@{
		//! Default constructor
		Datasource();
		~Datasource();
		//@}

		//! \name Matching
		//@{
		bool matchSingleAsciiChar(char);
		bool matchString(const AnyString& text, uint ut8Count);
		bool matchOneOf(const AnyString& text, uint utf8count);
		//@}

		//! \name Chunk
		//@{
		void pushInclude(uint urlindex);
		uint push();
		void restart(uint from);
		void pop();
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

	public:
		//! Stack
		Chunk* stack;
		uint offset;
		uint capacity;

		//! Root folder
		String root;
		//! File Content of all encountered urls, ordered by their order of arrival
		Clob::Vector contents;
		//! Reverse mapping for retriving the index of a known file
		::Yuni::Dictionary<AnyString, uint>::Unordered  reverseUrlIndexes;
		//! All urls
		::Yuni::String::Vector urls;

	}; // class Datasource



	inline Datasource::Datasource() :
		offset(0),
		capacity(512)
	{
		stack = (Chunk*)::malloc(sizeof(Chunk) * capacity);
		stack[0].offset   = (uint) -1; // EOF
		stack[0].urlindex = 0; // the first one will be good enough
	}


	inline Datasource::~Datasource()
	{
		::free(stack);
	}


	inline void Datasource::pushInclude(uint urlindex)
	{
		if (not (++offset < capacity))
		{
			// Grow !
			capacity += 512;
			stack = (Chunk*)::realloc(stack, capacity * sizeof(Chunk));
		}
		stack[offset].urlindex = urlindex;
		stack[offset].offset = 0;
	}


	inline uint Datasource::push()
	{
		if (not (++offset < capacity))
		{
			// Grow !
			capacity += 512;
			stack = (Chunk*)::realloc(stack, capacity * sizeof(Chunk));
		}
		stack[offset] = stack[offset - 1];
		TRACE_LOCAL("    push at offset " << stack[offset].offset);
		return offset - 1;
	}


	inline void Datasource::restart(uint from)
	{
		assert(from + 1 < capacity);
		offset = from + 1;
		stack[offset] = stack[offset - 1];
		TRACE_LOCAL("    restart at offset " << stack[offset].offset);
	}


	inline void Datasource::pop()
	{
		assert(offset > 0 and "internal error on offset");
		--offset;
		TRACE_LOCAL("    pop to offset " << stack[offset].offset);
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
		Yuni::Dictionary<AnyString, uint>::Unordered::const_iterator knownIndex = reverseUrlIndexes.find(filename);
		if (YUNI_LIKELY(knownIndex == reverseUrlIndexes.end()))
		{
			// indexes
			index = (uint) contents.size();
			reverseUrlIndexes[filename] = index;
			// load the entire content in memory
			contents.push_back(nullptr);
			urls.push_back(filename);
			::Yuni::IO::File::LoadFromFile(contents.back(), filename);
		}
		else
			index = knownIndex->second;

		// new item in the stack
		pushInclude(index);
		return true;
	}


	inline void Datasource::openContent(const AnyString& content)
	{
		String filename = "<memory>";
		// filename index
		uint index;
		::Yuni::Dictionary<AnyString, uint>::Unordered::const_iterator knownIndex = reverseUrlIndexes.find(filename);
		if (YUNI_LIKELY(knownIndex == reverseUrlIndexes.end()))
		{
			// indexes
			index = (uint) contents.size();
			reverseUrlIndexes[filename] = index;
			// load the entire content in memory
			contents.push_back(nullptr);
			urls.push_back(filename);
			contents.back() = content;
		}
		else
			index = knownIndex->second;

		pushInclude(index);
	}


	inline bool Datasource::matchSingleAsciiChar(char c)
	{
		Chunk& cursor = stack[offset];
		Clob&  data   = contents[cursor.urlindex];
		if (cursor.offset < data.size() and c == data[cursor.offset])
		{
			++cursor.offset;
			return true;
		}
		return false;
	}


	inline bool Datasource::matchString(const AnyString& text, uint ut8Count)
	{
		Chunk& cursor = stack[offset];
		Clob&  data   = contents[cursor.urlindex];

		if (AnyString(data, cursor.offset).startsWith(text))
		{
			cursor.offset += text.size();
			return true;
		}
		return false;
	}


	inline bool Datasource::matchOneOf(const AnyString& text, uint utf8count)
	{
		Chunk& cursor = stack[offset];
		Clob&  data   = contents[cursor.urlindex];

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






	//! Rule expr
	template<class ContextT>
	static inline bool yyrgExpr(ContextT& ctx)
	{
		TRACE("entering rgExpr");
		if (not yyrgExprSum(ctx))
			return false;
		return true;
	}


	//! Rule expr-atom
	template<class ContextT>
	static inline bool yyrgExprAtom(ContextT& ctx)
	{
		TRACE("entering rgExprAtom");
		uint sp2 = ctx.push();
		bool rt1 = false;
		do
		{
			if (not ctx.matchSingleAsciiChar('('))
				break;
			if (not yyrgExpr(ctx))
				break;
			if (not ctx.matchSingleAsciiChar(')'))
				break;
			rt1 = true;
		}
		while (false);
		while (not rt1)
		{
			ctx.restart(sp2);
			if (not yyrgNumber(ctx))
				break;
			rt1 = true;
			break;
		}

		if (not rt1)
		{
			ctx.offset = sp2; // restore context before if
			return false;
		}
		return true;
	}




	template<class ContextT>
	static inline bool __helper3(ContextT& ctx)
	{
		if (not ctx.matchOneOf(AnyString("*/", 2), 2))
			return false;
		return true;
	}

	template<class ContextT>
	static inline bool __helper4(ContextT& ctx)
	{
		if (not yyrgExprAtom(ctx))
			return false;
		return true;
	}



	//! Rule expr-product
	template<class ContextT>
	static inline bool yyrgExprProduct(ContextT& ctx)
	{
		TRACE("entering rgExprProduct");
		if (not yyrgExprAtom(ctx))
			return false;
		while (__helper3(ctx) and __helper4(ctx))
		{
		}
		return true;
	}




	template<class ContextT>
	static inline bool __helper6(ContextT& ctx)
	{
		if (not ctx.matchOneOf(AnyString("+-", 2), 2))
			return false;
		return true;
	}

	template<class ContextT>
	static inline bool __helper7(ContextT& ctx)
	{
		if (not yyrgExprProduct(ctx))
			return false;
		return true;
	}



	//! Rule expr-sum
	template<class ContextT>
	static inline bool yyrgExprSum(ContextT& ctx)
	{
		TRACE("entering rgExprSum");
		if (not yyrgExprProduct(ctx))
			return false;
		while (__helper6(ctx) and __helper7(ctx))
		{}
		return true;
	}




	template<class ContextT>
	static inline bool __helper9(ContextT& ctx)
	{
		if (not ctx.matchSingleAsciiChar('.'))
			return false;
		return true;
	}

	template<class ContextT>
	static inline bool __helper10(ContextT& ctx)
	{
		if (not ctx.matchOneOf(AnyString("0123456789", 10), 10))
			return false;
		while (not ctx.matchOneOf(AnyString("0123456789", 10), 10))
		{}
		return true;
	}



	//! Rule number
	template<class ContextT>
	static inline bool yyrgNumber(ContextT& ctx)
	{
		TRACE("entering rgNumber");
		if (not ctx.matchOneOf(AnyString("0123456789", 10), 10))
			return false;
		while (not ctx.matchOneOf(AnyString("0123456789", 10), 10))
		{}
		if (__helper9(ctx) and __helper10(ctx))
			{} // exec stmt - don't care of the result
		return true;
	}


	//! Rule start
	template<class ContextT>
	static inline bool yyrgStart(ContextT& ctx)
	{
		TRACE("entering rgStart");
		if (not yyrgExpr(ctx))
			return false;
		return true;
	}


	//! Rule wp
	template<class ContextT>
	static inline bool yyrgWp(ContextT& ctx)
	{
		TRACE("entering rgWp");
		while (not ctx.matchOneOf(AnyString(" \t\n\r", 4), 4))
		{}
		return true;
	}





} // anonymous namespace



	Parser::Parser()
	{
		onURILoading.bind(& StandardURILoaderHandler);
	}


	void Parser::loadFromFile(const AnyString& filename)
	{
		Datasource ctx;
		ctx.open(filename);
		bool ok = yyrgStart(ctx);
		TRACE("result: " << (ok ? "OK" : "FAILED"));
	}


	void Parser::load(const AnyString& content)
	{
		Datasource ctx;
		ctx.openContent(content);
		bool ok = yyrgStart(ctx);
		TRACE("result: " << (ok ? "OK" : "FAILED"));
	}







} // namespace Calculator
} // namespace Demo
