#include <gtest/gtest.h>

#include <Dice/sparql-parser/Parser.hpp>
#include <Dice/RDF/ParseTerm.hpp>

#include "TestUtilites.hpp"

using namespace Dice::sparql::Nodes::QueryNodes::SelectNodes;
using namespace Dice::sparql_parser;
using namespace Dice;

TEST(ComplexOptionalPatternTests, optionalWithBlankNodes) {
    std::string query{
            "PREFIX wde: <http://www.wikidata.org/entity/> PREFIX wdt: <http://www.wikidata.org/prop/direct/>  \n"
            " SELECT ?var1 ?var2 ?var3 WHERE { ?var1 wdt:P463 wde:Q202479 ; wdt:P31 wde:Q5 "
            "OPTIONAL { ?var1 <http://www.wikidata.org/prop/P463> _:b0 . _:b0 <http://www.wikidata.org/prop/statement/P463> wde:Q202479 ; <http://www.wikidata.org/prop/qualifier/P580> ?var2 } "
            "OPTIONAL { ?var1 <http://www.wikidata.org/prop/P463> _:b1 . _:b1 <http://www.wikidata.org/prop/statement/P463> wde:Q202479 ; <http://www.wikidata.org/prop/qualifier/P582> ?var3 } }"};


    std::shared_ptr<SelectNode> selectNode=Dice::sparql_parser::Parser::parseSelectQuery(query);



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
                                                     {'['},
                                                     {'a','b'},
                                                     {'b'},
                                                     {'b','c'},
                                                     {']'},
                                                     {'['},
                                                     {'a','d'},
                                                     {'d'},
                                                     {'d','e'},
                                                     {']'}
    };

    std::vector<sparql::Variable> expectedSelectVariables{ sparql::Variable{"var1"},
                                                         sparql::Variable{"var2"},
                                                         sparql::Variable{"var3"}
                                                         };

    SelectModifier selectModifier=SelectModifier::NONE;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);
}