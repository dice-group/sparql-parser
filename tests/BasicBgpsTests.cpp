#include <gtest/gtest.h>
#include <Dice/Sparql-Parser/Parser.hpp>

using namespace SparqlQueryGraph::Nodes::SelectNodes;



TEST(BasicBgpsTests, BasicSelectQueryDefaultTest) {
    std::string query{
            "SELECT ?book ?title ?price\n"
            "WHERE\n"
            "  { ?book <dc:title> ?title ;\n"
            "         <ns:price> ?price ."
            "  }"};

    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);
    auto operands=selectNode->getOperands();
    auto bgps=selectNode->getBgps();
    auto  prefixes=selectNode->getPrefixes();
    auto subscriptResult=selectNode->getSubscriptResult();
    auto selectVariables=selectNode->getSelectVariables();
    auto selectModifier=selectNode->getSelectModifier();

    std::vector<std::vector<char>> expectedOperands {{'a','b'},
                                                     {'a','c'}
                                                    };
    std::vector<TriplePatternElement> expectedBgps{TriplePatternElement(TripleVariable("book"),Term("<dc:title>"),TripleVariable("title")),
                                                   TriplePatternElement(TripleVariable("book"),Term("<ns:price>"),TripleVariable("price"))};

    std::map<std::string,std::string> expectedPrefixes{};
    std::vector<char> expectedSubscriptResult{'a','b','c'};
    std::vector<TripleVariable> expectedSelectVariables{ TripleVariable{"book"},
                                                         TripleVariable{"title"},
                                                         TripleVariable{"price"}
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

TEST(BasicBgpsTests, BasicSelectQueryDistinctTest) {
    std::string query{
            "SELECT DISTINCT ?book ?title ?price\n"
            "WHERE\n"
            "  { ?book <dc:title> ?title ;\n"
            "         <ns:price> ?price ."
            "  }"};

    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);
    auto operands=selectNode->getOperands();
    auto bgps=selectNode->getBgps();
    auto  prefixes=selectNode->getPrefixes();
    auto subscriptResult=selectNode->getSubscriptResult();
    auto selectVariables=selectNode->getSelectVariables();
    auto selectModifier=selectNode->getSelectModifier();

    std::vector<std::vector<char>> expectedOperands {{'a','b'},
                                                     {'a','c'}
    };
    std::vector<TriplePatternElement> expectedBgps{TriplePatternElement(TripleVariable("book"),Term("<dc:title>"),TripleVariable("title")),
                                                   TriplePatternElement(TripleVariable("book"),Term("<ns:price>"),TripleVariable("price"))};

    std::map<std::string,std::string> expectedPrefixes{};
    std::vector<char> expectedSubscriptResult{'a','b','c'};
    std::vector<TripleVariable> expectedSelectVariables{ TripleVariable{"book"},
                                                         TripleVariable{"title"},
                                                         TripleVariable{"price"}
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
    ASSERT_EQ(selectModifier,SelectModifier::DISTINCT);
}

TEST(BasicBgpsTests, BasicSelectQueryReducedTest) {
    std::string query{
            "SELECT REDUCED ?book ?title ?price\n"
            "WHERE\n"
            "  { ?book <dc:title> ?title ;\n"
            "         <ns:price> ?price ."
            "  }"};

    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);
    auto operands=selectNode->getOperands();
    auto bgps=selectNode->getBgps();
    auto  prefixes=selectNode->getPrefixes();
    auto subscriptResult=selectNode->getSubscriptResult();
    auto selectVariables=selectNode->getSelectVariables();
    auto selectModifier=selectNode->getSelectModifier();

    std::vector<std::vector<char>> expectedOperands {{'a','b'},
                                                     {'a','c'}
    };
    std::vector<TriplePatternElement> expectedBgps{TriplePatternElement(TripleVariable("book"),Term("<dc:title>"),TripleVariable("title")),
                                                   TriplePatternElement(TripleVariable("book"),Term("<ns:price>"),TripleVariable("price"))};

    std::map<std::string,std::string> expectedPrefixes{};
    std::vector<char> expectedSubscriptResult{'a','b','c'};
    std::vector<TripleVariable> expectedSelectVariables{ TripleVariable{"book"},
                                                         TripleVariable{"title"},
                                                         TripleVariable{"price"}
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
    ASSERT_EQ(selectModifier,SelectModifier::REDUCE);
}


TEST(BasicBgpsTests, multipleBgps) {
    std::string query{
            "SELECT ?s ?e WHERE {?s <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://localhost/vocabulary/bench/Journal>.?s <http://swrc.ontoware.org/ontology#editor> ?e}"};


    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);
    auto operands=selectNode->getOperands();
    auto bgps=selectNode->getBgps();
    auto  prefixes=selectNode->getPrefixes();
    auto subscriptResult=selectNode->getSubscriptResult();
    auto selectVariables=selectNode->getSelectVariables();
    auto selectModifier=selectNode->getSelectModifier();

    std::vector<std::vector<char>> expectedOperands {{'a'},
                                                     {'a','b'}
    };
    std::vector<TriplePatternElement> expectedBgps{TriplePatternElement(TripleVariable("s"),Term("<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>"),Term("<http://localhost/vocabulary/bench/Journal>")),
                                                   TriplePatternElement(TripleVariable("s"),Term("<http://swrc.ontoware.org/ontology#editor>"),TripleVariable("e"))};

    std::map<std::string,std::string> expectedPrefixes{};
    std::vector<char> expectedSubscriptResult{'a','b'};
    std::vector<TripleVariable> expectedSelectVariables{ TripleVariable{"s"},
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


TEST(BasicBgpsTests, multipleBgps2) {
    std::string query{
            "SELECT DISTINCT ?s ?p ?o WHERE {?s <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://localhost/vocabulary/bench/Article> . ?s ?p ?o .}"};


    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);
    auto operands=selectNode->getOperands();
    auto bgps=selectNode->getBgps();
    auto  prefixes=selectNode->getPrefixes();
    auto subscriptResult=selectNode->getSubscriptResult();
    auto selectVariables=selectNode->getSelectVariables();
    auto selectModifier=selectNode->getSelectModifier();

    std::vector<std::vector<char>> expectedOperands {{'a'},
                                                     {'a','b','c'}
    };
    std::vector<TriplePatternElement> expectedBgps{TriplePatternElement(TripleVariable("s"),Term("<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>"),Term("<http://localhost/vocabulary/bench/Article>")),
                                                   TriplePatternElement(TripleVariable("s"),TripleVariable("p"),TripleVariable("o"))};

    std::map<std::string,std::string> expectedPrefixes{};
    std::vector<char> expectedSubscriptResult{'a','b','c'};
    std::vector<TripleVariable> expectedSelectVariables{ TripleVariable{"s"},
                                                         TripleVariable{"p"},
                                                         TripleVariable{"o"}
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
    ASSERT_EQ(selectModifier,SelectModifier::DISTINCT);

}
