#include <gtest/gtest.h>

#include <Dice/RDF/ParseTerm.hpp>

#include "TestUtilites.hpp"
#include <Dice/sparql-parser/Parser.hpp>

using namespace Dice::sparql::Nodes::QueryNodes::SelectNodes;
using namespace Dice::sparql_parser;
using namespace Dice;

TEST(BasicBgpsTests, BasicSelectQueryDefaultTest) {
    std::string query{
            "SELECT ?book ?title ?price\n"
            "WHERE\n"
            "  { ?book <dc:title> ?title ;\n"
            "         <ns:price> ?price ."
            "  }"};

    std::shared_ptr<SelectNode> selectNode=Parser::parseSelectQuery(query);

    std::vector<std::vector<char>> expectedOperands {{'a','b'},
                                                     {'a','c'}
                                                    };
    std::vector<sparql::TriplePattern> expectedBgps{sparql::TriplePattern(sparql::Variable("book"),rdf::parse_term("<dc:title>"),sparql::Variable("title")),
                                                   sparql::TriplePattern(sparql::Variable("book"),rdf::parse_term("<ns:price>"),sparql::Variable("price"))};

    std::map<std::string,std::string> expectedPrefixes{};
    std::vector<char> expectedSubscriptResult{'a','b','c'};
    std::vector<sparql::Variable> expectedSelectVariables{ sparql::Variable{"book"},
                                                         sparql::Variable{"title"},
                                                         sparql::Variable{"price"}
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

    std::shared_ptr<SelectNode> selectNode=Parser::parseSelectQuery(query);

    std::vector<std::vector<char>> expectedOperands {{'a','b'},
                                                     {'a','c'}
    };
    std::vector<sparql::TriplePattern> expectedBgps{sparql::TriplePattern(sparql::Variable("book"),rdf::parse_term("<dc:title>"),sparql::Variable("title")),
                                                   sparql::TriplePattern(sparql::Variable("book"),rdf::parse_term("<ns:price>"),sparql::Variable("price"))};

    std::map<std::string,std::string> expectedPrefixes{};
    std::vector<char> expectedSubscriptResult{'a','b','c'};
    std::vector<sparql::Variable> expectedSelectVariables{ sparql::Variable{"book"},
                                                         sparql::Variable{"title"},
                                                         sparql::Variable{"price"}
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

    std::shared_ptr<SelectNode> selectNode=Parser::parseSelectQuery(query);


    std::vector<std::vector<char>> expectedOperands {{'a','b'},
                                                     {'a','c'}
    };
    std::vector<sparql::TriplePattern> expectedBgps{sparql::TriplePattern(sparql::Variable("book"),rdf::parse_term("<dc:title>"),sparql::Variable("title")),
                                                   sparql::TriplePattern(sparql::Variable("book"),rdf::parse_term("<ns:price>"),sparql::Variable("price"))};

    std::map<std::string,std::string> expectedPrefixes{};
    std::vector<char> expectedSubscriptResult{'a','b','c'};
    std::vector<sparql::Variable> expectedSelectVariables{ sparql::Variable{"book"},
                                                         sparql::Variable{"title"},
                                                         sparql::Variable{"price"}
    };

    SelectModifier selectModifier=SelectModifier::REDUCE;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);
}

TEST(BasicBgpsTests, multipleBgps) {
    std::string query{
            "SELECT ?s ?e WHERE {?s <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://localhost/vocabulary/bench/Journal>.?s <http://swrc.ontoware.org/ontology#editor> ?e}"};


    std::shared_ptr<SelectNode> selectNode=Parser::parseSelectQuery(query);

    std::vector<std::vector<char>> expectedOperands {{'a'},
                                                     {'a','b'}
    };
    std::vector<sparql::TriplePattern> expectedBgps{sparql::TriplePattern(sparql::Variable("s"),rdf::parse_term("<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>"),rdf::parse_term("<http://localhost/vocabulary/bench/Journal>")),
                                                   sparql::TriplePattern(sparql::Variable("s"),rdf::parse_term("<http://swrc.ontoware.org/ontology#editor>"),sparql::Variable("e"))};

    std::map<std::string,std::string> expectedPrefixes{};
    std::vector<char> expectedSubscriptResult{'a','b'};
    std::vector<sparql::Variable> expectedSelectVariables{ sparql::Variable{"s"},
                                                         sparql::Variable{"e"}
                                                        };

    SelectModifier selectModifier=SelectModifier::NONE;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);
}

TEST(BasicBgpsTests, multipleBgps2) {
    std::string query{
            "SELECT DISTINCT ?s ?p ?o WHERE {?s <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://localhost/vocabulary/bench/Article> . ?s ?p ?o .}"};


    std::shared_ptr<SelectNode> selectNode=Parser::parseSelectQuery(query);

    std::vector<std::vector<char>> expectedOperands {{'a'},
                                                     {'a','b','c'}
    };
    std::vector<sparql::TriplePattern> expectedBgps{sparql::TriplePattern(sparql::Variable("s"),rdf::parse_term("<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>"),rdf::parse_term("<http://localhost/vocabulary/bench/Article>")),
                                                   sparql::TriplePattern(sparql::Variable("s"),sparql::Variable("p"),sparql::Variable("o"))};

    std::map<std::string,std::string> expectedPrefixes{};
    std::vector<char> expectedSubscriptResult{'a','b','c'};
    std::vector<sparql::Variable> expectedSelectVariables{ sparql::Variable{"s"},
                                                         sparql::Variable{"p"},
                                                         sparql::Variable{"o"}
    };

    SelectModifier selectModifier=SelectModifier::DISTINCT;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);

}



TEST(BasicBgpsTests, multipleBgpsConnectedWithSemiColon) {
    std::string query{
            "PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#> PREFIX swc: <http://data.semanticweb.org/ns/swc/ontology#> PREFIX dce: <http://purl.org/dc/elements/1.1/> \n"
            " SELECT ?el ?et WHERE { <http://data.semanticweb.org/workshop/usewod/2012> swc:hasLocation ?l . ?e swc:hasLocation ?l ; rdfs:label ?el }"};


    std::shared_ptr<SelectNode> selectNode=Parser::parseSelectQuery(query);

    std::vector<std::vector<char>> expectedOperands {{'a'},
                                                     {'b','a'},
                                                     {'b','c'}
    };
    std::vector<sparql::TriplePattern> expectedBgps{sparql::TriplePattern(rdf::parse_term("<http://data.semanticweb.org/workshop/usewod/2012>"),rdf::parse_term("<http://data.semanticweb.org/ns/swc/ontology#hasLocation>"),sparql::Variable("l")),
                                                   sparql::TriplePattern(sparql::Variable("e"),rdf::parse_term("<http://data.semanticweb.org/ns/swc/ontology#hasLocation>"),sparql::Variable("l")),
                                                   sparql::TriplePattern(sparql::Variable("e"),rdf::parse_term("<http://www.w3.org/2000/01/rdf-schema#label>"),sparql::Variable("el"))
    };

    std::map<std::string,std::string> expectedPrefixes{ {"rdfs","http://www.w3.org/2000/01/rdf-schema#"},
                                                        {"swc","http://data.semanticweb.org/ns/swc/ontology#"},
                                                        {"dce","http://purl.org/dc/elements/1.1/"}
    };
    std::vector<char> expectedSubscriptResult{'c','d'};
    std::vector<sparql::Variable> expectedSelectVariables{ sparql::Variable{"el"},
                                                         sparql::Variable{"et"}
    };

    SelectModifier selectModifier=SelectModifier::NONE;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);

}

TEST(BasicBgpsTests, GroupGraphPatternInsideGroupOrUnionGraphPattern) {
    std::string query{
            "PREFIX wde: <http://www.wikidata.org/entity/> PREFIX wdt: <http://www.wikidata.org/prop/direct/>  \n"
            " SELECT ?var1 ?var2 ?var3 WHERE { ?var1 wdt:P463 wde:Q202479 ; wdt:P31 wde:Q5 "
            " { ?var1 <http://www.wikidata.org/prop/P463> _:b0 . _:b0 <http://www.wikidata.org/prop/statement/P463> wde:Q202479 ; <http://www.wikidata.org/prop/qualifier/P580> ?var2 } "
            " { ?var1 <http://www.wikidata.org/prop/P463> _:b1 . _:b1 <http://www.wikidata.org/prop/statement/P463> wde:Q202479 ; <http://www.wikidata.org/prop/qualifier/P582> ?var3 } }"};


    std::shared_ptr<SelectNode> selectNode=Parser::parseSelectQuery(query);

    std::vector<sparql::TriplePattern> expectedBgps{sparql::TriplePattern(sparql::Variable("var1") ,
                                                                        rdf::parse_term("<http://www.wikidata.org/prop/direct/P463>"),
                                                                        rdf::parse_term("<http://www.wikidata.org/entity/Q202479>") ),

                                                   sparql::TriplePattern(sparql::Variable("var1") ,
                                                                        rdf::parse_term("<http://www.wikidata.org/prop/direct/P31>"),
                                                                        rdf::parse_term("<http://www.wikidata.org/entity/Q5>") ),

                                                   sparql::TriplePattern(sparql::Variable("var1") ,
                                                                        rdf::parse_term("<http://www.wikidata.org/prop/P463>"),
                                                                        sparql::Variable("b0") ),

                                                   sparql::TriplePattern(sparql::Variable("b0") ,
                                                                        rdf::parse_term("<http://www.wikidata.org/prop/statement/P463>"),
                                                                        rdf::parse_term("<http://www.wikidata.org/entity/Q202479>")),

                                                   sparql::TriplePattern(sparql::Variable("b0") ,
                                                                        rdf::parse_term("<http://www.wikidata.org/prop/qualifier/P580>"),
                                                                        sparql::Variable("var2") ),

                                                   sparql::TriplePattern(sparql::Variable("var1") ,
                                                                        rdf::parse_term("<http://www.wikidata.org/prop/P463>"),
                                                                        sparql::Variable("b1") ),

                                                   sparql::TriplePattern(sparql::Variable("b1") ,
                                                                        rdf::parse_term("<http://www.wikidata.org/prop/statement/P463>"),
                                                                        rdf::parse_term("<http://www.wikidata.org/entity/Q202479>") ),

                                                   sparql::TriplePattern(sparql::Variable("b1") ,
                                                                        rdf::parse_term("<http://www.wikidata.org/prop/qualifier/P582>"),
                                                                        sparql::Variable("var3") )
    };

    std::map<std::string,std::string> expectedPrefixes{{"wde", "http://www.wikidata.org/entity/"},
                                                       {"wdt", "http://www.wikidata.org/prop/direct/"}
    };

    std::vector<char> expectedSubscriptResult{'a','c','e'};

    std::vector<std::vector<char>> expectedOperands {{'a'},
                                                     {'a'},
                                                     {'a','b'},
                                                     {'b'},
                                                     {'b','c'},
                                                     {'a','d'},
                                                     {'d'},
                                                     {'d','e'},
    };

    std::vector<sparql::Variable> expectedSelectVariables{ sparql::Variable{"var1"},
                                                         sparql::Variable{"var2"},
                                                         sparql::Variable{"var3"}
    };

    SelectModifier selectModifier=SelectModifier::NONE;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);
}


TEST(BasicBgpsTests, GroupPatterns) {
    std::string query{
            "PREFIX wde: <http://www.wikidata.org/entity/> PREFIX wdt: <http://www.wikidata.org/prop/direct/>  \n"
            " SELECT ?var1 ?var2 ?var3 WHERE { ?var1 wdt:P463 wde:Q202479 ; wdt:P31 wde:Q5 "
            " { ?var1 <http://www.wikidata.org/prop/P463> _:b0 . _:b0 <http://www.wikidata.org/prop/statement/P463> wde:Q202479 ; <http://www.wikidata.org/prop/qualifier/P580> ?var2 } "
            " { ?var1 <http://www.wikidata.org/prop/P463> _:b1 . _:b1 <http://www.wikidata.org/prop/statement/P463> wde:Q202479 ; <http://www.wikidata.org/prop/qualifier/P582> ?var3 } }"};


    std::shared_ptr<SelectNode> selectNode=Parser::parseSelectQuery(query);

    std::vector<sparql::TriplePattern> expectedBgps{sparql::TriplePattern(sparql::Variable("var1") ,
                                                                        rdf::parse_term("<http://www.wikidata.org/prop/direct/P463>"),
                                                                        rdf::parse_term("<http://www.wikidata.org/entity/Q202479>") ),

                                                   sparql::TriplePattern(sparql::Variable("var1") ,
                                                                        rdf::parse_term("<http://www.wikidata.org/prop/direct/P31>"),
                                                                        rdf::parse_term("<http://www.wikidata.org/entity/Q5>") ),

                                                   sparql::TriplePattern(sparql::Variable("var1") ,
                                                                        rdf::parse_term("<http://www.wikidata.org/prop/P463>"),
                                                                        sparql::Variable("b0") ),

                                                   sparql::TriplePattern(sparql::Variable("b0") ,
                                                                        rdf::parse_term("<http://www.wikidata.org/prop/statement/P463>"),
                                                                        rdf::parse_term("<http://www.wikidata.org/entity/Q202479>")),

                                                   sparql::TriplePattern(sparql::Variable("b0") ,
                                                                        rdf::parse_term("<http://www.wikidata.org/prop/qualifier/P580>"),
                                                                        sparql::Variable("var2") ),

                                                   sparql::TriplePattern(sparql::Variable("var1") ,
                                                                        rdf::parse_term("<http://www.wikidata.org/prop/P463>"),
                                                                        sparql::Variable("b1") ),

                                                   sparql::TriplePattern(sparql::Variable("b1") ,
                                                                        rdf::parse_term("<http://www.wikidata.org/prop/statement/P463>"),
                                                                        rdf::parse_term("<http://www.wikidata.org/entity/Q202479>") ),

                                                   sparql::TriplePattern(sparql::Variable("b1") ,
                                                                        rdf::parse_term("<http://www.wikidata.org/prop/qualifier/P582>"),
                                                                        sparql::Variable("var3") )
    };

    std::map<std::string,std::string> expectedPrefixes{{"wde", "http://www.wikidata.org/entity/"},
                                                       {"wdt", "http://www.wikidata.org/prop/direct/"}
    };

    std::vector<char> expectedSubscriptResult{'a','c','e'};

    std::vector<std::vector<char>> expectedOperands {{'a'},
                                                     {'a'},
                                                     {'a','b'},
                                                     {'b'},
                                                     {'b','c'},
                                                     {'a','d'},
                                                     {'d'},
                                                     {'d','e'},
    };

    std::vector<sparql::Variable> expectedSelectVariables{ sparql::Variable{"var1"},
                                                         sparql::Variable{"var2"},
                                                         sparql::Variable{"var3"}
    };

    SelectModifier selectModifier=SelectModifier::NONE;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);
}