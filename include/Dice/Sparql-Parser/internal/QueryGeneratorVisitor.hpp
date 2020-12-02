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
#include <Dice/Sparql-Query/QueryNodes/leafNodes/TriplePatternNode.hpp>
#include <Dice/Sparql-Query/QueryNodes/SolutionDecorator.hpp>
#include <Dice/Sparql-Query/TriplePatternElement.hpp>
#include <Dice/Sparql-Query/QueryNodes/GroupNodes/OptionalPatternNode.hpp>
#include <Dice/Sparql-Query/QueryNodes/GroupNodes/GroupPatternNode.hpp>

#include <Dice/rdf_parser/RDF/Term.hpp>

#include <Dice/rdf_parser/Parser/Turtle/Parsers/StringParser.hpp>
#include <Dice/rdf_parser/Sparql/TriplePatternElement.hpp>


#include "SelectNodeType.hpp"
#include "SelectClause.hpp"
#include "PropertySetElement.hpp"
#include "Exceptions.hpp"

namespace SparqlParser::internal {
    class QueryGeneratorVisitor : Dice::tentris::SparqlParserBase::SparqlParserBaseVisitor {
    public:


        antlrcpp::Any visitQuery(Dice::tentris::SparqlParserBase::SparqlParser::QueryContext *ctx) override {

            //For now the parser only supports Select queries.
            if (ctx->selectQuery() != NULL)
                return visitSelectQuery(ctx->selectQuery());
            else
                return NotImplementedException();

        }


        antlrcpp::Any visitSelectQuery(Dice::tentris::SparqlParserBase::SparqlParser::SelectQueryContext *ctx) override {

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

            //deal with the SelectClause to define the SelectNode Type(Distinct,Reduced,Default) and to get the select variables
            SelectClause clause = visitSelectClause(ctx->selectClause());

            //create the selectNode
            if (clause.nodeType == SelectNodeType::DISTINCT)
                selectNode = std::make_shared<DistinctSelectNode>(queryNode, clause.selectVariables);
            else if (clause.nodeType == SelectNodeType::REDUCED)
                selectNode = std::make_shared<ReducedSelectNode>(queryNode, clause.selectVariables);
            else
                selectNode = std::make_shared<DefaultSelectNode>(queryNode, clause.selectVariables);


            //create the select query
            selectQuery = std::make_shared<SelectQuery>(selectNode);

            return selectQuery;

        }


        antlrcpp::Any
        visitSelectClause(Dice::tentris::SparqlParserBase::SparqlParser::SelectClauseContext *ctx) override {

            SelectClause selectClause;

            //deal with node type
            SelectNodeType nodeType;

            if (ctx->selectModifier() != nullptr)
                nodeType = visitSelectModifier(ctx->selectModifier());
            else
                nodeType = SelectNodeType::DEFAULT;

            selectClause.nodeType = nodeType;

            //deal with the variables
            std::vector<TripleVariable> selectVariables;
            for (auto selectVariable:ctx->selectVariables()) {
                selectVariables.push_back(TripleVariable(selectVariable->getText()));
            }
            selectClause.selectVariables = selectVariables;

            return selectClause;
        }

        antlrcpp::Any
        visitSelectModifier(Dice::tentris::SparqlParserBase::SparqlParser::SelectModifierContext *ctx) override {

            std::string nodeTypeRaw = ctx->getText();
            SelectNodeType nodeType;
            if (nodeTypeRaw == "DISTINCT")
                nodeType = SelectNodeType::DISTINCT;
            else if (nodeTypeRaw == "REDUCED")
                nodeType = SelectNodeType::REDUCED;
            else
                throw new ParseException();
            return nodeType;
        }


        antlrcpp::Any
        visitSolutionModifier(Dice::tentris::SparqlParserBase::SparqlParser::SolutionModifierContext *ctx) override {
            std::vector<std::shared_ptr<ISolutionModifier>> solutionModifiers;

            if (ctx->groupClause() != nullptr) {
                //ToDo
            }

            if (ctx->havingClause() != nullptr) {
                //ToDo
            }

            if (ctx->orderClause() != nullptr) {
                //ToDo
            }

            if (ctx->limitOffsetClauses() != nullptr) {
                //ToDo
            }

            return solutionModifiers;
        }


        antlrcpp::Any visitWhereClause(Dice::tentris::SparqlParserBase::SparqlParser::WhereClauseContext *ctx) override {
            return visitGroupGraphPattern(ctx->groupGraphPattern());

        }


        antlrcpp::Any
        visitGroupGraphPattern(Dice::tentris::SparqlParserBase::SparqlParser::GroupGraphPatternContext *ctx) override {
            std::shared_ptr<ICommandNode> commandNode;

            if (ctx->subSelect() != nullptr)
                commandNode = visitSubSelect(ctx->subSelect());
            else
                commandNode = visitGroupGraphPatternSub(ctx->groupGraphPatternSub());

            return commandNode;
        }

        antlrcpp::Any
        visitGroupGraphPatternSub(
                Dice::tentris::SparqlParserBase::SparqlParser::GroupGraphPatternSubContext *ctx) override {

            std::shared_ptr<ICommandNode> commandNode;
            std::shared_ptr<GroupPatternNode> groupNode = std::make_shared<GroupPatternNode>();

            //ToDo
            if (ctx->triplesBlock() != nullptr) {
                groupNode->addChild(visitTriplesBlock(ctx->triplesBlock()));
            }

            for (auto &subList:ctx->groupGraphPatternSubList()) {
                //ToDo
                groupNode->addChild(visitGroupGraphPatternSubList(subList));
            }
            commandNode = std::dynamic_pointer_cast<ICommandNode>(groupNode);
            return commandNode;

        }


    public:
        antlrcpp::Any visitGroupGraphPatternSubList(
                Dice::tentris::SparqlParserBase::SparqlParser::GroupGraphPatternSubListContext *ctx) override {

            std::shared_ptr<ICommandNode> commandNode;

            //Deal with the triplesBlock
            if (ctx->triplesBlock() != nullptr) {
                //ToDo
                std::shared_ptr<GroupPatternNode> groupNode = std::make_shared<GroupPatternNode>();
                groupNode->addChild(visitGraphPatternNotTriples(ctx->graphPatternNotTriples()));
                groupNode->addChild(visitTriplesBlock(ctx->triplesBlock()));
                commandNode = std::dynamic_pointer_cast<ICommandNode>(groupNode);
            } else
                commandNode=visitGraphPatternNotTriples(ctx->graphPatternNotTriples());

            return commandNode;
        }

        antlrcpp::Any
        visitTriplesBlock(Dice::tentris::SparqlParserBase::SparqlParser::TriplesBlockContext *ctx) override {
            std::shared_ptr<ICommandNode> commandNode;
            std::vector<TriplePatternElement> elements;
            std::string tb = ctx->getText();
            rdf_parser::Turtle::parsers::StringParser<true> parser(ctx->getText());
            auto it = parser.begin();
            while (it) {
                //ToDo check this
                TriplePatternElement triple = (TriplePatternElement &&) *it;
                elements.push_back(triple);
                it++;
            }
            commandNode = std::make_shared<TriplePatternNode>(elements);
            return commandNode;
        }


        antlrcpp::Any visitGraphPatternNotTriples(
                Dice::tentris::SparqlParserBase::SparqlParser::GraphPatternNotTriplesContext *ctx) override {

            std::shared_ptr<ICommandNode> commandNode;

            if (ctx->groupOrUnionGraphPattern() != nullptr) {

            } else if (ctx->optionalGraphPattern() != nullptr) {
                commandNode = visitOptionalGraphPattern(ctx->optionalGraphPattern());
            } else if (ctx->minusGraphPattern() != nullptr) {

            } else if (ctx->graphGraphPattern() != nullptr) {

            } else if (ctx->serviceGraphPattern() != nullptr) {

            } else if (ctx->filter() != nullptr) {

            } else if (ctx->bind() != nullptr) {

            }

                // else inlineData
            else {

            }


            return commandNode;
        }

        antlrcpp::Any visitOptionalGraphPattern(
                Dice::tentris::SparqlParserBase::SparqlParser::OptionalGraphPatternContext *ctx) override
        {
            std::shared_ptr<GroupNode> optionalNode=std::make_shared<OptionalPatternNode>();
            optionalNode->addChild(visitGroupGraphPattern(ctx->groupGraphPattern()));
            std::shared_ptr<ICommandNode> commandNode=std::dynamic_pointer_cast<ICommandNode>(optionalNode);
            return commandNode;


        }

        antlrcpp::Any visitTriplesSameSubjectPath(
                Dice::tentris::SparqlParserBase::SparqlParser::TriplesSameSubjectPathContext *ctx) override {
            return SparqlParserBaseVisitor::visitTriplesSameSubjectPath(ctx);
        }


        antlrcpp::Any visitPropertyListPathNotEmpty(
                Dice::tentris::SparqlParserBase::SparqlParser::PropertyListPathNotEmptyContext *ctx) override {
            return SparqlParserBaseVisitor::visitPropertyListPathNotEmpty(ctx);
        }

        antlrcpp::Any visitVerbPath(Dice::tentris::SparqlParserBase::SparqlParser::VerbPathContext *ctx) override {
            return visitPath(ctx->path());
        }

        antlrcpp::Any visitPath(Dice::tentris::SparqlParserBase::SparqlParser::PathContext *ctx) override {
            return visitPathAlternative(ctx->pathAlternative());
        }

        //ToDo check later
//    antlrcpp::Any
//    visitPathAlternative(Dice::tentris::SparqlParserBase::Parser::PathAlternativeContext *ctx) override {
//        std::vector<PathSequence> pathSequences;
//        for (auto sequence:ctx->pathSequence())
//            pathSequences.push_back(visitPathSequence(sequence));
//        return pathSequences;
//    }

        antlrcpp::Any
        visitPathSequence(Dice::tentris::SparqlParserBase::SparqlParser::PathSequenceContext *ctx) override {
            std::vector<PropertySetElement> elements;
            for (auto element:ctx->pathEltOrInverse())
                elements.push_back(visitPathEltOrInverse(element));
            return elements;
        }

        antlrcpp::Any
        visitPathEltOrInverse(Dice::tentris::SparqlParserBase::SparqlParser::PathEltOrInverseContext *ctx) override {
            PropertySetElement element = visitPathElt(ctx->pathElt());
            if (ctx->INVERSE() != nullptr)
                element.setIsInversed(true);
            return element;
        }

        antlrcpp::Any visitPathElt(Dice::tentris::SparqlParserBase::SparqlParser::PathEltContext *ctx) override {
            PropertySetElement element = visitPathPrimary(ctx->pathPrimary());
            if (ctx->pathMod() != nullptr)
                element.setpathMode(visitPathMod(ctx->pathMod()));
            return element;
        }

        antlrcpp::Any visitPathPrimary(Dice::tentris::SparqlParserBase::SparqlParser::PathPrimaryContext *ctx) override {
            return SparqlParserBaseVisitor::visitPathPrimary(ctx);
        }


        //done
        antlrcpp::Any visitPathNegatedPropertySet(
                Dice::tentris::SparqlParserBase::SparqlParser::PathNegatedPropertySetContext *ctx) override {
            std::vector<PropertySetElement> elements;
            for (auto element:ctx->pathOneInPropertySet())
                elements.push_back(visitPathOneInPropertySet(element));
            return elements;
        }

        //done
        antlrcpp::Any
        visitPathOneInPropertySet(
                Dice::tentris::SparqlParserBase::SparqlParser::PathOneInPropertySetContext *ctx) override {
            bool isInvsersed;
            if (ctx->INVERSE() != nullptr)
                isInvsersed = true;
            else
                isInvsersed = false;
            //if it has an iri
            if (ctx->iri() != nullptr) {
                rdf_parser::store::rdf::URIRef iri = visitIri(ctx->iri());
                return IriPropertySetElement(iri, isInvsersed);
            }
                //then it has an A
            else
                return APropertySetElement(isInvsersed);
        }

        antlrcpp::Any visitIri(Dice::tentris::SparqlParserBase::SparqlParser::IriContext *ctx) override {
            //if the iri is IRIREF
            if (ctx->IRIREF() != nullptr) {
                Term term = Term::make_term(ctx->IRIREF()->getText());
                return term;
            }
                //else the iri is prefixedName
            else {

            }
        }


        //done
        antlrcpp::Any visitPathMod(Dice::tentris::SparqlParserBase::SparqlParser::PathModContext *ctx) override {
            std::string mod = ctx->getText();
            PathMode pathMode;
            if (mod == "?")
                pathMode = PathMode::ZeroOrOne;
            else if (mod == "*")
                pathMode = PathMode::Any;
            else if (mod == "+")
                pathMode = PathMode::MoreThanZero;
            return pathMode;
        }


        //partialy done ..check the return type
        antlrcpp::Any visitVarOrTerm(Dice::tentris::SparqlParserBase::SparqlParser::VarOrTermContext *ctx) override {



            //visit term
            if (ctx->graphTerm() != nullptr)
                return visitGraphTerm(ctx->graphTerm());
                //visit var
            else
                return visitVar(ctx->var());
        }

        virtual ~QueryGeneratorVisitor();


        //Done
        antlrcpp::Any visitVar(Dice::tentris::SparqlParserBase::SparqlParser::VarContext *ctx) override {
            if (ctx->VAR1() != nullptr) {
                return TripleVariable(ctx->VAR1()->getText().substr(1, ctx->VAR1()->getText().length() - 1));
            } else {
                return TripleVariable(ctx->VAR2()->getText().substr(1, ctx->VAR2()->getText().length() - 1));
            }

        }


        //Done
        antlrcpp::Any visitGraphTerm(Dice::tentris::SparqlParserBase::SparqlParser::GraphTermContext *ctx) override {
            std::string termRaw = ctx->getText();
            rdf_parser::store::rdf::Term term = rdf_parser::Turtle::parseTerm(termRaw);
            return term;
        }


        antlrcpp::Any visitVerbSimple(Dice::tentris::SparqlParserBase::SparqlParser::VerbSimpleContext *ctx) override {
            return visitVar(ctx->var());
        }


        antlrcpp::Any visitObject(Dice::tentris::SparqlParserBase::SparqlParser::ObjectContext *ctx) override;


        antlrcpp::Any visitTriplesNode(Dice::tentris::SparqlParserBase::SparqlParser::TriplesNodeContext *ctx) override;

        antlrcpp::Any visitCollection(Dice::tentris::SparqlParserBase::SparqlParser::CollectionContext *ctx) override;

        antlrcpp::Any
        visitBlankNodePropertyList(
                Dice::tentris::SparqlParserBase::SparqlParser::BlankNodePropertyListContext *ctx) override;

        antlrcpp::Any
        visitPropertyListNotEmpty(
                Dice::tentris::SparqlParserBase::SparqlParser::PropertyListNotEmptyContext *ctx) override;

    };

//ToDo
//antlrcpp::Any
//QueryGeneratorVisitor::visitObjectList(Dice::tentris::SparqlParserBase::Parser::ObjectListContext *ctx) {
//    std::vector<std::shared_ptr<AbstractTripleObject>> objects;
//    for (auto &object:ctx->object())
//        objects.push_back(visitObject(object));
//    return objects;
//}

    antlrcpp::Any QueryGeneratorVisitor::visitObject(Dice::tentris::SparqlParserBase::SparqlParser::ObjectContext *ctx) {
        return visitGraphNode(ctx->graphNode());
    }


////ToDo
//antlrcpp::Any
//QueryGeneratorVisitor::visitGraphNode(Dice::tentris::SparqlParserBase::Parser::GraphNodeContext *ctx) {
//    if (ctx->varOrTerm() != nullptr) {
//        return std::make_shared<SingleTripleObject>(visitVarOrTerm(ctx->varOrTerm()));
//    } else {
//        return std::make_shared<CompositeTripleObject>(visitTriplesNode(ctx->triplesNode()));
//    }
//}

    antlrcpp::Any
    QueryGeneratorVisitor::visitTriplesNode(Dice::tentris::SparqlParserBase::SparqlParser::TriplesNodeContext *ctx) {
        if (ctx->collection() != nullptr)
            return visitCollection(ctx->collection());
        else
            return visitBlankNodePropertyList(ctx->blankNodePropertyList());
    }

    antlrcpp::Any
    QueryGeneratorVisitor::visitCollection(Dice::tentris::SparqlParserBase::SparqlParser::CollectionContext *ctx) {
        return SparqlParserBaseVisitor::visitCollection(ctx);
    }

    antlrcpp::Any QueryGeneratorVisitor::visitBlankNodePropertyList(
            Dice::tentris::SparqlParserBase::SparqlParser::BlankNodePropertyListContext *ctx) {
        return SparqlParserBaseVisitor::visitBlankNodePropertyList(ctx);
    }

    antlrcpp::Any QueryGeneratorVisitor::visitPropertyListNotEmpty(
            Dice::tentris::SparqlParserBase::SparqlParser::PropertyListNotEmptyContext *ctx) {
        return SparqlParserBaseVisitor::visitPropertyListNotEmpty(ctx);
    }

    QueryGeneratorVisitor::~QueryGeneratorVisitor() {

    }



}

#endif //SPARQL_PARSER_QUERYGENERATORVISITOR_HPP
