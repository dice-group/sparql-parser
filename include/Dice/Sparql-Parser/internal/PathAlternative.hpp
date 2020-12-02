//
// Created by fakhr on 08.06.20.
//

#ifndef SPARQL_PARSER_PATHALTERNATIVE_HPP
#define SPARQL_PARSER_PATHALTERNATIVE_HPP

#include <vector>

namespace SparqlParser::internal {
    class PathSequence;

    class PathAlternative {
    private:
        std::vector<PathSequence> sequences;
    };
}
#endif //SPARQL_PARSER_PATHALTERNATIVE_HPP
