#include <gtest/gtest.h>
#include <Dice/Sparql-Parser/Parser.hpp>

#include "TestUtilites.hpp"

using namespace SparqlQueryGraph::Nodes::SelectNodes;


TEST(BasicOptionalPatternTests, multipleBgpsBeforeOptional) {
    std::string query{
            "SELECT ?s ?t ?e WHERE {?s <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://localhost/vocabulary/bench/Journal> . ?s <http://purl.org/dc/elements/1.1/title> ?t . OPTIONAL{?s <http://swrc.ontoware.org/ontology#editor> ?e}}"};


    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);

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

    SelectModifier selectModifier=SelectModifier::NONE;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);
}

TEST(BasicOptionalPatternTests, multipleBgpsInsideOptional) {
    std::string query{
            "SELECT ?a ?t ?j ?e WHERE {?a <http://purl.org/dc/elements/1.1/title> ?t . OPTIONAL { ?a <http://swrc.ontoware.org/ontology#journal> ?j . ?j <http://swrc.ontoware.org/ontology#editor> ?e  } }"};


    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);

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

    SelectModifier selectModifier=SelectModifier::NONE;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);
}

TEST(BasicOptionalPatternTests, multipleBgpsConnectedWithSemiColon1) {
    std::string query{
            "PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> PREFIX swc: <http://data.semanticweb.org/ns/swc/ontology#> PREFIX foaf: <http://xmlns.com/foaf/0.1/> SELECT DISTINCT ?person ?paper WHERE { ?role swc:isRoleAt <http://data.semanticweb.org/conference/eswc/2010> ; rdf:type swc:Chair ; swc:heldBy ?person OPTIONAL { ?person foaf:made ?paper } }"};


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

TEST(BasicOptionalPatternTests, multipleBgpsConnectedWithSemiColon2) {
    std::string query{
            "PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> PREFIX swc: <http://data.semanticweb.org/ns/swc/ontology#> PREFIX foaf: <http://xmlns.com/foaf/0.1/> PREFIX dce: <http://purl.org/dc/elements/1.1/> SELECT DISTINCT ?name ?title ?author WHERE { ?role swc:isRoleAt <http://data.semanticweb.org/conference/eswc/2010> ; rdf:type swc:Chair ; swc:heldBy ?person . ?person foaf:name ?name OPTIONAL { ?person foaf:made ?paper . ?paper dce:title ?title } OPTIONAL { ?paper dce:creator ?author } }"};


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

TEST(BasicOptionalPatternTests, multipleBgpsConnectedWithSemiColon3) {
    std::string query{
            "PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#> PREFIX swc: <http://data.semanticweb.org/ns/swc/ontology#> PREFIX dce: <http://purl.org/dc/elements/1.1/> SELECT ?el ?et WHERE { <http://data.semanticweb.org/workshop/usewod/2012> swc:hasLocation ?l . ?e swc:hasLocation ?l ; rdfs:label ?el OPTIONAL { ?e dce:title ?et } }"};


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

TEST(BasicOptionalPatternTests, multipleBgpsConnectedWithSemiColon4) {
    std::string query{
            "PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#> PREFIX swc: <http://data.semanticweb.org/ns/swc/ontology#> PREFIX dce: <http://purl.org/dc/elements/1.1/> SELECT ?el ?et WHERE { <http://data.semanticweb.org/workshop/usewod/2012> swc:hasLocation ?l . ?e swc:hasLocation ?l ; rdfs:label ?el }"};


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