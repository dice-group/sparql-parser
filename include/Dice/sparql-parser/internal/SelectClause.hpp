#ifndef SPARQL_PARSER_SELECTCLAUSE_HPP
#define SPARQL_PARSER_SELECTCLAUSE_HPP

#include "Dice/sparql-parser/internal/SelectNodeType.hpp"

namespace Dice::sparql_parser::internal {
	struct SelectClause {
		SelectNodeType nodeType;
		std::vector<sparql::Variable> selectVariables;
	};
}// namespace Dice::sparql_parser::internal
#endif//SPARQL_PARSER_SELECTCLAUSE_HPP
