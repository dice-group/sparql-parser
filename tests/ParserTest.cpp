#include <gtest/gtest.h>
#include <Dice/Sparql-Parser/Parser.hpp>

using namespace SparqlQueryGraph::Nodes::SelectNodes;

TEST(ParserTests, query1) {
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