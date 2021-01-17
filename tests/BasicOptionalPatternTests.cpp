#include <gtest/gtest.h>
#include <Dice/sparql-parser/Parser.hpp>

#include "TestUtilites.hpp"

using namespace SparqlQueryGraph::Nodes::SelectNodes;

TEST(BasicOptionalPatternTests, emptyOptionalPattern) {
    std::string query{
            "PREFIX wde: <http://www.wikidata.org/entity/> PREFIX wdt: <http://www.wikidata.org/prop/direct/>  \n"
            " SELECT * WHERE { ?var1 wdt:P31 wde:Q5 OPTIONAL { } }"};


    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);

    std::vector<std::vector<char>> expectedOperands {{'a'},
                                                     {'['},
                                                     {']'}
    };

    std::vector<TriplePatternElement> expectedBgps{TriplePatternElement(TripleVariable("var1") ,
                                                                        Term("<http://www.wikidata.org/prop/direct/P31>"),
                                                                        Term("<http://www.wikidata.org/entity/Q5>")
                                                                        )};

    std::map<std::string,std::string> expectedPrefixes{{"wde", "http://www.wikidata.org/entity/"},
                                                       {"wdt", "http://www.wikidata.org/prop/direct/"}
    };

    std::vector<char> expectedSubscriptResult{'a'};
    std::vector<TripleVariable> expectedSelectVariables{ TripleVariable{"var1"} };

    SelectModifier selectModifier=SelectModifier::NONE;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);

}

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

TEST(BasicOptionalPatternTests, multipleBgpsConnectedWithSemiColonAndOneOptional) {
    std::string query{
            "PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> PREFIX swc: <http://data.semanticweb.org/ns/swc/ontology#> PREFIX foaf: <http://xmlns.com/foaf/0.1/> \n"
            "SELECT DISTINCT ?person ?paper WHERE { ?role swc:isRoleAt <http://data.semanticweb.org/conference/eswc/2010> ; rdf:type swc:Chair ; swc:heldBy ?person OPTIONAL { ?person foaf:made ?paper } }"};


    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);

    std::vector<std::vector<char>> expectedOperands {{'a'},
                                                     {'a',},
                                                     {'a','b'},
                                                     {'['},
                                                     {'b','c'},
                                                     {']'}
    };
    std::vector<TriplePatternElement> expectedBgps{TriplePatternElement(TripleVariable("role"),Term("<http://data.semanticweb.org/ns/swc/ontology#isRoleAt>"),Term("<http://data.semanticweb.org/conference/eswc/2010>")),
                                                   TriplePatternElement(TripleVariable("role"),Term("<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>"),Term("<http://data.semanticweb.org/ns/swc/ontology#Chair>")),
                                                   TriplePatternElement(TripleVariable("role"),Term("<http://data.semanticweb.org/ns/swc/ontology#heldBy>"),TripleVariable("person")),
                                                   TriplePatternElement(TripleVariable("person"),Term("<http://xmlns.com/foaf/0.1/made>"),TripleVariable("paper"))};

    std::map<std::string,std::string> expectedPrefixes{ {"rdf","http://www.w3.org/1999/02/22-rdf-syntax-ns#"},
                                                        {"swc","http://data.semanticweb.org/ns/swc/ontology#"},
                                                        {"foaf","http://xmlns.com/foaf/0.1/"}
    };
    std::vector<char> expectedSubscriptResult{'b','c'};
    std::vector<TripleVariable> expectedSelectVariables{ TripleVariable{"person"},
                                                         TripleVariable{"paper"}
    };

    SelectModifier selectModifier=SelectModifier::DISTINCT;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);

}

TEST(BasicOptionalPatternTests, multipleBgpsConnectedWithSemiColonAndTwoOptional) {
    std::string query{
            "PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> PREFIX swc: <http://data.semanticweb.org/ns/swc/ontology#> PREFIX foaf: <http://xmlns.com/foaf/0.1/> PREFIX dce: <http://purl.org/dc/elements/1.1/> \n"
            " SELECT DISTINCT ?name ?title ?author WHERE { ?role swc:isRoleAt <http://data.semanticweb.org/conference/eswc/2010> ; rdf:type swc:Chair ; swc:heldBy ?person . ?person foaf:name ?name OPTIONAL { ?person foaf:made ?paper . ?paper dce:title ?title } OPTIONAL { ?paper dce:creator ?author } }"};


    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);

    std::vector<std::vector<char>> expectedOperands {{'a'},
                                                     {'a',},
                                                     {'a','b'},
                                                     {'b','c'},
                                                     {'['},
                                                     {'b','d'},
                                                     {'d','e'},
                                                     {']'},
                                                     {'['},
                                                     {'d','f'},
                                                     {']'}
    };
    std::vector<TriplePatternElement> expectedBgps{TriplePatternElement(TripleVariable("role"),Term("<http://data.semanticweb.org/ns/swc/ontology#isRoleAt>"),Term("<http://data.semanticweb.org/conference/eswc/2010>")),
                                                   TriplePatternElement(TripleVariable("role"),Term("<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>"),Term("<http://data.semanticweb.org/ns/swc/ontology#Chair>")),
                                                   TriplePatternElement(TripleVariable("role"),Term("<http://data.semanticweb.org/ns/swc/ontology#heldBy>"),TripleVariable("person")),
                                                   TriplePatternElement(TripleVariable("person"),Term("<http://xmlns.com/foaf/0.1/name>"),TripleVariable("name")),
                                                   TriplePatternElement(TripleVariable("person"),Term("<http://xmlns.com/foaf/0.1/made>"),TripleVariable("paper")),
                                                   TriplePatternElement(TripleVariable("paper"),Term("<http://purl.org/dc/elements/1.1/title>"),TripleVariable("title")),
                                                   TriplePatternElement(TripleVariable("paper"),Term("<http://purl.org/dc/elements/1.1/creator>"),TripleVariable("author"))};

    std::map<std::string,std::string> expectedPrefixes{ {"rdf","http://www.w3.org/1999/02/22-rdf-syntax-ns#"},
                                                        {"swc","http://data.semanticweb.org/ns/swc/ontology#"},
                                                        {"foaf","http://xmlns.com/foaf/0.1/"},
                                                        {"dce","http://purl.org/dc/elements/1.1/"}
    };
    std::vector<char> expectedSubscriptResult{'c','e','f'};
    std::vector<TripleVariable> expectedSelectVariables{ TripleVariable{"name"},
                                                         TripleVariable{"title"},
                                                         TripleVariable{"author"}
    };

    SelectModifier selectModifier=SelectModifier::DISTINCT;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);

}

TEST(BasicOptionalPatternTests, multipleBgpsConnectedWithSemiColon3) {
    std::string query{
            "PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#> PREFIX swc: <http://data.semanticweb.org/ns/swc/ontology#> PREFIX dce: <http://purl.org/dc/elements/1.1/> \n"
            " SELECT ?el ?et WHERE { <http://data.semanticweb.org/workshop/usewod/2012> swc:hasLocation ?l . ?e swc:hasLocation ?l ; rdfs:label ?el OPTIONAL { ?e dce:title ?et } }"};


    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);

    std::vector<std::vector<char>> expectedOperands {{'a'},
                                                     {'b','a'},
                                                     {'b','c'},
                                                     {'['},
                                                     {'b','d'},
                                                     {']'}
    };

    std::vector<TriplePatternElement> expectedBgps{TriplePatternElement(Term("<http://data.semanticweb.org/workshop/usewod/2012>"),Term("<http://data.semanticweb.org/ns/swc/ontology#hasLocation>"),TripleVariable("l")),
                                                   TriplePatternElement(TripleVariable("e"),Term("<http://data.semanticweb.org/ns/swc/ontology#hasLocation>"),TripleVariable("l")),
                                                   TriplePatternElement(TripleVariable("e"),Term("<http://www.w3.org/2000/01/rdf-schema#label>"),TripleVariable("el")),
                                                   TriplePatternElement(TripleVariable("e"),Term("<http://purl.org/dc/elements/1.1/title>"),TripleVariable("et"))
    };

    std::map<std::string,std::string> expectedPrefixes{{"rdfs", "http://www.w3.org/2000/01/rdf-schema#"},
                                                       {"swc",  "http://data.semanticweb.org/ns/swc/ontology#"},
                                                       {"dce",  "http://purl.org/dc/elements/1.1/"}
    };

    std::vector<char> expectedSubscriptResult{'c','d'};
    std::vector<TripleVariable> expectedSelectVariables{ TripleVariable{"el"},
                                                         TripleVariable{"et"}
    };

    SelectModifier selectModifier=SelectModifier::NONE;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);

}



