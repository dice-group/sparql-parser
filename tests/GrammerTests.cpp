#include <gtest/gtest.h>
#include <Dice/Sparql-Parser/QueryGeneratorVisitor.hpp>
#include <SparqlLexer/SparqlLexer.h>
#include <SparqlParser/SparqlParser.h>

namespace
{
    using namespace Dice::tentris::sparql::parser;
}

std::unique_ptr<SparqlParser> createParser(std::string text)
{
    antlr4::ANTLRInputStream input(text);
    SparqlLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    std::unique_ptr<SparqlParser> parser=std::make_unique<SparqlParser>(&tokens);
    return parser;
}


TEST(GrammerTests, junk) {

}


TEST(GrammerTests, Var1Test) {




}