//
// Created by fakhr on 15.04.20.
//

#ifndef SPARQL_PARSER_SELECTCLAUSE_HPP
#define SPARQL_PARSER_SELECTCLAUSE_HPP

#include "SelectNodeType.hpp"

struct SelectClause
{
    SelectNodeType nodeType;
    std::vector<TripleVariable> selectVariables;

};

#endif //SPARQL_PARSER_SELECTCLAUSE_HPP
