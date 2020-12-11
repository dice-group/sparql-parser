//
// Created by fakhr on 11.03.20.
//

#ifndef SPARQL_PARSER_EXCEPTIONS_HPP
#define SPARQL_PARSER_EXCEPTIONS_HPP

#include <exception>

namespace SparqlParser::internal {
    class ParseException : public std::exception {
    public:
        const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
            return "The query can't be parsed";
        }
    };


    class NotImplementedException : public std::exception {

    };

}
#endif //SPARQL_PARSER_EXCEPTIONS_HPP
