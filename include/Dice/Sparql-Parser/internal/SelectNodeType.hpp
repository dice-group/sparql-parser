//
// Created by fakhr on 11.03.20.
//

#ifndef SPARQL_PARSER_SELECTNODETYPE_HPP
#define SPARQL_PARSER_SELECTNODETYPE_HPP
namespace SparqlParser::internal {
    enum SelectNodeType {
        DEFAULT,
        DISTINCT,
        REDUCED
    };
}
#endif //SPARQL_PARSER_SELECTNODETYPE_HPP
