#include <gtest/gtest.h>
#include <Dice/Sparql-Parser/Parser.hpp>

#include "TestUtilites.hpp"

using namespace SparqlQueryGraph::Nodes::SelectNodes;

TEST(ComplexOptionalPatternTests, optionalWithBlankNodes) {
    std::string query{
            "PREFIX wde: <http://www.wikidata.org/entity/> PREFIX wdt: <http://www.wikidata.org/prop/direct/>  \n"
            " SELECT ?var1 ?var2 ?var3 WHERE { ?var1 wdt:P463 wde:Q202479 ; wdt:P31 wde:Q5 "
            "OPTIONAL { ?var1 <http://www.wikidata.org/prop/P463> _:b0 . _:b0 <http://www.wikidata.org/prop/statement/P463> wde:Q202479 ; <http://www.wikidata.org/prop/qualifier/P580> ?var2 } "
            "OPTIONAL { ?var1 <http://www.wikidata.org/prop/P463> _:b1 . _:b1 <http://www.wikidata.org/prop/statement/P463> wde:Q202479 ; <http://www.wikidata.org/prop/qualifier/P582> ?var3 } }"};


    std::shared_ptr<SelectNode> selectNode=SparqlParser::Parser::parseSelectQuery(query);



    std::vector<TriplePatternElement> expectedBgps{TriplePatternElement(TripleVariable("var1") ,
                                                                        Term("<http://www.wikidata.org/prop/direct/P463>"),
                                                                        Term("<http://www.wikidata.org/entity/Q202479>") ),

                                                   TriplePatternElement(TripleVariable("var1") ,
                                                                        Term("<http://www.wikidata.org/prop/direct/P31>"),
                                                                        Term("<http://www.wikidata.org/entity/Q5>") ),

                                                   TriplePatternElement(TripleVariable("var1") ,
                                                                        Term("<http://www.wikidata.org/prop/P463>"),
                                                                        TripleVariable("b0") ),

                                                   TriplePatternElement(TripleVariable("b0") ,
                                                                        Term("<http://www.wikidata.org/prop/statement/P463>"),
                                                                        Term("<http://www.wikidata.org/entity/Q202479>")),

                                                   TriplePatternElement(TripleVariable("b0") ,
                                                                        Term("<http://www.wikidata.org/prop/qualifier/P580>"),
                                                                        TripleVariable("var2") ),

                                                   TriplePatternElement(TripleVariable("var1") ,
                                                                        Term("<http://www.wikidata.org/prop/P463>"),
                                                                        TripleVariable("b1") ),

                                                   TriplePatternElement(TripleVariable("b1") ,
                                                                        Term("<http://www.wikidata.org/prop/statement/P463>"),
                                                                        Term("<http://www.wikidata.org/entity/Q202479>") ),

                                                   TriplePatternElement(TripleVariable("b1") ,
                                                                        Term("<http://www.wikidata.org/prop/qualifier/P582>"),
                                                                        TripleVariable("var3") )
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

    std::vector<TripleVariable> expectedSelectVariables{ TripleVariable{"var1"},
                                                         TripleVariable{"var2"},
                                                         TripleVariable{"var3"}
                                                         };

    SelectModifier selectModifier=SelectModifier::NONE;

    TestUtilites::checkResult(selectNode,expectedOperands,expectedBgps,expectedPrefixes,expectedSelectVariables,expectedSubscriptResult,selectModifier);
}