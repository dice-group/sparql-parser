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
#include <Dice/Sparql-Query/TriplePatternElement.hpp>

#include <Dice/rdf_parser/RDF/Term.hpp>



#include "SelectNodeType.hpp"
#include "SelectClause.hpp"
#include "PropertySetElement.hpp"
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



//    antlrcpp::Any visitTriplesBlock(Dice::tentris::sparql::parser::SparqlParser::TriplesBlockContext *ctx) override {
//        return SparqlBaseVisitor::visitTriplesBlock(ctx);
//    }


    antlrcpp::Any visitTriplesSameSubjectPath(
            Dice::tentris::sparql::parser::SparqlParser::TriplesSameSubjectPathContext *ctx) override {
        return SparqlParserBaseVisitor::visitTriplesSameSubjectPath(ctx);
    }


    antlrcpp::Any visitPropertyListPathNotEmpty(
            Dice::tentris::sparql::parser::SparqlParser::PropertyListPathNotEmptyContext *ctx) override {
        return SparqlParserBaseVisitor::visitPropertyListPathNotEmpty(ctx);
    }

    antlrcpp::Any visitVerbPath(Dice::tentris::sparql::parser::SparqlParser::VerbPathContext *ctx) override {
        return visitPath(ctx->path());
    }

    antlrcpp::Any visitPath(Dice::tentris::sparql::parser::SparqlParser::PathContext *ctx) override {
        return visitPathAlternative(ctx->pathAlternative());
    }

    antlrcpp::Any
    visitPathAlternative(Dice::tentris::sparql::parser::SparqlParser::PathAlternativeContext *ctx) override {
        std::vector<> pathSequences;
        for(auto sequence:ctx->pathSequence())
            pathSequences.push_back(visitPathSequence(sequence));
        return pathSequences;
    }

    antlrcpp::Any visitPathSequence(Dice::tentris::sparql::parser::SparqlParser::PathSequenceContext *ctx) override {
        std::vector<> elements;
        for(auto element:ctx->pathEltOrInverse())
            elements.push_back(visitPathEltOrInverse(element));
        return elements;
    }

    antlrcpp::Any
    visitPathEltOrInverse(Dice::tentris::sparql::parser::SparqlParser::PathEltOrInverseContext *ctx) override {
        return SparqlParserBaseVisitor::visitPathEltOrInverse(ctx);
    }

    antlrcpp::Any visitPathElt(Dice::tentris::sparql::parser::SparqlParser::PathEltContext *ctx) override {
        return SparqlParserBaseVisitor::visitPathElt(ctx);
    }

    antlrcpp::Any visitPathPrimary(Dice::tentris::sparql::parser::SparqlParser::PathPrimaryContext *ctx) override {
        return SparqlParserBaseVisitor::visitPathPrimary(ctx);
    }




    //done
    antlrcpp::Any visitPathNegatedPropertySet(
            Dice::tentris::sparql::parser::SparqlParser::PathNegatedPropertySetContext *ctx) override {
        std::vector<PropertySetElement> elements;
        for(auto element:ctx->pathOneInPropertySet())
            elements.push_back(visitPathOneInPropertySet(element));
        return elements;
    }

    //done
    antlrcpp::Any
    visitPathOneInPropertySet(Dice::tentris::sparql::parser::SparqlParser::PathOneInPropertySetContext *ctx) override {
        bool isInvsersed;
        if(ctx->INVERSE()!= nullptr)
            isInvsersed=true;
        else
            isInvsersed=false;
        //if it has an iri
        if(ctx->iri()!=nullptr)
        {
            rdf_parser::store::rdf::URIRef iri=visitIri(ctx->iri());
            return IriPropertySetElement(iri,isInvsersed);
        }
        //then it has an A
        else
            return  APropertySetElement(isInvsersed);
    }

    antlrcpp::Any visitIri(Dice::tentris::sparql::parser::SparqlParser::IriContext *ctx) override {
        //if the iri is IRIREF
        if(ctx->IRIREF()!= nullptr)
        {
            Term term= Term::make_term(ctx->IRIREF()->getText());
            return term;
        }
        //else the iri is prefixedName
        else
        {

        }
    }


    //done
    antlrcpp::Any visitPathMod(Dice::tentris::sparql::parser::SparqlParser::PathModContext *ctx) override {
        return ctx->getText();
    }



    //partialy done ..check the return type
    antlrcpp::Any visitVarOrTerm(Dice::tentris::sparql::parser::SparqlParser::VarOrTermContext *ctx) override {



        //visit term
        if(ctx->graphTerm()!= nullptr)
            return visitGraphTerm(ctx->graphTerm());
        //visit var
        else
            return visitVar(ctx->var());
    }


    //Done
    antlrcpp::Any visitVar(Dice::tentris::sparql::parser::SparqlParser::VarContext *ctx) override {
        if(ctx->VAR1() != nullptr)
        {
            return TripleVariable(ctx->VAR1()->getText().substr(1,ctx->VAR1()->getText().length()-1));
        } else
        {
            return TripleVariable(ctx->VAR2()->getText().substr(1,ctx->VAR2()->getText().length()-1));
        }

    }

    //Done
    antlrcpp::Any visitGraphTerm(Dice::tentris::sparql::parser::SparqlParser::GraphTermContext *ctx) override {
        std::string termRaw=ctx->getText();
        rdf_parser::store::rdf::Term term=rdf_parser::Turtle::parseTerm(termRaw);
        return term;
    }




};


#endif //SPARQL_PARSER_QUERYGENERATORVISITOR_HPP
