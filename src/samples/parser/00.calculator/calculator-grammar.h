#ifndef __HEADER_DEMO_CALCULATOR_GRAMMAR_H__
# define __HEADER_DEMO_CALCULATOR_GRAMMAR_H__

# include <yuni/yuni.h>
# include <yuni/core/string.h>
# include <yuni/core/bind.h>
# include "calculator-grammar.nodes.h"


namespace Demo
{
namespace Calculator
{


	class Parser final
	{
	public:
		typedef Yuni::Bind<bool (Yuni::Clob& out, const AnyString& uri)>   OnURILoading;
		typedef Yuni::Bind<bool (const AnyString& filename, uint line, uint offset, Error, const YString::Vector&)>  OnError;

	public:
		Parser();
		~Parser() {}

		void loadFromFile(const AnyString& filename);
		void load(const AnyString& content);

	public:
		//! Event: load another include file
		OnURILoading onURILoading;
		//! Event: load another include file
		OnError  onError;

	}; // class Parser





} // namespace Calculator
} // namespace Demo

# include "calculator-grammar.hxx"

#endif // __HEADER_DEMO_CALCULATOR_GRAMMAR_H__
