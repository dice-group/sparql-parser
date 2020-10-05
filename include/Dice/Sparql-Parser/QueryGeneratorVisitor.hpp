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

#include <Dice/rdf_parser/RDF/Term.hpp>

#include <Dice/rdf_parser/Parser/Turtle/Parsers/StringParser.hpp>
#include <Dice/rdf_parser/Sparql/TriplePatternElement.hpp>


#include "SelectNodeType.hpp"
#include "SelectClause.hpp"
#include "PropertySetElement.hpp"
#include "Exceptions.hpp"


class QueryGeneratorVisitor : Dice::tentris::sparql::parser::SparqlParserBaseVisitor {
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

        SelectClause clause = visitSelectClause(ctx->selectClause());

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

//        if (ctx->selectModifier() != nullptr)
//            nodeType = visitSelectModifier(ctx->selectModifier());
//        else
//            nodeType = SelectNodeType::DEFAULT;
//
//        selectClause.nodeType = nodeType;

        //ToDo deal with the variables


        return selectClause;
    }

//    antlrcpp::Any
//    visitSelectModifier(Dice::tentris::sparql::parser::SparqlParser::SelectModifierContext *ctx) override {
//
//        std::string nodeTypeRaw = ctx->toString();
//        SelectNodeType nodeType;
//        if (nodeTypeRaw == "DISTINCT")
//            nodeType = SelectNodeType::DISTINCT;
//        else if (nodeTypeRaw == "REDUCE")
//            nodeType = SelectNodeType::REDUCED;
//        else
//            throw new ParseException();
//        return nodeType;
//    }


    antlrcpp::Any
    visitSolutionModifier(Dice::tentris::sparql::parser::SparqlParser::SolutionModifierContext *ctx) override {
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


    antlrcpp::Any visitWhereClause(Dice::tentris::sparql::parser::SparqlParser::WhereClauseContext *ctx) override {
        return visitGroupGraphPattern(ctx->groupGraphPattern());

    }


    antlrcpp::Any
    visitGroupGraphPattern(Dice::tentris::sparql::parser::SparqlParser::GroupGraphPatternContext *ctx) override {
        std::shared_ptr<ICommandNode> commandNode;

        //ToDo
        if (ctx->subSelect() != nullptr)
            commandNode = visitSubSelect(ctx->subSelect());
        else
            commandNode = visitGroupGraphPatternSub(ctx->groupGraphPatternSub());

        return commandNode;
    }

    antlrcpp::Any
    visitGroupGraphPatternSub(Dice::tentris::sparql::parser::SparqlParser::GroupGraphPatternSubContext *ctx) override {

        std::shared_ptr<ICommandNode> commandNode;

        //ToDo
        if (ctx->triplesBlock() != nullptr) {
            visitTriplesBlock(ctx->triplesBlock());
        }

        for (auto &subList:ctx->groupGraphPatternSubList()) {
            visitGroupGraphPatternSubList(subList);
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
        if (ctx->triplesBlock() != nullptr) {
            visitTriplesBlock(ctx->triplesBlock());
        }

        return commandNode;
    }

    antlrcpp::Any visitTriplesBlock(Dice::tentris::sparql::parser::SparqlParser::TriplesBlockContext *ctx) override {
        std::shared_ptr<ICommandNode> commandNode;
        std::vector<TriplePatternElement> elements;
        rdf_parser::Turtle::parsers::StringParser<true> parser(ctx->getText());
        auto it = parser.begin();
        while (it) {
            //ToDo check this
            TriplePatternElement  triple= (TriplePatternElement &&) *it;
            elements.push_back(triple);
            it++;
        }
        commandNode=std::make_shared<TriplePatternNode>(elements);
        return commandNode;
    }


public:

    antlrcpp::Any visitGraphPatternNotTriples(
            Dice::tentris::sparql::parser::SparqlParser::GraphPatternNotTriplesContext *ctx) override {

        std::shared_ptr<ICommandNode> commandNode;

        //ToDo
        if (ctx->optionalGraphPattern() != nullptr) {

        }
        //ToDo deal with the remaining types


        return commandNode;
    }





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

    //ToDo check later
//    antlrcpp::Any
//    visitPathAlternative(Dice::tentris::sparql::parser::SparqlParser::PathAlternativeContext *ctx) override {
//        std::vector<PathSequence> pathSequences;
//        for (auto sequence:ctx->pathSequence())
//            pathSequences.push_back(visitPathSequence(sequence));
//        return pathSequences;
//    }

    antlrcpp::Any visitPathSequence(Dice::tentris::sparql::parser::SparqlParser::PathSequenceContext *ctx) override {
        std::vector<PropertySetElement> elements;
        for (auto element:ctx->pathEltOrInverse())
            elements.push_back(visitPathEltOrInverse(element));
        return elements;
    }

    antlrcpp::Any
    visitPathEltOrInverse(Dice::tentris::sparql::parser::SparqlParser::PathEltOrInverseContext *ctx) override {
        PropertySetElement element = visitPathElt(ctx->pathElt());
        if (ctx->INVERSE() != nullptr)
            element.setIsInversed(true);
        return element;
    }

    antlrcpp::Any visitPathElt(Dice::tentris::sparql::parser::SparqlParser::PathEltContext *ctx) override {
        PropertySetElement element = visitPathPrimary(ctx->pathPrimary());
        if (ctx->pathMod() != nullptr)
            element.setpathMode(visitPathMod(ctx->pathMod()));
        return element;
    }

    antlrcpp::Any visitPathPrimary(Dice::tentris::sparql::parser::SparqlParser::PathPrimaryContext *ctx) override {
        return SparqlParserBaseVisitor::visitPathPrimary(ctx);
    }


    //done
    antlrcpp::Any visitPathNegatedPropertySet(
            Dice::tentris::sparql::parser::SparqlParser::PathNegatedPropertySetContext *ctx) override {
        std::vector<PropertySetElement> elements;
        for (auto element:ctx->pathOneInPropertySet())
            elements.push_back(visitPathOneInPropertySet(element));
        return elements;
    }

    //done
    antlrcpp::Any
    visitPathOneInPropertySet(Dice::tentris::sparql::parser::SparqlParser::PathOneInPropertySetContext *ctx) override {
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

    antlrcpp::Any visitIri(Dice::tentris::sparql::parser::SparqlParser::IriContext *ctx) override {
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
    antlrcpp::Any visitPathMod(Dice::tentris::sparql::parser::SparqlParser::PathModContext *ctx) override {
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
    antlrcpp::Any visitVarOrTerm(Dice::tentris::sparql::parser::SparqlParser::VarOrTermContext *ctx) override {



        //visit term
        if (ctx->graphTerm() != nullptr)
            return visitGraphTerm(ctx->graphTerm());
            //visit var
        else
            return visitVar(ctx->var());
    }

    virtual ~QueryGeneratorVisitor();


    //Done
    antlrcpp::Any visitVar(Dice::tentris::sparql::parser::SparqlParser::VarContext *ctx) override {
        if (ctx->VAR1() != nullptr) {
            return TripleVariable(ctx->VAR1()->getText().substr(1, ctx->VAR1()->getText().length() - 1));
        } else {
            return TripleVariable(ctx->VAR2()->getText().substr(1, ctx->VAR2()->getText().length() - 1));
        }

    }


    //Done
    antlrcpp::Any visitGraphTerm(Dice::tentris::sparql::parser::SparqlParser::GraphTermContext *ctx) override {
        std::string termRaw = ctx->getText();
        rdf_parser::store::rdf::Term term = rdf_parser::Turtle::parseTerm(termRaw);
        return term;
    }



    antlrcpp::Any visitVerbSimple(Dice::tentris::sparql::parser::SparqlParser::VerbSimpleContext *ctx) override {
        return visitVar(ctx->var());
    }


    antlrcpp::Any visitObject(Dice::tentris::sparql::parser::SparqlParser::ObjectContext *ctx) override;


    antlrcpp::Any visitTriplesNode(Dice::tentris::sparql::parser::SparqlParser::TriplesNodeContext *ctx) override;

    antlrcpp::Any visitCollection(Dice::tentris::sparql::parser::SparqlParser::CollectionContext *ctx) override;

    antlrcpp::Any
    visitBlankNodePropertyList(Dice::tentris::sparql::parser::SparqlParser::BlankNodePropertyListContext *ctx) override;

    antlrcpp::Any
    visitPropertyListNotEmpty(Dice::tentris::sparql::parser::SparqlParser::PropertyListNotEmptyContext *ctx) override;

};

//ToDo
//antlrcpp::Any
//QueryGeneratorVisitor::visitObjectList(Dice::tentris::sparql::parser::SparqlParser::ObjectListContext *ctx) {
//    std::vector<std::shared_ptr<AbstractTripleObject>> objects;
//    for (auto &object:ctx->object())
//        objects.push_back(visitObject(object));
//    return objects;
//}

antlrcpp::Any QueryGeneratorVisitor::visitObject(Dice::tentris::sparql::parser::SparqlParser::ObjectContext *ctx) {
    return visitGraphNode(ctx->graphNode());
}


////ToDo
//antlrcpp::Any
//QueryGeneratorVisitor::visitGraphNode(Dice::tentris::sparql::parser::SparqlParser::GraphNodeContext *ctx) {
//    if (ctx->varOrTerm() != nullptr) {
//        return std::make_shared<SingleTripleObject>(visitVarOrTerm(ctx->varOrTerm()));
//    } else {
//        return std::make_shared<CompositeTripleObject>(visitTriplesNode(ctx->triplesNode()));
//    }
//}

antlrcpp::Any
QueryGeneratorVisitor::visitTriplesNode(Dice::tentris::sparql::parser::SparqlParser::TriplesNodeContext *ctx) {
    if (ctx->collection() != nullptr)
        return visitCollection(ctx->collection());
    else
        return visitBlankNodePropertyList(ctx->blankNodePropertyList());
}

antlrcpp::Any
QueryGeneratorVisitor::visitCollection(Dice::tentris::sparql::parser::SparqlParser::CollectionContext *ctx) {
    return SparqlParserBaseVisitor::visitCollection(ctx);
}

antlrcpp::Any QueryGeneratorVisitor::visitBlankNodePropertyList(
        Dice::tentris::sparql::parser::SparqlParser::BlankNodePropertyListContext *ctx) {
    return SparqlParserBaseVisitor::visitBlankNodePropertyList(ctx);
}

antlrcpp::Any QueryGeneratorVisitor::visitPropertyListNotEmpty(
        Dice::tentris::sparql::parser::SparqlParser::PropertyListNotEmptyContext *ctx) {
    return SparqlParserBaseVisitor::visitPropertyListNotEmpty(ctx);
}

QueryGeneratorVisitor::~QueryGeneratorVisitor() {

}



#endif //SPARQL_PARSER_QUERYGENERATORVISITOR_HPP
