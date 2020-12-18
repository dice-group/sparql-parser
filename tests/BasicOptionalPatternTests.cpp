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
    auto subscriptResult=selectNode->getSubscriptResult();
    auto selectVariables=selectNode->getSelectVariables();
    auto selectModifier=selectNode->getSelectModifier();

    std::vector<std::vector<char>> expectedOperands {{'a'},
                                                     {'a','b'},
                                                     {'['},
                                                     {'a','c'},
                                                     {']'}
    };
    std::vector<TriplePatternElement> expectedBgps{TriplePatternElement(TripleVariable("s"),Term("<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>"),Term("<http://localhost/vocabulary/bench/Journal>")),
                                                   TriplePatternElement(TripleVariable("s"),Term("<http://purl.org/dc/elements/1.1/title>"),TripleVariable("t")),
                                                   TriplePatternElement(TripleVariable("s"),Term("<http://swrc.ontoware.org/ontology#editor>"),TripleVariable("e"))
                                                   };

    std::map<std::string,std::string> expectedPrefixes{};
    std::vector<char> expectedSubscriptResult{'a','b','c'};
    std::vector<TripleVariable> expectedSelectVariables{ TripleVariable{"s"},
                                                         TripleVariable{"t"},
                                                         TripleVariable{"e"}
    };

    bool operandsEqual=std::equal(operands.begin(),operands.end(),expectedOperands.begin());
    bool bgpsEqual=std::equal(bgps.begin(),bgps.end(),expectedBgps.begin());
    bool prefixesEqual=std::equal(prefixes.begin(),prefixes.end(),expectedPrefixes.begin());
    bool subscriptResultEqual=std::equal(subscriptResult.begin(),subscriptResult.end(),expectedSubscriptResult.begin());
    bool selectVariablesEqual=std::equal(selectVariables.begin(),selectVariables.end(),expectedSelectVariables.begin());

    ASSERT_EQ(operandsEqual,true);
    ASSERT_EQ(bgpsEqual,true);
    ASSERT_EQ(prefixesEqual,true);
    ASSERT_EQ(subscriptResultEqual,true);
    ASSERT_EQ(selectVariablesEqual,true);
    ASSERT_EQ(selectModifier,SelectModifier::NONE);
}

TEST(BasicOptionalPatternTests, multipleBgpsInsideOptional) {
    std::string query{
            "SELECT ?a ?t ?j ?e WHERE {?a <http://purl.org/dc/elements/1.1/title> ?t . OPTIONAL { ?a <http://swrc.ontoware.org/ontology#journal> ?j . ?j <http://swrc.ontoware.org/ontology#editor> ?e  } }"};


    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);
    auto operands=selectNode->getOperands();
    auto bgps=selectNode->getBgps();
    auto  prefixes=selectNode->getPrefixes();
    auto subscriptResult=selectNode->getSubscriptResult();
    auto selectVariables=selectNode->getSelectVariables();
    auto selectModifier=selectNode->getSelectModifier();

    std::vector<std::vector<char>> expectedOperands {{'a','b'},
                                                     {'['},
                                                     {'a','c'},
                                                     {'c','d'},
                                                     {']'}
    };
    std::vector<TriplePatternElement> expectedBgps{TriplePatternElement(TripleVariable("a"),Term("<http://purl.org/dc/elements/1.1/title>"),TripleVariable("t")),
                                                   TriplePatternElement(TripleVariable("a"),Term("<http://swrc.ontoware.org/ontology#journal>"),TripleVariable("j")),
                                                   TriplePatternElement(TripleVariable("j"),Term("<http://swrc.ontoware.org/ontology#editor>"),TripleVariable("e"))
    };

    std::map<std::string,std::string> expectedPrefixes{};
    std::vector<char> expectedSubscriptResult{'a','b','c','d'};
    std::vector<TripleVariable> expectedSelectVariables{ TripleVariable{"a"},
                                                         TripleVariable{"t"},
                                                         TripleVariable{"j"},
                                                         TripleVariable{"e"}
    };

    bool operandsEqual=std::equal(operands.begin(),operands.end(),expectedOperands.begin());
    bool bgpsEqual=std::equal(bgps.begin(),bgps.end(),expectedBgps.begin());
    bool prefixesEqual=std::equal(prefixes.begin(),prefixes.end(),expectedPrefixes.begin());
    bool subscriptResultEqual=std::equal(subscriptResult.begin(),subscriptResult.end(),expectedSubscriptResult.begin());
    bool selectVariablesEqual=std::equal(selectVariables.begin(),selectVariables.end(),expectedSelectVariables.begin());

    ASSERT_EQ(operandsEqual,true);
    ASSERT_EQ(bgpsEqual,true);
    ASSERT_EQ(prefixesEqual,true);
    ASSERT_EQ(subscriptResultEqual,true);
    ASSERT_EQ(selectVariablesEqual,true);
    ASSERT_EQ(selectModifier,SelectModifier::NONE);
}