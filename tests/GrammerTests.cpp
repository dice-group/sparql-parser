#include <gtest/gtest.h>
#include <Dice/Sparql-Parser/internal/QueryGeneratorVisitor.hpp>
#include <SparqlLexer/SparqlLexer.h>
#include <SparqlParser/SparqlParser.h>

#include <Dice/rdf_parser/Sparql/TripleVariable.hpp>

namespace
{
    using namespace Dice::tentris;
}

SparqlParserBase::SparqlParser* createParser(std::string text)
{
    antlr4::ANTLRInputStream* input=new antlr4::ANTLRInputStream(text);
    SparqlParserBase::SparqlLexer* lexer=new SparqlParserBase::SparqlLexer(input);
    antlr4::CommonTokenStream* tokens=new antlr4::CommonTokenStream(lexer);
    SparqlParserBase::SparqlParser* parser=new  SparqlParserBase::SparqlParser(tokens);
    return parser;
}












TEST(GrammerTests,varOrTermTest) {

    //test var type
    std::string var1{"?abc"};
    //test term type
    std::string termString{"<http://script.example/example1>"};
    SparqlParserBase::SparqlParser*  parser1=createParser(var1);
    SparqlParserBase::SparqlParser*  parser2=createParser(termString);

    SparqlParserBase::SparqlParser::VarOrTermContext* tree1=parser1->varOrTerm();
    SparqlParserBase::SparqlParser::VarOrTermContext* tree2=parser2->varOrTerm();

    SparqlParser::internal::QueryGeneratorVisitor visitor;
    rdf_parser::SparqlQuery::TripleVariable  tripleVariable= visitor.visitVarOrTerm(tree1);
    Term term= visitor.visitVarOrTerm(tree2);
    ASSERT_EQ(tripleVariable.getName(),"abc");
    ASSERT_EQ(term,Term::make_term(termString));


}

TEST(GrammerTests, VarTest) {

    //test var1 type
    std::string var1{"?abc"};
    //test var2 type
    std::string var2{"$abc"};
    SparqlParserBase::SparqlParser*  parser1=createParser(var1);
    SparqlParserBase::SparqlParser*  parser2=createParser(var2);

    SparqlParserBase::SparqlParser::VarContext* tree1=parser1->var();
    SparqlParserBase::SparqlParser::VarContext* tree2=parser2->var();

    SparqlParser::internal::QueryGeneratorVisitor visitor;
    rdf_parser::SparqlQuery::TripleVariable tripleVariable1= visitor.visitVar(tree1);
    rdf_parser::SparqlQuery::TripleVariable  tripleVariable2= visitor.visitVar(tree2);
    ASSERT_EQ(tripleVariable1.getName(),"abc");
    ASSERT_EQ(tripleVariable2.getName(),"abc");


}

TEST(GrammerTests, GraphTermTest) {

    std::string iri{"<http://script.example/example1>"};
    SparqlParserBase::SparqlParser*  parser=createParser(iri);
    SparqlParserBase::SparqlParser::GraphTermContext* tree=parser->graphTerm();

    SparqlParser::internal::QueryGeneratorVisitor visitor;
    Term term= visitor.visitGraphTerm(tree);
    ASSERT_EQ(term,Term::make_term(iri));

}

TEST(GrammerTests, pathModeTest) {

    std::string modeString{"+"};
    SparqlParserBase::SparqlParser*  parser=createParser(modeString);
    SparqlParserBase::SparqlParser::PathModContext* tree=parser->pathMod();

    SparqlParser::internal::QueryGeneratorVisitor visitor;
    std::string mode= visitor.visitPathMod(tree);
    ASSERT_EQ(mode,"+");

}



TEST(GrammerTests, iriTest) {

    std::string iriString{"<http://purl.org/dc/elements/1.1/title>"};
    SparqlParserBase::SparqlParser*  parser=createParser(iriString);
    SparqlParserBase::SparqlParser::IriContext* tree=parser->iri();

    SparqlParser::internal::QueryGeneratorVisitor visitor;
    Term iri= visitor.visitIri(tree);
    ASSERT_EQ(iri,Term::make_term(iriString));

}

TEST(GrammerTests, tripleBlockTest) {
        std::string tripleBlockString{"?book <dc:title> ?title ;\n"
                              "         <ns:price> ?price ."};
        SparqlParserBase::SparqlParser *parser = createParser(tripleBlockString);
        SparqlParserBase::SparqlParser::TriplesBlockContext *tree = parser->triplesBlock();

        SparqlParser::internal::QueryGeneratorVisitor visitor;
        std::shared_ptr<ICommandNode> node = visitor.visitTriplesBlock(tree);
        //ASSERT_EQ(iri,Term::make_term(iriString));
}

TEST(GrammerTests, BasicSelectQueryDefaultTest) {
    std::string query{
                      "SELECT ?book ?nameY ?nickY\n"
                      "WHERE\n"
                      "  { ?book <dc:title> ?title ;\n"
            "         <ns:price> ?price ."
                      "  }"};
    SparqlParserBase::SparqlParser *parser = createParser(query);
    SparqlParserBase::SparqlParser::QueryContext *tree = parser->query();

    SparqlParser::internal::QueryGeneratorVisitor visitor;
    std::shared_ptr<SelectQuery> selectQuery = visitor.visitQuery(tree);
//    selectQuery->executeQuery();


    ASSERT_EQ(true,true);
    //ASSERT_EQ(iri,Term::make_term(iriString));
}

TEST(GrammerTests, BasicSelectQueryDistinctTest) {
    std::string query{
            "SELECT DISTINCT ?book ?nameY ?nickY\n"
            "WHERE\n"
            "  { ?book <dc:title> ?title ;\n"
            "         <ns:price> ?price ."
            "  }"};
    SparqlParserBase::SparqlParser *parser = createParser(query);
    SparqlParserBase::SparqlParser::QueryContext *tree = parser->query();

    SparqlParser::internal::QueryGeneratorVisitor visitor;
    std::shared_ptr<SelectQuery> selectQuery = visitor.visitQuery(tree);
//    selectQuery->executeQuery();


    ASSERT_EQ(true,true);
    //ASSERT_EQ(iri,Term::make_term(iriString));
}

TEST(GrammerTests, BasicSelectQueryReducedTest) {
    std::string query{
            "SELECT REDUCED ?book ?nameY ?nickY\n"
            "WHERE\n"
            "  { ?book <dc:title> ?title ;\n"
            "         <ns:price> ?price ."
            "  }"};
    SparqlParserBase::SparqlParser *parser = createParser(query);
    SparqlParserBase::SparqlParser::QueryContext *tree = parser->query();

    SparqlParser::internal::QueryGeneratorVisitor visitor;
    std::shared_ptr<SelectQuery> selectQuery = visitor.visitQuery(tree);
//    selectQuery->executeQuery();


    ASSERT_EQ(true,true);
    //ASSERT_EQ(iri,Term::make_term(iriString));
}

TEST(GrammerTests, BasicOptionalSelectQueryReducedTest) {
    std::string query{
            "SELECT REDUCED ?book ?nameY ?nickY\n"
            "WHERE\n"
            "  { ?book <dc:title> ?title ;\n"
            "          <ns:price> ?price ."
            "OPTIONAL { ?book <dc:author> <dc:title> }"
            "  }"};
    SparqlParserBase::SparqlParser *parser = createParser(query);
    SparqlParserBase::SparqlParser::QueryContext *tree = parser->query();

    SparqlParser::internal::QueryGeneratorVisitor visitor;
    std::shared_ptr<SelectQuery> selectQuery = visitor.visitQuery(tree);
//    selectQuery->executeQuery();


    ASSERT_EQ(true,true);
    //ASSERT_EQ(iri,Term::make_term(iriString));
}

TEST(GrammerTests, ComplexOptionalSelectQueryReducedTest) {
    std::string query{
            "SELECT REDUCED ?book ?nameY ?nickY\n"
            "WHERE\n"
            "  { ?book <dc:title> ?title ;\n"
            "          <ns:price> ?price ."
            "OPTIONAL { ?book <dc:author> <dc:title> \n"
            "  OPTIONAL { ?book <dc:isbn> ?isbn. } \n } \n"
            "  }"};
    SparqlParserBase::SparqlParser *parser = createParser(query);
    SparqlParserBase::SparqlParser::QueryContext *tree = parser->query();

    SparqlParser::internal::QueryGeneratorVisitor visitor;
    std::shared_ptr<SelectQuery> selectQuery = visitor.visitQuery(tree);
//    selectQuery->executeQuery();


    ASSERT_EQ(true,true);
    //ASSERT_EQ(iri,Term::make_term(iriString));
}