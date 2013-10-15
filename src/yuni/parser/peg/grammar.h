#ifndef __YUNI_PARSER_GENERATOR_GRAMMAR_H__
# define __YUNI_PARSER_GENERATOR_GRAMMAR_H__

# include "../../yuni.h"
# include "../../core/string.h"
# include "../../core/noncopyable.h"
# include "../../core/event/event.h"
# include "node.h"
# include <iosfwd>


namespace Yuni
{
namespace Parser
{
namespace PEG
{


	class Grammar final : private NonCopyable<Grammar>
	{
	public:
		//! \name Constructor & Destructor
		//@{
		//! Default constructor
		Grammar();
		//! Destructor
		~Grammar();
		//@}


		//! clear
		void clear();

		//! Load a grammar file
		bool loadFromFile(const AnyString& filename);

		//! Load a grammar from a string content
		bool loadFromString(const AnyString& content);

		//! Export as DOT file
		void exportToDOT(Clob& out) const;
		//! Export to C++
		bool exportToCPP(const AnyString& rootfilename, const AnyString& name) const;

		//! print the whole grammar to cout
		void print(std::ostream& out) const;


	public:
		//! Event: warning
		Event<void (const AnyString&)> onWarning;
		//! Event: error
		Event<void (const AnyString&)> onError;

	private:
		bool loadFromData(const AnyString& content, const AnyString& source);

	private:
		//! Rules
		Node::Map pRules;

	}; // class Grammar





} // namespace PEG
} // namespace Parser
} // namespace Yuni

std::ostream& operator << (std::ostream& out, const Yuni::Parser::PEG::Grammar& grammar);

# include "grammar.hxx"

#endif // __YUNI_PARSER_GENERATOR_GRAMMAR_H__
