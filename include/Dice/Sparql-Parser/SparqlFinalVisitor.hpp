//
// Created by fakhr on 18.02.20.
//

#ifndef SPARQL_PARSER_SPARQLFINALVISITOR_HPP
#define SPARQL_PARSER_SPARQLFINALVISITOR_HPP

#include <Sparql/SparqlBaseVisitor.h>

class SparqlFinalVisitor : public Dice::tentris::sparql::parser::SparqlBaseVisitor
{
public:

    antlrcpp::Any visitSelectQuery(Dice::tentris::sparql::parser::SparqlParser::SelectQueryContext *ctx) override {
        return SparqlBaseVisitor::visitSelectQuery(ctx);
    }

    antlrcpp::Any
    visitSelectModifier(Dice::tentris::sparql::parser::SparqlParser::SelectModifierContext *ctx) override {
        return SparqlBaseVisitor::visitSelectModifier(ctx);
    }



    antlrcpp::Any visitWhereClause(Dice::tentris::sparql::parser::SparqlParser::WhereClauseContext *ctx) override {
        return SparqlBaseVisitor::visitWhereClause(ctx);
    }

    antlrcpp::Any
    visitSolutionModifier(Dice::tentris::sparql::parser::SparqlParser::SolutionModifierContext *ctx) override {
        return SparqlBaseVisitor::visitSolutionModifier(ctx);
    }

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

    antlrcpp::Any
    visitGroupGraphPattern(Dice::tentris::sparql::parser::SparqlParser::GroupGraphPatternContext *ctx) override {
        return SparqlBaseVisitor::visitGroupGraphPattern(ctx);
    }

    antlrcpp::Any visitTriplesBlock(Dice::tentris::sparql::parser::SparqlParser::TriplesBlockContext *ctx) override {
        return SparqlBaseVisitor::visitTriplesBlock(ctx);
    }

    antlrcpp::Any visitGraphPatternNotTriples(
            Dice::tentris::sparql::parser::SparqlParser::GraphPatternNotTriplesContext *ctx) override {
        return SparqlBaseVisitor::visitGraphPatternNotTriples(ctx);
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


#endif //SPARQL_PARSER_SPARQLFINALVISITOR_HPP
