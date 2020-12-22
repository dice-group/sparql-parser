#include <gtest/gtest.h>
#include <Dice/Sparql-Parser/Parser.hpp>
#include <search.h>

#include "TestUtilites.hpp"

using namespace SparqlQueryGraph::Nodes::SelectNodes;


TEST(BasicBgpsTests, BasicSelectQueryDefaultTest) {
    std::string query{
            "SELECT ?book ?title ?price\n"
            "WHERE\n"
            "  { ?book <dc:title> ?title ;\n"
            "         <ns:price> ?price ."
            "  }"};

    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);

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
    SelectModifier selectModifier=SelectModifier::NONE;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);

}

TEST(BasicBgpsTests, BasicSelectQueryDistinctTest) {
    std::string query{
            "SELECT DISTINCT ?book ?title ?price\n"
            "WHERE\n"
            "  { ?book <dc:title> ?title ;\n"
            "         <ns:price> ?price ."
            "  }"};

    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);

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

    SelectModifier selectModifier=SelectModifier::DISTINCT;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);
}

TEST(BasicBgpsTests, BasicSelectQueryReducedTest) {
    std::string query{
            "SELECT REDUCED ?book ?title ?price\n"
            "WHERE\n"
            "  { ?book <dc:title> ?title ;\n"
            "         <ns:price> ?price ."
            "  }"};

    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);


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

    SelectModifier selectModifier=SelectModifier::REDUCE;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);
}

TEST(BasicBgpsTests, multipleBgps) {
    std::string query{
            "SELECT ?s ?e WHERE {?s <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://localhost/vocabulary/bench/Journal>.?s <http://swrc.ontoware.org/ontology#editor> ?e}"};


    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);

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

    SelectModifier selectModifier=SelectModifier::NONE;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);
}

TEST(BasicBgpsTests, multipleBgps2) {
    std::string query{
            "SELECT DISTINCT ?s ?p ?o WHERE {?s <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://localhost/vocabulary/bench/Article> . ?s ?p ?o .}"};


    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);

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

    SelectModifier selectModifier=SelectModifier::DISTINCT;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);

}



TEST(BasicBgpsTests, multipleBgpsConnectedWithSemiColon) {
    std::string query{
            "PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#> PREFIX swc: <http://data.semanticweb.org/ns/swc/ontology#> PREFIX dce: <http://purl.org/dc/elements/1.1/> \n"
            " SELECT ?el ?et WHERE { <http://data.semanticweb.org/workshop/usewod/2012> swc:hasLocation ?l . ?e swc:hasLocation ?l ; rdfs:label ?el }"};


    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);

    std::vector<std::vector<char>> expectedOperands {{'a'},
                                                     {'b','a'},
                                                     {'b','c'}
    };
    std::vector<TriplePatternElement> expectedBgps{TriplePatternElement(Term("<http://data.semanticweb.org/workshop/usewod/2012>"),Term("<http://data.semanticweb.org/ns/swc/ontology#hasLocation>"),TripleVariable("l")),
                                                   TriplePatternElement(TripleVariable("e"),Term("<http://data.semanticweb.org/ns/swc/ontology#hasLocation>"),TripleVariable("l")),
                                                   TriplePatternElement(TripleVariable("e"),Term("<http://www.w3.org/2000/01/rdf-schema#label>"),TripleVariable("el"))
    };

    std::map<std::string,std::string> expectedPrefixes{ {"rdfs","http://www.w3.org/2000/01/rdf-schema#"},
                                                        {"swc","http://data.semanticweb.org/ns/swc/ontology#"},
                                                        {"dce","http://purl.org/dc/elements/1.1/"}
    };
    std::vector<char> expectedSubscriptResult{'c','d'};
    std::vector<TripleVariable> expectedSelectVariables{ TripleVariable{"el"},
                                                         TripleVariable{"et"}
    };

    SelectModifier selectModifier=SelectModifier::NONE;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);

}

