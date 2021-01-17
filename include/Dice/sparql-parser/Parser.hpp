
#ifndef SPARQL_PARSER_PARSER_HPP
#define SPARQL_PARSER_PARSER_HPP

#include <SparqlLexer/SparqlLexer.h>

#include "Dice/sparql-parser/internal/QueryGeneratorVisitor.hpp"
#include "Dice/sparql-parser/internal/Exceptions.hpp"


namespace Dice::sparql_parser {
    class Parser {

    private:
        static Dice::tentris::SparqlParserBase::SparqlParser *createParser(std::string text) {
            antlr4::ANTLRInputStream *input = new antlr4::ANTLRInputStream(text);
            Dice::tentris::SparqlParserBase::SparqlLexer *lexer = new Dice::tentris::SparqlParserBase::SparqlLexer(input);
            antlr4::CommonTokenStream *tokens = new antlr4::CommonTokenStream(lexer);
            Dice::tentris::SparqlParserBase::SparqlParser *parser = new Dice::tentris::SparqlParserBase::SparqlParser(tokens);
            return parser;
        }

    public:
        static std::shared_ptr<SparqlQueryGraph::Nodes::SelectNodes::SelectNode> parseSelectQuery(std::string query) {

            Dice::tentris::SparqlParserBase::SparqlParser *parser;
            Dice::tentris::SparqlParserBase::SparqlParser::QueryContext *tree;
            try {
                parser = createParser(query);
                tree = parser->query();
            }
            catch (const sparql_parser::internal::ParseException &exception) {
                std::cout<<exception.what()<<std::endl;
            }
            internal::QueryGeneratorVisitor visitor;
            std::shared_ptr<SparqlQueryGraph::Nodes::SelectNodes::SelectNode> selectNode = visitor.visitQuery(tree);
            return selectNode;

        }
    };
}

#endif //SPARQL_PARSER_PARSER_HPP
