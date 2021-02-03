#ifndef SPARQL_PARSER_SELECTCLAUSE_HPP
#define SPARQL_PARSER_SELECTCLAUSE_HPP

#include "SelectNodeType.hpp"

namespace Dice::sparql_parser::internal {
    struct SelectClause {
        SelectNodeType nodeType;
        std::vector<sparql::Variable> selectVariables;

    };
}
#endif //SPARQL_PARSER_SELECTCLAUSE_HPP
