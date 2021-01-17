//
// Created by fakhr on 22.12.20.
//

#ifndef SPARQL_PARSER_TESTUTILITES_HPP
#define SPARQL_PARSER_TESTUTILITES_HPP

#include <gtest/gtest.h>
#include <Dice/sparql-parser/Parser.hpp>

using namespace Dice::sparql::Nodes::QueryNodes::SelectNodes;

class TestUtilites
{
public:
    void static checkResult(std::shared_ptr<SelectNode> result,
                     std::vector<std::vector<char>> expectedOperands,
                     std::vector<Dice::sparql::TriplePattern> expectedBgps,
                     std::map<std::string,std::string> expectedPrefixes,
                     std::vector<Dice::sparql::Variable> expectedSelectVariables,
                     std::vector<char> expectedSubscriptResult,
                     SelectModifier expectedSelectModifier
    )
    {


        auto operands=result->getOperands();
        auto bgps=result->getBgps();
        auto  prefixes=result->getPrefixes();
        auto subscriptResult=result->getSubscriptResult();
        auto selectVariables=result->getSelectVariables();
        auto selectModifier=result->getSelectModifier();

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
        ASSERT_EQ(selectModifier,expectedSelectModifier);
    }
};


#endif //SPARQL_PARSER_TESTUTILITES_HPP
