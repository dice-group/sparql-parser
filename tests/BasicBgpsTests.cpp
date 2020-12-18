#include <gtest/gtest.h>
#include <Dice/Sparql-Parser/Parser.hpp>

using namespace SparqlQueryGraph::Nodes::SelectNodes;

TEST(BasicBgpsTests, multipleBgps) {
    std::string query{
            "SELECT ?s ?e WHERE {?s <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://localhost/vocabulary/bench/Journal>.?s <http://swrc.ontoware.org/ontology#editor> ?e}"};


    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);
    auto operands=selectNode->getOperands();
    auto bgps=selectNode->getBgps();
    auto  prefixes=selectNode->getPrefixes();
    auto subscript=selectNode->getSubscriptResult();
    auto selectVariables=selectNode->getSelectVariables();


    ASSERT_EQ(true,true);
    //ASSERT_EQ(iri,Term::make_term(iriString));
}



TEST(BasicBgpsTests, multipleBgps2) {
    std::string query{
            "SELECT DISTINCT ?s ?p ?o WHERE {?s <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://localhost/vocabulary/bench/Article> . ?s ?p ?o .}"};


    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);
    auto operands=selectNode->getOperands();
    auto bgps=selectNode->getBgps();
    auto  prefixes=selectNode->getPrefixes();
    auto subscript=selectNode->getSubscriptResult();
    auto selectVariables=selectNode->getSelectVariables();


    ASSERT_EQ(true,true);
    //ASSERT_EQ(iri,Term::make_term(iriString));
}

TEST(BasicBgpsTests, BasicSelectQueryDefaultTest) {
    std::string query{
            "SELECT ?book ?nameY ?nickY\n"
            "WHERE\n"
            "  { ?book <dc:title> ?title ;\n"
            "         <ns:price> ?price ."
            "  }"};

    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);
    auto operands=selectNode->getOperands();
    auto bgps=selectNode->getBgps();
    auto  prefixes=selectNode->getPrefixes();
    auto subscript=selectNode->getSubscriptResult();
    auto selectVariables=selectNode->getSelectVariables();

    ASSERT_EQ(true,true);
}

TEST(BasicBgpsTests, BasicSelectQueryDistinctTest) {
    std::string query{
            "SELECT DISTINCT ?book ?nameY ?nickY\n"
            "WHERE\n"
            "  { ?book <dc:title> ?title ;\n"
            "         <ns:price> ?price ."
            "  }"};

    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);
    auto operands=selectNode->getOperands();
    auto bgps=selectNode->getBgps();
    auto  prefixes=selectNode->getPrefixes();
    auto subscript=selectNode->getSubscriptResult();
    auto selectVariables=selectNode->getSelectVariables();

    ASSERT_EQ(true,true);
}

TEST(BasicBgpsTests, BasicSelectQueryReducedTest) {
    std::string query{
            "SELECT REDUCED ?book ?nameY ?nickY\n"
            "WHERE\n"
            "  { ?book <dc:title> ?title ;\n"
            "         <ns:price> ?price ."
            "  }"};

    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);
    auto operands=selectNode->getOperands();
    auto bgps=selectNode->getBgps();
    auto  prefixes=selectNode->getPrefixes();
    auto subscript=selectNode->getSubscriptResult();
    auto selectVariables=selectNode->getSelectVariables();

    ASSERT_EQ(true,true);
}

TEST(BasicBgpsTests, BasicOptionalSelectQueryReducedTest) {
    std::string query{
            "SELECT REDUCED ?book ?nameY ?nickY\n"
            "WHERE\n"
            "  { ?book <dc:title> ?title ;\n"
            "          <ns:price> ?price ."
            "OPTIONAL { ?book <dc:author> <dc:title> }"
            "  }"};

    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);
    auto operands=selectNode->getOperands();
    auto bgps=selectNode->getBgps();
    auto  prefixes=selectNode->getPrefixes();
    auto subscript=selectNode->getSubscriptResult();
    auto selectVariables=selectNode->getSelectVariables();

    ASSERT_EQ(true,true);
}

TEST(BasicBgpsTests, ComplexOptionalSelectQueryReducedTest) {
    std::string query{
            "SELECT REDUCED ?book ?nameY ?nickY\n"
            "WHERE\n"
            "  { ?book <dc:title> ?title ;\n"
            "          <ns:price> ?price ."
            "OPTIONAL { ?book <dc:author> <dc:title> "
            "  OPTIONAL { ?book <dc:isbn> ?isbn. } \n } \n"
            "  }"};

    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);
    auto operands=selectNode->getOperands();
    auto bgps=selectNode->getBgps();
    auto  prefixes=selectNode->getPrefixes();
    auto subscript=selectNode->getSubscriptResult();
    auto selectVariables=selectNode->getSelectVariables();

    ASSERT_EQ(true,true);

}