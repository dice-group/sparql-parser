

#include "include/Dice/Sparql-Parser/QueryGeneratorVisitor.hpp"
#include <SparqlLexer/SparqlLexer.h>
#include <SparqlParser/SparqlParser.h>
#include <antlr4-runtime.h>

using namespace Dice::tentris::sparql::parser;
using namespace antlr4;

int main(){
    std::istringstream str_stream{"SELECT ?title\n"
                                  "WHERE\n"
                                  "{\n"
                                  "  <http://example.org/book/book1> <http://purl.org/dc/elements/1.1/title> ?title .\n"
                                  "}"};

    ANTLRInputStream input(str_stream);
    SparqlLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    SparqlParser parser(&tokens);
    SparqlParser::QueryContext* tree = parser.query();
    QueryGeneratorVisitor visitor;
    //SelectQuery query = visitor.visitSelectQuery(tree);
    //query.executeQuery();


    return  1 ;
}