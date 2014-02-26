
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

			h << "#ifndef " << headerGuardID << "_H__\n";
			h << "# define " << headerGuardID << "_H__\n";
			h << "\n";
			h << "# include <yuni/yuni.h>\n";
			h << "# include <yuni/core/string.h>\n";
			h << "# include <yuni/core/bind.h>\n";
			h << "\n\n";

			for (uint i = 0; i != namespaces.size(); ++i)
				h << "namespace " << namespaces[i] << "\n{\n";
			h << "\n\n";

			h << "\tenum Rule\n";
			h << "\t{\n";
			h << "\t\t//! Unknown rule\n";
			h << "\t\trgUnknown = 0,\n";
			uint enumIndex = 1;
			for (Node::Map::const_iterator i = rules.begin(); i != end; ++i, ++enumIndex)
			{
				h << "\t\t//! enum for the rule `" << i->first << '`';
				if (i->second.attributes.inlined)
					h << " [inline]";
				h << '\n';
				h << "\t\t" << i->second.enumID << " = " << enumIndex << ",\n";
			}

			h << "\t\t//! enum for the final rule\n";
			h << "\t\trgEOF = " << (enumIndex++) << "\n";

			h << "\t};\n";

			// rule count
			h << "\tenum\n";
			h << "\t{\n";
			h << "\t\t//! The total number of rules\n";
			h << "\t\truleCount = " << enumIndex << '\n';
			h << "\t};\n";
			h << "\n\n\n";

			h << "\t//! Convert a rule id into its text representation\n";
			h << "\tAnyString RuleToString(enum Rule);\n";
			h << "\n\n\n";

			h << "\tenum Error\n";
			h << "\t{\n";
			h << "\t\t//! No error\n";
			h << "\t\terrNone,\n";
			h << "\t\t//! Parse error\n";
			h << "\t\terrParse,\n";
			h << "\t};\n";
			h << "\n\n\n\n\n";

			h << "\tclass Notification final\n";
			h << "\t{\n";
			h << "\tpublic:\n";
			h << "\t\t//! Most suitable martptr\n";
			h << "\t\ttypedef Yuni::SmartPtr<Notification> Ptr;\n";
			h << "\t\t//! Vector of nodes\n";
			h << "\t\ttypedef std::vector<Ptr> Vector;\n";
			h << "\n";
			h << "\tpublic:\n";
			h << "\t	Notification()\n";
			h << "\t		: offset()\n";
			h << "\t		, line()\n";
			h << "\t	{}\n";
			h << '\n';
			h << "\tpublic:\n";
			h << "\t	//! Start offset\n";
			h << "\t	uint offset;\n";
			h << "\t	//! Line Index\n";
			h << "\t	uint line;\n";
			h << "\t	//! Filename\n";
			h << "\t	YString message;\n";
			h << "\t	//! Filename\n";
			h << "\t	YString filename;\n";
			h << '\n';
			h << "\t};\n\n\n\n\n";

			h << "\tclass Node final\n";
			h << "\t{\n";
			h << "\tpublic:\n";
			h << "\t\ttypedef AnyString Text;\n";
			h << "\t\t//! Vector of nodes\n";
			h << "\t\ttypedef std::vector<Node*> Vector;\n";
			h << "\n";
			h << "\tpublic:\n";
			h << "\t	static void Export(Yuni::Clob& out, const Node& node);\n";
			h << "\t	static void Export(Yuni::Clob& out, const Node& node, bool color);\n";
			h << "\t	static void ExportToHTML(Yuni::Clob& out, const Node& node);\n";
			h << "\n";
			h << "\tpublic:\n";
			h << "\t	Node() :\n";
			h << "\t		userdata()\n";
			h << "\t	{}\n";
			h << '\n';
			h << "\t	~Node()\n";
			h << "\t	{\n";
			h << "\t		for (unsigned int i = 0; i != (unsigned) children.size(); ++i)\n";
			h << "\t			delete children[i];\n";
			h << "\t	}\n";
			h << "\n";
			h << "\tpublic:\n";
			h << "\t	//! The rule ID\n";
			h << "\t	enum Rule rule;\n";
			h << "\t	//! Start offset\n";
			h << "\t	uint offset;\n";
			h << "\t	//! End offset\n";
			h << "\t	uint offsetEnd;\n";
			h << "\t	//! Line Index\n";
			h << "\t	uint line;\n";
			h << "\t	//! Text associated to the node (if any)\n";
			h << "\t	Text text;\n";
			h << "\t	//! Data associated to this node\n";
			h << "\t	void* userdata;\n";
			h << "\n";
			h << "\t	//! Children\n";
			h << "\t	Node::Vector children;\n";
			h << "\t};\n\n\n\n\n";

			h << "\tclass Parser final\n";
			h << "\t{\n";
			h << "\tpublic:\n";
			h << "\t\ttypedef Yuni::Bind<bool (Yuni::Clob& out, const AnyString& uri)>   OnURILoading;\n";
			h << "\t\ttypedef Yuni::Bind<bool (const AnyString& filename, uint line, uint offset, Error, const YString::Vector&)>  OnError;\n";
			h << '\n';
			h << "\tpublic:\n";
			h << "\t\tParser();\n";
			h << "\t\t~Parser();\n";
			h << '\n';
			h << "\t\tbool loadFromFile(const AnyString& filename);\n";
			h << "\t\tbool load(const AnyString& content);\n";
			h << '\n';
			h << "\tpublic:\n";
			h << "\t\t//! Event: load another include file\n";
			h << "\t\tOnURILoading onURILoading;\n";
			h << "\t\t//! Event: load another include file\n";
			h << "\t\tOnError  onError;\n";
			h << '\n';
			h << "\t\t//! The root node, if any\n";
			h << "\t\tNode* root;\n";
			h << '\n';
			h << "\t\t//! Notifications\n";
			h << "\t\tNotification::Vector notifications;\n";
			h << '\n';
			h << '\n';
			h << "\tprivate:\n";
			h << "\t\tvoid* pData;\n";
			h << '\n';
			h << "\t}; // class Parser\n";
		}


		inline void CPPConverter::generateHXX()
		{
			hxx << "#ifndef " << headerGuardID << "_HXX__\n";
			hxx << "# define " << headerGuardID << "_HXX__\n";
			hxx << "\n\n\n";

			hxx << "inline std::ostream& operator << (std::ostream& out, const ";
			for (uint i = 0; i != namespaces.size(); ++i)
				hxx << "::" << namespaces[i];
			hxx << "::Node& node)\n";
			hxx << "{\n";
			hxx << "\t::Yuni::Clob content;\n";
			hxx << "\t";
			for (uint i = 0; i != namespaces.size(); ++i)
				hxx << "::" << namespaces[i];
			hxx << "::Node::Export(content, node);";
			hxx << "out << content;\n";
			hxx << "\treturn out;\n";
			hxx << "}\n";
			hxx << '\n';
			hxx << '\n';
			hxx << '\n';


			hxx << "inline std::ostream& operator << (std::ostream& out, const ";
			for (uint i = 0; i != namespaces.size(); ++i)
				hxx << "::" << namespaces[i];
			hxx << "::Node* node)\n";
			hxx << "{\n";
			hxx << "\tif (node)\n";
			hxx << "\t\tout << *node;\n";
			hxx << "\telse\n";
			hxx << "\t\tout << \"<invalid ast node>\";\n";
			hxx << "\treturn out;\n";
			hxx << "}\n";
			hxx << '\n';
			hxx << '\n';
			hxx << '\n';

			for (uint i = 0; i != namespaces.size(); ++i)
				hxx << "namespace " << namespaces[i] << "\n{\n";
			hxx << "\n\n";

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
					for (uint i = 0; i != (uint) node.children.size(); ++i)
						node.children[i].exportCPP(body, rules, helpers, datatext, 2, true, sp);
				}
				else
					node.exportCPP(body, rules, helpers, datatext, 2, true, sp);

				// helpers
				bool foundHelper = false;

				if (not datatext.empty())
				{
					for (uint i = 0; i != (uint) datatext.size(); ++i)
						cpp << '\t' << datatext[i] << '\n';
					cpp << "\n\n\n";
				}

				for (uint i = 0; i != (uint) helpers.size(); ++i)
				{
					if (not helpers[i].empty())
					{
						if (not foundHelper)
						{
							foundHelper = true;
							cpp << "\n\n";
						}
						// extract forward declaration
						uint linefeed = helpers[i].find('\n');
						if (linefeed < helpers[i].size())
							cpp << AnyString(helpers[i], 0, linefeed) << ";\n";
					}
				}

				foundHelper = false;
				for (uint i = 0; i != (uint) helpers.size(); ++i)
				{
					if (not helpers[i].empty())
					{
						if (not foundHelper)
						{
							foundHelper = true;
							cpp << "\n\n";
						}
						cpp << helpers[i] << "\n";
					}
				}
			}

			cpp << "\t//! Rule " << name << '\n';
			cpp << "\t";
			cpp << ((node.enumID != "rgStart") ? "static inline " : "static ");

			cpp << "bool yy" << node.enumID << "(Datasource& ctx)\n";
			cpp << "\t{\n";
			cpp << "\t\tTRACE(\"entering " << node.enumID;
			if (node.attributes.inlined)
				cpp << " [inline]";
			cpp << "\");\n";

			if (not node.attributes.inlined)
				cpp << "\t\tuint _ruleOffset = ctx.enterRule(" << node.enumID << ");\n";
			cpp << '\n';
			cpp << body;
			cpp << '\n';
			if (not node.attributes.inlined)
				cpp << "\t\tctx.commit(_ruleOffset, " << node.enumID << ");\n";
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
			cpp << "#include <yuni/core/noncopyable.h>\n";
			cpp << "#include <yuni/datetime/timestamp.h>\n";
			cpp << "#include <yuni/core/system/console/console.h>\n";
			cpp << '\n';
			cpp << "using namespace Yuni;\n";
			cpp << "\n\n";

			for (uint i = 0; i != namespaces.size(); ++i)
				cpp << "namespace " << namespaces[i] << "\n{\n";

			cpp << "namespace // anonymous\n{\n";
			cpp << "\n\n";

			cpp << "\tstatic inline bool  RuleAttributeCapture(enum Rule ruleid)\n";
			cpp << "\t{\n";
			cpp << "\t\tstatic const bool attr[] = {\n";
			cpp << "\t\t\tfalse, // rgUnknown\n";
			for (Node::Map::const_iterator i = rules.begin(); i != end; ++i)
				cpp << "\t\t\t" << (i->second.attributes.capture ? "true" : "false") << ", // " << i->second.enumID << "\n";
			cpp << "\t\t\tfalse, // rgEOF\n";
			cpp << "\t\t};\n";
			cpp << "\t\tassert((uint) ruleid < (uint) ruleCount);\n";
			cpp << "\t\treturn attr[(uint) ruleid];\n";
			cpp << "\t}\n\n\n\n";

			cpp << "\tstatic inline bool  RuleAttributeImportant(enum Rule ruleid)\n";
			cpp << "\t{\n";
			cpp << "\t\tstatic const bool attr[] = {\n";
			cpp << "\t\t\tfalse, // rgUnknown\n";
			for (Node::Map::const_iterator i = rules.begin(); i != end; ++i)
				cpp << "\t\t\t" << (i->second.attributes.important ? "true" : "false") << ", // " << i->second.enumID << "\n";
			cpp << "\t\t\tfalse, // rgEOF\n";
			cpp << "\t\t};\n";
			cpp << "\t\tassert((uint) ruleid < (uint) ruleCount);\n";
			cpp << "\t\treturn attr[(uint) ruleid];\n";
			cpp << "\t}\n\n\n\n";

			cpp << "\tstatic inline AnyString  RuleAttributeSimpleTextCapture(enum Rule ruleid)\n";
			cpp << "\t{\n";
			cpp << "\t\tstatic const AnyString attr[] = {\n";
			cpp << "\t\t\tnullptr, // rgUnknown\n";
			String textCapture;
			for (Node::Map::const_iterator i = rules.begin(); i != end; ++i)
			{
				cpp << "\t\t\t";
				if (i->second.children.size() == 1 and i->second.children[0].isSimpleTextCapture())
				{
					textCapture = i->second.children[0].rule.text;
					textCapture.replace("\"", "\\\"");
					cpp << '"' << textCapture << '"';
				}
				else
					cpp << "nullptr";

				cpp << ", // " << i->second.enumID << "\n";
			}
			cpp << "\t\t\tnullptr, // rgEOF\n";
			cpp << "\t\t};\n";
			cpp << "\t\tassert((uint) ruleid < (uint) ruleCount);\n";
			cpp << "\t\treturn attr[(uint) ruleid];\n";
			cpp << "\t}\n\n\n\n";

			PrepareCPPInclude(cpp);

			cpp << "\t// Forward declarations\n";
			for (Node::Map::const_iterator i = rules.begin(); i != end; ++i)
				cpp << "\tstatic inline bool yy" << i->second.enumID << "(Datasource& ctx);\n";

			cpp << "\n\n\n";

			cpp << "\n";
			// generate all rules
			{
				uint sp = 0;
				for (Node::Map::const_iterator i = rules.begin(); i != end; ++i)
					GenerateFunctionForEachRule(cpp, sp, rules, i->first, i->second);
			}

			cpp << "\n\n\n";
			cpp << "} // anonymous namespace\n\n\n\n\n\n";

			cpp << "\tAnyString RuleToString(enum Rule ruleid)\n";
			cpp << "\t{\n";
			cpp << "\t\tswitch (ruleid)\n";
			cpp << "\t\t{\n";
			cpp << "\t\t\tcase rgUnknown:\n";
			cpp << "\t\t\t\treturn \"<unknown>\";\n";
			for (Node::Map::const_iterator i = rules.begin(); i != end; ++i)
			{
				cpp << "\t\t\tcase " << i->second.enumID << ":\n";
				cpp << "\t\t\t\treturn \"" << i->first << "\";\n";
			}
			cpp << "\t\t\tcase rgEOF:\n";
			cpp << "\t\t\t\treturn \"EOF\";\n";
			cpp << "\t\t}\n";
			cpp << "\t\treturn \"<error>\";\n";
			cpp << "\t}\n";
			cpp << "\n\n\n\n";

			cpp << "\tParser::Parser() :\n";
			cpp << "\t\troot(nullptr),\n";
			cpp << "\t\tpData()\n";
			cpp << "\t{\n";
			cpp << "\t\tonURILoading.bind(& StandardURILoaderHandler);\n";
			cpp << "\t}\n\n\n";

			cpp << "\tParser::~Parser()\n";
			cpp << "\t{\n";
			cpp << "\t\tdelete (Datasource*) pData;\n";
			cpp << "\t}\n\n\n";

			cpp << "\tbool Parser::loadFromFile(const AnyString& filename)\n";
			cpp << "\t{\n";
			cpp << "\t\tif (!pData)\n";
			cpp << "\t\t\tpData = new Datasource(notifications);\n";
			cpp << "\n";
			cpp << "\t\tDatasource& ctx = *((Datasource*) pData);\n";
			cpp << "\t\tctx.open(filename);\n";
			cpp << "\t\tDATASOURCE_PARSE(ctx);\n";
			cpp << "\t\treturn ctx.success;\n";
			cpp << "\t}\n\n\n";

			cpp << "\tbool Parser::load(const AnyString& content)\n";
			cpp << "\t{\n";
			cpp << "\t\tif (!pData)\n";
			cpp << "\t\t\tpData = new Datasource(notifications);\n";
			cpp << "\n";
			cpp << "\t\tDatasource& ctx = *((Datasource*) pData);\n";
			cpp << "\t\tctx.openContent(content);\n";
			cpp << "\t\tDATASOURCE_PARSE(ctx);\n";
			cpp << "\t\treturn ctx.success;\n";
			cpp << "\t}\n\n\n";

			cpp << "\tvoid Node::ExportToHTML(Clob& out, const Node& node)\n";
			cpp << "\t{\n";
			cpp << "\t\tassert(&node and \"invalid reference to node\");\n";
			cpp << '\n';
			cpp << "\t\tString tmp;\n";
			cpp << "\t\tString indent;\n";
			cpp << "\t\tInternalNodeExportHTML(out, node, indent, tmp);\n";
			cpp << "\t}\n\n\n";

			cpp << "\tvoid Node::Export(Clob& out, const Node& node, bool color)\n";
			cpp << "\t{\n";
			cpp << "\t\tassert(&node and \"invalid reference to node\");\n";
			cpp << '\n';
			cpp << "\t\tString tmp;\n";
			cpp << "\t\tString indent;\n";
			cpp << "\t\tif (not color)\n";
			cpp << "\t\t\tInternalNodeExportConsole<false>(out, node, false, indent, tmp);\n";
			cpp << "\t\telse\n";
			cpp << "\t\t\tInternalNodeExportConsole<true>(out, node, false, indent, tmp);\n";
			cpp << "\t}\n\n\n";

			cpp << "\tvoid Node::Export(Clob& out, const Node& node)\n";
			cpp << "\t{\n";
			cpp << "\t\tExport(out, node, ::Yuni::System::Console::IsStdoutTTY());\n";
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
					hxx << "} // namespace " << namespaces[i] << '\n';
					cpp << "} // namespace " << namespaces[i] << '\n';
				}
				while (i > 0);
			}

			h << '\n' << "# include \"" << localInclude << "hxx\"\n\n";
			h << "#endif // " << headerGuardID << "_H__\n";
			hxx << "\n#endif // " << headerGuardID << "_HXX__\n";
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
		if (not data.initialize(name))
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

