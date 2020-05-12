#include <gtest/gtest.h>
#include <Dice/Sparql-Parser/QueryGeneratorVisitor.hpp>
#include <SparqlLexer/SparqlLexer.h>
#include <SparqlParser/SparqlParser.h>

namespace
{
    using namespace Dice::tentris::sparql::parser;
}

SparqlParser* createParser(std::string text)
{
    antlr4::ANTLRInputStream* input=new antlr4::ANTLRInputStream(text);
    SparqlLexer* lexer=new SparqlLexer(input);
    antlr4::CommonTokenStream* tokens=new antlr4::CommonTokenStream(lexer);
    SparqlParser* parser=new  SparqlParser(tokens);
    return parser;
}


TEST(GrammerTests, junk) {

}


TEST(GrammerTests, Var1Test) {




}

TEST(GrammerTests, GraphTermTest) {

    std::string iri{"<http://script.example/Latin>"};
    SparqlParser*  parser=createParser(iri);
    SparqlParser::GraphTermContext* tree=parser->graphTerm();

    QueryGeneratorVisitor visitor;
    Term term= visitor.visitGraphTerm(tree);

}