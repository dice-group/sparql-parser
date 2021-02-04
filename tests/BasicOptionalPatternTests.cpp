#include <gtest/gtest.h>

#include <Dice/sparql-parser/Parser.hpp>
#include <Dice/RDF/ParseTerm.hpp>

#include "TestUtilites.hpp"

using namespace Dice::sparql::Nodes::QueryNodes::SelectNodes;
using namespace Dice::sparql_parser;
using namespace Dice;

TEST(BasicOptionalPatternTests, emptyOptionalPattern) {
    std::string query{
            "PREFIX wde: <http://www.wikidata.org/entity/> PREFIX wdt: <http://www.wikidata.org/prop/direct/>  \n"
            " SELECT * WHERE { ?var1 wdt:P31 wde:Q5 OPTIONAL { } }"};


    std::shared_ptr<SelectNode> selectNode=sparql_parser::parseSelectQuery(query);

    std::vector<std::vector<char>> expectedOperands {{'a'},
                                                     {'['},
                                                     {']'}
    };

    std::vector<sparql::TriplePattern> expectedBgps{sparql::TriplePattern(sparql::Variable("var1") ,
                                                                        rdf::parse_term("<http://www.wikidata.org/prop/direct/P31>"),
                                                                        rdf::parse_term("<http://www.wikidata.org/entity/Q5>")
                                                                        )};

    robin_hood::unordered_map<std::string,std::string> expectedPrefixes{{"wde", "http://www.wikidata.org/entity/"},
                                                       {"wdt", "http://www.wikidata.org/prop/direct/"}
    };

    std::vector<char> expectedSubscriptResult{'a'};
    std::vector<sparql::Variable> expectedSelectVariables{ sparql::Variable{"var1"} };

    SelectModifier selectModifier=SelectModifier::NONE;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);

}

TEST(BasicOptionalPatternTests, multipleBgpsBeforeOptional) {
    std::string query{
            "SELECT ?s ?t ?e WHERE"
            " {?s <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://localhost/vocabulary/bench/Journal> ."
            " ?s <http://purl.org/dc/elements/1.1/title> ?t ."
            " OPTIONAL{?s <http://swrc.ontoware.org/ontology#editor> ?e}}"};


    std::shared_ptr<SelectNode> selectNode=sparql_parser::parseSelectQuery(query);

    std::vector<std::vector<char>> expectedOperands {{'a'},
                                                     {'a','b'},
                                                     {'['},
                                                     {'a','c'},
                                                     {']'}
    };
    std::vector<sparql::TriplePattern> expectedBgps{sparql::TriplePattern(sparql::Variable("s"),rdf::parse_term("<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>"),rdf::parse_term("<http://localhost/vocabulary/bench/Journal>")),
                                                   sparql::TriplePattern(sparql::Variable("s"),rdf::parse_term("<http://purl.org/dc/elements/1.1/title>"),sparql::Variable("t")),
                                                   sparql::TriplePattern(sparql::Variable("s"),rdf::parse_term("<http://swrc.ontoware.org/ontology#editor>"),sparql::Variable("e"))
                                                   };

    robin_hood::unordered_map<std::string,std::string> expectedPrefixes{};
    std::vector<char> expectedSubscriptResult{'a','b','c'};
    std::vector<sparql::Variable> expectedSelectVariables{ sparql::Variable{"s"},
                                                         sparql::Variable{"t"},
                                                         sparql::Variable{"e"}
    };

    SelectModifier selectModifier=SelectModifier::NONE;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);
}

TEST(BasicOptionalPatternTests, multipleBgpsInsideOptional) {
    std::string query{
            "SELECT ?a ?t ?j ?e WHERE {?a <http://purl.org/dc/elements/1.1/title> ?t . OPTIONAL { ?a <http://swrc.ontoware.org/ontology#journal> ?j . ?j <http://swrc.ontoware.org/ontology#editor> ?e  } }"};


    std::shared_ptr<SelectNode> selectNode=sparql_parser::parseSelectQuery(query);

    std::vector<std::vector<char>> expectedOperands {{'a','b'},
                                                     {'['},
                                                     {'a','c'},
                                                     {'c','d'},
                                                     {']'}
    };
    std::vector<sparql::TriplePattern> expectedBgps{sparql::TriplePattern(sparql::Variable("a"),rdf::parse_term("<http://purl.org/dc/elements/1.1/title>"),sparql::Variable("t")),
                                                   sparql::TriplePattern(sparql::Variable("a"),rdf::parse_term("<http://swrc.ontoware.org/ontology#journal>"),sparql::Variable("j")),
                                                   sparql::TriplePattern(sparql::Variable("j"),rdf::parse_term("<http://swrc.ontoware.org/ontology#editor>"),sparql::Variable("e"))
    };

    robin_hood::unordered_map<std::string,std::string> expectedPrefixes{};
    std::vector<char> expectedSubscriptResult{'a','b','c','d'};
    std::vector<sparql::Variable> expectedSelectVariables{ sparql::Variable{"a"},
                                                         sparql::Variable{"t"},
                                                         sparql::Variable{"j"},
                                                         sparql::Variable{"e"}
    };

    SelectModifier selectModifier=SelectModifier::NONE;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);
}

TEST(BasicOptionalPatternTests, multipleBgpsConnectedWithSemiColonAndOneOptional) {
    std::string query{
            "PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> PREFIX swc: <http://data.semanticweb.org/ns/swc/ontology#> PREFIX foaf: <http://xmlns.com/foaf/0.1/> \n"
            "SELECT DISTINCT ?person ?paper WHERE { ?role swc:isRoleAt <http://data.semanticweb.org/conference/eswc/2010> ; rdf:type swc:Chair ; swc:heldBy ?person OPTIONAL { ?person foaf:made ?paper } }"};


    std::shared_ptr<SelectNode> selectNode=sparql_parser::parseSelectQuery(query);

    std::vector<std::vector<char>> expectedOperands {{'a'},
                                                     {'a',},
                                                     {'a','b'},
                                                     {'['},
                                                     {'b','c'},
                                                     {']'}
    };
    std::vector<sparql::TriplePattern> expectedBgps{sparql::TriplePattern(sparql::Variable("role"),rdf::parse_term("<http://data.semanticweb.org/ns/swc/ontology#isRoleAt>"),rdf::parse_term("<http://data.semanticweb.org/conference/eswc/2010>")),
                                                   sparql::TriplePattern(sparql::Variable("role"),rdf::parse_term("<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>"),rdf::parse_term("<http://data.semanticweb.org/ns/swc/ontology#Chair>")),
                                                   sparql::TriplePattern(sparql::Variable("role"),rdf::parse_term("<http://data.semanticweb.org/ns/swc/ontology#heldBy>"),sparql::Variable("person")),
                                                   sparql::TriplePattern(sparql::Variable("person"),rdf::parse_term("<http://xmlns.com/foaf/0.1/made>"),sparql::Variable("paper"))};

    robin_hood::unordered_map<std::string,std::string> expectedPrefixes{ {"rdf","http://www.w3.org/1999/02/22-rdf-syntax-ns#"},
                                                        {"swc","http://data.semanticweb.org/ns/swc/ontology#"},
                                                        {"foaf","http://xmlns.com/foaf/0.1/"}
    };
    std::vector<char> expectedSubscriptResult{'b','c'};
    std::vector<sparql::Variable> expectedSelectVariables{ sparql::Variable{"person"},
                                                         sparql::Variable{"paper"}
    };

    SelectModifier selectModifier=SelectModifier::DISTINCT;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);

}

TEST(BasicOptionalPatternTests, multipleBgpsConnectedWithSemiColonAndTwoOptional) {
    std::string query{
            "PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> PREFIX swc: <http://data.semanticweb.org/ns/swc/ontology#> PREFIX foaf: <http://xmlns.com/foaf/0.1/> PREFIX dce: <http://purl.org/dc/elements/1.1/> \n"
            " SELECT DISTINCT ?name ?title ?author WHERE { ?role swc:isRoleAt <http://data.semanticweb.org/conference/eswc/2010> ; rdf:type swc:Chair ; swc:heldBy ?person . ?person foaf:name ?name OPTIONAL { ?person foaf:made ?paper . ?paper dce:title ?title } OPTIONAL { ?paper dce:creator ?author } }"};


    std::shared_ptr<SelectNode> selectNode=sparql_parser::parseSelectQuery(query);

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
    std::vector<sparql::TriplePattern> expectedBgps{sparql::TriplePattern(sparql::Variable("role"),rdf::parse_term("<http://data.semanticweb.org/ns/swc/ontology#isRoleAt>"),rdf::parse_term("<http://data.semanticweb.org/conference/eswc/2010>")),
                                                   sparql::TriplePattern(sparql::Variable("role"),rdf::parse_term("<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>"),rdf::parse_term("<http://data.semanticweb.org/ns/swc/ontology#Chair>")),
                                                   sparql::TriplePattern(sparql::Variable("role"),rdf::parse_term("<http://data.semanticweb.org/ns/swc/ontology#heldBy>"),sparql::Variable("person")),
                                                   sparql::TriplePattern(sparql::Variable("person"),rdf::parse_term("<http://xmlns.com/foaf/0.1/name>"),sparql::Variable("name")),
                                                   sparql::TriplePattern(sparql::Variable("person"),rdf::parse_term("<http://xmlns.com/foaf/0.1/made>"),sparql::Variable("paper")),
                                                   sparql::TriplePattern(sparql::Variable("paper"),rdf::parse_term("<http://purl.org/dc/elements/1.1/title>"),sparql::Variable("title")),
                                                   sparql::TriplePattern(sparql::Variable("paper"),rdf::parse_term("<http://purl.org/dc/elements/1.1/creator>"),sparql::Variable("author"))};

    robin_hood::unordered_map<std::string,std::string> expectedPrefixes{ {"rdf","http://www.w3.org/1999/02/22-rdf-syntax-ns#"},
                                                        {"swc","http://data.semanticweb.org/ns/swc/ontology#"},
                                                        {"foaf","http://xmlns.com/foaf/0.1/"},
                                                        {"dce","http://purl.org/dc/elements/1.1/"}
    };
    std::vector<char> expectedSubscriptResult{'c','e','f'};
    std::vector<sparql::Variable> expectedSelectVariables{ sparql::Variable{"name"},
                                                         sparql::Variable{"title"},
                                                         sparql::Variable{"author"}
    };

    SelectModifier selectModifier=SelectModifier::DISTINCT;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);

}

TEST(BasicOptionalPatternTests, multipleBgpsConnectedWithSemiColon3) {
    std::string query{
            "PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#> PREFIX swc: <http://data.semanticweb.org/ns/swc/ontology#> PREFIX dce: <http://purl.org/dc/elements/1.1/> \n"
            " SELECT ?el ?et WHERE { <http://data.semanticweb.org/workshop/usewod/2012> swc:hasLocation ?l . ?e swc:hasLocation ?l ; rdfs:label ?el OPTIONAL { ?e dce:title ?et } }"};


    std::shared_ptr<SelectNode> selectNode=sparql_parser::parseSelectQuery(query);

    std::vector<std::vector<char>> expectedOperands {{'a'},
                                                     {'b','a'},
                                                     {'b','c'},
                                                     {'['},
                                                     {'b','d'},
                                                     {']'}
    };

    std::vector<sparql::TriplePattern> expectedBgps{sparql::TriplePattern(rdf::parse_term("<http://data.semanticweb.org/workshop/usewod/2012>"),rdf::parse_term("<http://data.semanticweb.org/ns/swc/ontology#hasLocation>"),sparql::Variable("l")),
                                                   sparql::TriplePattern(sparql::Variable("e"),rdf::parse_term("<http://data.semanticweb.org/ns/swc/ontology#hasLocation>"),sparql::Variable("l")),
                                                   sparql::TriplePattern(sparql::Variable("e"),rdf::parse_term("<http://www.w3.org/2000/01/rdf-schema#label>"),sparql::Variable("el")),
                                                   sparql::TriplePattern(sparql::Variable("e"),rdf::parse_term("<http://purl.org/dc/elements/1.1/title>"),sparql::Variable("et"))
    };

    robin_hood::unordered_map<std::string,std::string> expectedPrefixes{{"rdfs", "http://www.w3.org/2000/01/rdf-schema#"},
                                                       {"swc",  "http://data.semanticweb.org/ns/swc/ontology#"},
                                                       {"dce",  "http://purl.org/dc/elements/1.1/"}
    };

    std::vector<char> expectedSubscriptResult{'c','d'};
    std::vector<sparql::Variable> expectedSelectVariables{ sparql::Variable{"el"},
                                                         sparql::Variable{"et"}
    };

    SelectModifier selectModifier=SelectModifier::NONE;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);

}



