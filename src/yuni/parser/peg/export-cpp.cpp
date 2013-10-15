
#include "grammar.h"
#include "../../io/file.h"


namespace Yuni
{
namespace Parser
{
namespace PEG
{

	# include "../../private/parser/peg/__parser.include.cpp.hxx"


	namespace // anonymous
	{

		class CPPConverter
		{
		public:
			CPPConverter(const AnyString& root, const Node::Map& rules);
			bool initialize(const AnyString& name);
			void startHeaderheaderGuardID();
			void endHeaderheaderGuardID();
			void generateH();
			void generateHNodes();
			void generateHXX();
			void generateCPP();
			bool writeFiles() const;

		public:
			//! Original rules
			const Node::Map& rules;

			//! Code in the .h file
			Clob h;
			Clob hNodes;
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
			out[4] = (char) YString::ToUpper(out[4]);
		}


		static inline void SingleInstructionType(Clob& cpp, uint64 rulecount)
		{
			cpp << "\t//! A single instruction type (its size in bits is based on the number of rules)\n";
			cpp << "\ttypedef ::Yuni::";
			if (rulecount < 255)
				cpp << "uint8";
			else if (rulecount < 65535)
				cpp << "uint16";
			else if (rulecount < (uint32) -1)
				cpp << "uint32";
			else
				cpp << "uint64";

			cpp << "  Instruction;  // [uint8, uint16, uint32, uint64]\n\n\n";
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


		inline void CPPConverter::generateHNodes()
		{
			const Node::Map::const_iterator end = rules.end();
			hNodes << "#ifndef " << headerGuardID << "_NODES_H__\n";
			hNodes << "# define " << headerGuardID << "_NODES_H__\n";
			hNodes << "\n";
			hNodes << "# include <yuni/yuni.h>\n";
			hNodes << "# include <yuni/core/string.h>\n";
			hNodes << "\n\n";

			for (uint i = 0; i != namespaces.size(); ++i)
				hNodes << "namespace " << namespaces[i] << "\n{\n";
			hNodes << "\n\n";

			hNodes << "\tenum Rule\n";
			hNodes << "\t{\n";
			for (Node::Map::const_iterator i = rules.begin(); i != end; ++i)
			{
				hNodes << "\t\t//! enum for the rule `" << i->first << "`\n";
				hNodes << "\t\t" << i->second.enumID << ",\n";
			}

			hNodes << "\t\t//! enum for the final rule\n";
			hNodes << "\t\trgEOF\n";

			hNodes << "\t};\n";
			hNodes << '\n';


			// rule count
			hNodes << "\tenum\n";
			hNodes << "\t{\n";
			hNodes << "\t\t//! The total number of rules\n";
			hNodes << "\t\truleCount = " << (1 + rules.size()) << '\n';
			hNodes << "\t};\n";
			hNodes << "\n\n\n";

			hNodes << "\tenum Error\n";
			hNodes << "\t{\n";
			hNodes << "\t\terrNone,\n";
			hNodes << "\t};\n";
			hNodes << "\n\n\n";

			YString nodename;

			for (Node::Map::const_iterator i = rules.begin(); i != end; ++i)
			{
				EnumIDToClassname(nodename, i->second.enumID);

				hNodes << "\tclass " << nodename << " final\n";
				hNodes << "\t{\n";
				hNodes << "\tpublic:\n";
				hNodes << "\t\t//! rule enum id\n";
				hNodes << "\t\tstatic inline Rule rule()  { return " << i->second.enumID << "; }\n";
				hNodes << "\t};\n\n\n";
			}

			hNodes << "\n\n";
		}


		inline void CPPConverter::generateH()
		{
			h << "#ifndef " << headerGuardID << "_H__\n";
			h << "# define " << headerGuardID << "_H__\n";
			h << "\n";
			h << "# include <yuni/yuni.h>\n";
			h << "# include <yuni/core/string.h>\n";
			h << "# include <yuni/core/bind.h>\n";
			h << "# include \"" << localInclude << "nodes.h\"\n";
			h << "\n\n";

			for (uint i = 0; i != namespaces.size(); ++i)
				h << "namespace " << namespaces[i] << "\n{\n";
			h << "\n\n";

			h << "\tclass Parser final\n";
			h << "\t{\n";
			h << "\tpublic:\n";
			h << "\t\ttypedef Yuni::Bind<bool (Yuni::Clob& out, const AnyString& uri)>   OnURILoading;\n";
			h << "\t\ttypedef Yuni::Bind<bool (const AnyString& filename, uint line, uint offset, Error, const YString::Vector&)>  OnError;\n";
			h << '\n';
			h << "\tpublic:\n";
			h << "\t\tParser();\n";
			h << "\t\t~Parser() {}\n";
			h << '\n';
			h << "\t\tvoid loadFromFile(const AnyString& filename);\n";
			h << "\t\tvoid load(const AnyString& content);\n";
			h << '\n';
			h << "\tpublic:\n";
			h << "\t\t//! Event: load another include file\n";
			h << "\t\tOnURILoading onURILoading;\n";
			h << "\t\t//! Event: load another include file\n";
			h << "\t\tOnError  onError;\n";
			h << '\n';
			h << "\t}; // class Parser\n";
		}


		inline void CPPConverter::generateHXX()
		{
			hxx << "#ifndef " << headerGuardID << "_HXX__\n";
			hxx << "# define " << headerGuardID << "_HXX__\n";
			hxx << "\n\n\n";

			for (uint i = 0; i != namespaces.size(); ++i)
				hxx << "namespace " << namespaces[i] << "\n{\n";
			hxx << "\n\n";
		}


		static inline void GenerateFunctionForEachRule(Clob& cpp, uint& sp, const Node::Map& rules, const AnyString& name, const Node& node)
		{
			Clob body;
			Clob::Vector helpers;
			// skip the first AND block, which is useless
			if (node.rule.type == Node::asAND)
			{
				for (uint i = 0; i != node.children.size(); ++i)
					node.children[i].exportCPP(body, rules, helpers, 2, true, sp);
			}
			else
				node.exportCPP(body, rules, helpers, 2, true, sp);

			if (not helpers.empty())
			{
				cpp << "\n\n";
				for (uint i = 0; i != helpers.size(); ++i)
					cpp << helpers[i] << "\n";
			}

			cpp << "\t//! Rule " << name << '\n';
			cpp << "\ttemplate<class ContextT>\n";
			cpp << "\tstatic inline bool yy" << node.enumID << "(ContextT& ctx)\n";
			cpp << "\t{\n";
			cpp << "\t\tTRACE(\"entering " << node.enumID << "\");\n";
			cpp << body;
			cpp << "\t\treturn true;\n";
			cpp << "\t}\n";
			cpp << "\n\n";
		}


		inline void CPPConverter::generateCPP()
		{
			const Node::Map::const_iterator end = rules.end();
			cpp << "\n";
			cpp << "#include \"" << localInclude << "h\"\n";
			cpp << "#include <cassert>\n";
			cpp << "#include <yuni/core/string.h>\n";
			cpp << "#include <yuni/io/file.h>\n";
			cpp << "#include <yuni/io/directory.h>\n";
			cpp << "#include <yuni/core/dictionary.h>\n";
			cpp << '\n';
			cpp << "using namespace Yuni;\n";
			cpp << "\n\n";

			for (uint i = 0; i != namespaces.size(); ++i)
				cpp << "namespace " << namespaces[i] << "\n{\n";

			cpp << "namespace // anonymous\n{\n";
			cpp << "\n\n";
			SingleInstructionType(cpp, rules.size());
			cpp << "\n\n\n";

			cpp << "\t// Forward declarations\n";
			for (Node::Map::const_iterator i = rules.begin(); i != end; ++i)
				cpp << "\ttemplate<class ContextT> static bool yy" << i->second.enumID << "(ContextT& ctx);\n";

			cpp << "\n\n\n";

			PrepareCPPInclude(cpp);

			cpp << "\n";
			// generate all rules
			{
				uint sp = 0;
				for (Node::Map::const_iterator i = rules.begin(); i != end; ++i)
					GenerateFunctionForEachRule(cpp, sp, rules, i->first, i->second);
			}

			cpp << "\n\n\n";
			cpp << "} // anonymous namespace\n\n\n\n";

			cpp << "\tParser::Parser()\n";
			cpp << "\t{\n";
			cpp << "\t\tonURILoading.bind(& StandardURILoaderHandler);\n";
			cpp << "\t}\n\n\n";

			cpp << "\tvoid Parser::loadFromFile(const AnyString& filename)\n";
			cpp << "\t{\n";
			cpp << "\t\tDatasource ctx;\n";
			cpp << "\t\tctx.open(filename);\n";
			cpp << "\t\tbool ok = yyrgStart(ctx);\n";
			cpp << "\t\tTRACE(\"result: \" << (ok ? \"OK\" : \"FAILED\"));\n";
			cpp << "\t}\n\n\n";

			cpp << "\tvoid Parser::load(const AnyString& content)\n";
			cpp << "\t{\n";
			cpp << "\t\tDatasource ctx;\n";
			cpp << "\t\tctx.openContent(content);\n";
			cpp << "\t\tbool ok = yyrgStart(ctx);\n";
			cpp << "\t\tTRACE(\"result: \" << (ok ? \"OK\" : \"FAILED\"));\n";
			cpp << "\t}\n\n\n";
		}


		inline void CPPConverter::startHeaderheaderGuardID()
		{
			headerGuardID = "__HEADER";

			for (uint i = 0; i != namespaces.size(); ++i)
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
				uint i = (uint) namespaces.size();
				do
				{
					--i;
					h << "} // namespace " << namespaces[i] << '\n';
					hNodes << "} // namespace " << namespaces[i] << '\n';
					hxx << "} // namespace " << namespaces[i] << '\n';
					cpp << "} // namespace " << namespaces[i] << '\n';
				}
				while (i > 0);
			}

			h << '\n' << "# include \"" << localInclude << "hxx\"\n\n";
			h << "#endif // " << headerGuardID << "_H__\n";
			hNodes << "\n#endif // " << headerGuardID << "_NODES_H__\n";
			hxx << "\n#endif // " << headerGuardID << "_HXX__\n";
		}


		inline bool CPPConverter::writeFiles() const
		{
			AnyString root = rootfilename;
			YString file;
			file.clear() << root << "h";
			if (not IO::File::SetContent(file, h))
				return false;

			file.clear() << root << "nodes.h";
			if (not IO::File::SetContent(file, hNodes))
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
		if (not data.initialize(name))
			return false;

		data.startHeaderheaderGuardID();
		data.generateH();
		data.generateHNodes();
		data.generateHXX();
		data.generateCPP();
		data.endHeaderheaderGuardID();
		return data.writeFiles();
	}





} // namespace PEG
} // namespace Parser
} // namespace Yuni

