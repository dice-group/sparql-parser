
#ifndef SPARQL_PARSER_PARSER_HPP
#define SPARQL_PARSER_PARSER_HPP

#include <SparqlLexer/SparqlLexer.h>

#include "Dice/sparql-parser/internal/Exceptions.hpp"
#include "Dice/sparql-parser/internal/QueryGeneratorVisitor.hpp"


namespace Dice::sparql_parser {
	inline std::shared_ptr<Dice::sparql::Nodes::QueryNodes::SelectNodes::SelectNode>
	parseSelectQuery(const std::string &query) {
		antlr4::ANTLRInputStream input(query);
		Dice::sparql_parser::base::SparqlLexer lexer(&input);
		antlr4::CommonTokenStream tokens(&lexer);
		Dice::sparql_parser::base::SparqlParser parser(&tokens);

		auto tree = parser.query();
		internal::QueryGeneratorVisitor visitor;
		return visitor.visitQuery(tree);
	}

	inline std::shared_ptr<Dice::sparql::Nodes::Node>
	parse(const std::string &query) {
		return parseSelectQuery(query);
	}
}// namespace Dice::sparql_parser

#endif//SPARQL_PARSER_PARSER_HPP
