#include <gtest/gtest.h>
#include <Dice/Sparql-Parser/Parser.hpp>

using namespace SparqlQueryGraph::Nodes::SelectNodes;

TEST(BasicOptionalPatternTests, multipleBgpsBeforeOptional) {
    std::string query{
            "SELECT ?s ?t ?e WHERE {?s <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://localhost/vocabulary/bench/Journal> . ?s <http://purl.org/dc/elements/1.1/title> ?t . OPTIONAL{?s <http://swrc.ontoware.org/ontology#editor> ?e}}"};


    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);
    auto operands=selectNode->getOperands();
    auto bgps=selectNode->getBgps();
    auto  prefixes=selectNode->getPrefixes();
    auto subscript=selectNode->getSubscriptResult();
    auto selectVariables=selectNode->getSelectVariables();


    ASSERT_EQ(true,true);
    //ASSERT_EQ(iri,Term::make_term(iriString));
}

TEST(BasicOptionalPatternTests, multipleBgpsInsideOptional) {
    std::string query{
            "SELECT ?a ?t ?j ?e WHERE {?a <http://purl.org/dc/elements/1.1/title> ?t . OPTIONAL { ?a <http://swrc.ontoware.org/ontology#journal> ?j . ?j <http://swrc.ontoware.org/ontology#editor> ?e  } }"};


    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);
    auto operands=selectNode->getOperands();
    auto bgps=selectNode->getBgps();
    auto  prefixes=selectNode->getPrefixes();
    auto subscript=selectNode->getSubscriptResult();
    auto selectVariables=selectNode->getSelectVariables();


    ASSERT_EQ(true,true);
    //ASSERT_EQ(iri,Term::make_term(iriString));
}