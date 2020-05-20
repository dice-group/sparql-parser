#include <gtest/gtest.h>
#include <Dice/Sparql-Parser/QueryGeneratorVisitor.hpp>
#include <SparqlLexer/SparqlLexer.h>
#include <SparqlParser/SparqlParser.h>

#include <Dice/Sparql-Query/TripleVariable.hpp>

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












TEST(GrammerTests,varOrTermTest) {

    //test var type
    std::string var1{"?abc"};
    //test term type
    std::string termString{"<http://script.example/example1>"};
    SparqlParser*  parser1=createParser(var1);
    SparqlParser*  parser2=createParser(termString);

    SparqlParser::VarOrTermContext* tree1=parser1->varOrTerm();
    SparqlParser::VarOrTermContext* tree2=parser2->varOrTerm();

    QueryGeneratorVisitor visitor;
    TripleVariable tripleVariable= visitor.visitVarOrTerm(tree1);
    Term term= visitor.visitVarOrTerm(tree2);
    ASSERT_EQ(tripleVariable.getName(),"abc");
    ASSERT_EQ(term,Term::make_term(termString));


}

TEST(GrammerTests, VarTest) {

    //test var1 type
    std::string var1{"?abc"};
    //test var2 type
    std::string var2{"$abc"};
    SparqlParser*  parser1=createParser(var1);
    SparqlParser*  parser2=createParser(var2);

    SparqlParser::VarContext* tree1=parser1->var();
    SparqlParser::VarContext* tree2=parser2->var();

    QueryGeneratorVisitor visitor;
    TripleVariable tripleVariable1= visitor.visitVar(tree1);
    TripleVariable tripleVariable2= visitor.visitVar(tree2);
    ASSERT_EQ(tripleVariable1.getName(),"abc");
    ASSERT_EQ(tripleVariable2.getName(),"abc");


}

TEST(GrammerTests, GraphTermTest) {

    std::string iri{"<http://script.example/example1>"};
    SparqlParser*  parser=createParser(iri);
    SparqlParser::GraphTermContext* tree=parser->graphTerm();

    QueryGeneratorVisitor visitor;
    Term term= visitor.visitGraphTerm(tree);
    ASSERT_EQ(term,Term::make_term(iri));

}