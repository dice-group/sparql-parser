
#ifndef SPARQL_PARSER_PARSER_HPP
#define SPARQL_PARSER_PARSER_HPP

#include "Dice/Sparql-Parser/internal/QueryGeneratorVisitor.hpp"
#include <SparqlLexer/SparqlLexer.h>

namespace
{
    using namespace Dice::tentris::sparql::parser;
}

class Parser
{

private:
    static SparqlParser* createParser(std::string text)
    {
        antlr4::ANTLRInputStream* input=new antlr4::ANTLRInputStream(text);
        SparqlLexer* lexer=new SparqlLexer(input);
        antlr4::CommonTokenStream* tokens=new antlr4::CommonTokenStream(lexer);
        SparqlParser* parser=new  SparqlParser(tokens);
        return parser;
    }

public:
    static std::shared_ptr<SelectQuery> parseSelectQuery(std::string query)
    {
        SparqlParser *parser = createParser(query);
        SparqlParser::QueryContext *tree = parser->query();

        QueryGeneratorVisitor visitor;
        std::shared_ptr<SelectQuery> selectQuery = visitor.visitQuery(tree);
        return selectQuery;
    }
};


#endif //SPARQL_PARSER_PARSER_HPP
