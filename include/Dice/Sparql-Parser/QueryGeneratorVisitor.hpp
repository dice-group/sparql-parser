//
// Created by fakhr on 11.03.20.
//

#ifndef SPARQL_PARSER_QUERYGENERATORVISITOR_HPP
#define SPARQL_PARSER_QUERYGENERATORVISITOR_HPP

#include <memory>
#include <vector>

#include <SparqlParser/SparqlParserBaseVisitor.h>
#include <Dice/Sparql-Query/SelectQuery.hpp>
#include <Dice/Sparql-Query/QueryNodes/SelectNodes/AbstractSelectNode.hpp>
#include <Dice/Sparql-Query/QueryNodes/SelectNodes/DefaultSelectNode.hpp>
#include <Dice/Sparql-Query/QueryNodes/SelectNodes/DistinctSelectNode.hpp>
#include <Dice/Sparql-Query/QueryNodes/SelectNodes/ReducedSelectNode.hpp>
#include <Dice/Sparql-Query/QueryNodes/SolutionDecorator.hpp>

#include "SelectNodeType.hpp"
#include "SelectClause.hpp"
#include "Exceptions.hpp"

class QueryGeneratorVisitor : Dice::tentris::sparql::parser::SparqlParserBaseVisitor{
public:



    antlrcpp::Any visitQuery(Dice::tentris::sparql::parser::SparqlParser::QueryContext *ctx) override {
        return visitSelectQuery(ctx->selectQuery());
    }


    antlrcpp::Any visitSelectQuery(Dice::tentris::sparql::parser::SparqlParser::SelectQueryContext *ctx) override {

        std::shared_ptr<SelectQuery> selectQuery;
        std::shared_ptr<AbstractSelectNode> selectNode;
        std::shared_ptr<IQueryNode> queryNode;

        //visit where clause
        std::shared_ptr<ICommandNode> commandNode = visitWhereClause(ctx->whereClause());

        //get all solution modifiers. visitSolutionModifier() must handle the order.
        std::vector<std::shared_ptr<ISolutionModifier>> solutionModifiers = visitSolutionModifier(
                ctx->solutionModifier());

        //deal with the solution modifiers
        if (solutionModifiers.size() != 0) {

            //get the first modifier
            std::shared_ptr<ISolutionModifier> firstModifier = solutionModifiers.front();
            solutionModifiers.pop_back();
            std::shared_ptr<SolutionDecorator> solutionDecorator = std::make_shared<SolutionDecorator>(commandNode,
                                                                                                       firstModifier);

            //go through the rest of the modifiers and add them to the decorator
            for (auto const &solutionModifier : solutionModifiers)
                solutionDecorator = std::make_shared<SolutionDecorator>(solutionDecorator, solutionModifier);

            queryNode = solutionDecorator;
        } else
            queryNode = commandNode;

        SelectClause clause=visitSelectClause(ctx->selectClause());

        //create the selectNode
        if (clause.nodeType == SelectNodeType::DISTINCT)
            selectNode = std::make_shared<DistinctSelectNode>(queryNode);
        else if (clause.nodeType == SelectNodeType::REDUCED)
            selectNode = std::make_shared<ReducedSelectNode>(queryNode);
        else
            selectNode = std::make_shared<DefaultSelectNode>(queryNode);

    }


    antlrcpp::Any visitSelectClause(Dice::tentris::sparql::parser::SparqlParser::SelectClauseContext *ctx) override {

        SelectClause selectClause;

        //deal with node type
        SelectNodeType nodeType;

        if (ctx->selectModifier() != nullptr)
            nodeType = visitSelectModifier(ctx-> selectModifier());
        else
            nodeType = SelectNodeType::DEFAULT;

        selectClause.nodeType=nodeType;

        //ToDo deal with the variables


        return selectClause;
    }

    antlrcpp::Any
    visitSelectModifier(Dice::tentris::sparql::parser::SparqlParser::SelectModifierContext *ctx) override {

        std::string nodeTypeRaw = ctx->toString();
        SelectNodeType nodeType;
        if (nodeTypeRaw == "DISTINCT")
            nodeType = SelectNodeType::DISTINCT;
        else if (nodeTypeRaw == "REDUCE")
            nodeType = SelectNodeType::REDUCED;
        else
            throw new ParseException();
        return nodeType;
    }


    antlrcpp::Any
    visitSolutionModifier(Dice::tentris::sparql::parser::SparqlParser::SolutionModifierContext *ctx) override {
        std::vector<std::shared_ptr<ISolutionModifier>> solutionModifiers;

        if(ctx->groupClause()!= nullptr)
        {
            //ToDo
        }

        if(ctx->havingClause()!=nullptr)
        {
            //ToDo
        }

        if(ctx->orderClause()!= nullptr)
        {
            //ToDo
        }

        if(ctx->limitOffsetClauses()!= nullptr)
        {
            //ToDo
        }

        return solutionModifiers;
    }


    antlrcpp::Any visitWhereClause(Dice::tentris::sparql::parser::SparqlParser::WhereClauseContext *ctx) override {
        return visitGroupGraphPattern(ctx->groupGraphPattern());

    }



    antlrcpp::Any
    visitGroupGraphPattern(Dice::tentris::sparql::parser::SparqlParser::GroupGraphPatternContext *ctx) override {
        std::shared_ptr<ICommandNode> commandNode;

        //ToDo
        if (ctx->subSelect()!= nullptr)
            commandNode=visitSubSelect(ctx->subSelect());
        else
            commandNode=visitGroupGraphPatternSub(ctx->groupGraphPatternSub());

        return commandNode;
    }

    antlrcpp::Any
    visitGroupGraphPatternSub(Dice::tentris::sparql::parser::SparqlParser::GroupGraphPatternSubContext *ctx) override {

        std::shared_ptr<ICommandNode> commandNode;

        //ToDo
        if(ctx->triplesBlock()!= nullptr)
        {

        }

        for(auto& subList:ctx->groupGraphPatternSubList())
        {
            //ToDo
        }

        return commandNode;

    }


    antlrcpp::Any visitGroupGraphPatternSubList(
            Dice::tentris::sparql::parser::SparqlParser::GroupGraphPatternSubListContext *ctx) override {

        std::shared_ptr<ICommandNode> commandNode;

        //Deal with the graphPatternNotTriples
        //ToDo

        //Deal with the triplesBlock
        //ToDo
        if(ctx->triplesBlock()!= nullptr)
        {

        }

        return commandNode;
    }


    antlrcpp::Any visitGraphPatternNotTriples(
            Dice::tentris::sparql::parser::SparqlParser::GraphPatternNotTriplesContext *ctx) override {

        std::shared_ptr<ICommandNode> commandNode;

        //ToDo
        if(ctx->optionalGraphPattern()!= nullptr)
        {

        }
        //ToDo deal with the remaining types


        return commandNode;
    }


public:

    antlrcpp::Any
    visitLimitOffsetClauses(Dice::tentris::sparql::parser::SparqlParser::LimitOffsetClausesContext *ctx) override {
        return SparqlBaseVisitor::visitLimitOffsetClauses(ctx);
    }

    antlrcpp::Any visitOrderClause(Dice::tentris::sparql::parser::SparqlParser::OrderClauseContext *ctx) override {
        return SparqlBaseVisitor::visitOrderClause(ctx);
    }

    antlrcpp::Any
    visitOrderCondition(Dice::tentris::sparql::parser::SparqlParser::OrderConditionContext *ctx) override {
        return SparqlBaseVisitor::visitOrderCondition(ctx);
    }

    antlrcpp::Any visitLimitClause(Dice::tentris::sparql::parser::SparqlParser::LimitClauseContext *ctx) override {
        return SparqlBaseVisitor::visitLimitClause(ctx);
    }

    antlrcpp::Any visitOffsetClause(Dice::tentris::sparql::parser::SparqlParser::OffsetClauseContext *ctx) override {
        return SparqlBaseVisitor::visitOffsetClause(ctx);
    }



    antlrcpp::Any visitTriplesBlock(Dice::tentris::sparql::parser::SparqlParser::TriplesBlockContext *ctx) override {
        return SparqlBaseVisitor::visitTriplesBlock(ctx);
    }



    antlrcpp::Any
    visitOptionalGraphPattern(Dice::tentris::sparql::parser::SparqlParser::OptionalGraphPatternContext *ctx) override {
        return SparqlBaseVisitor::visitOptionalGraphPattern(ctx);
    }

    antlrcpp::Any
    visitGraphGraphPattern(Dice::tentris::sparql::parser::SparqlParser::GraphGraphPatternContext *ctx) override {
        return SparqlBaseVisitor::visitGraphGraphPattern(ctx);
    }

    antlrcpp::Any visitGroupOrUnionGraphPattern(
            Dice::tentris::sparql::parser::SparqlParser::GroupOrUnionGraphPatternContext *ctx) override {
        return SparqlBaseVisitor::visitGroupOrUnionGraphPattern(ctx);
    }

    antlrcpp::Any visitFilter(Dice::tentris::sparql::parser::SparqlParser::FilterContext *ctx) override {
        return SparqlBaseVisitor::visitFilter(ctx);
    }

};


#endif //SPARQL_PARSER_QUERYGENERATORVISITOR_HPP
