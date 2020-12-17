//
// Created by fakhr on 11.03.20.
//

#ifndef SPARQL_PARSER_QUERYGENERATORVISITOR_HPP
#define SPARQL_PARSER_QUERYGENERATORVISITOR_HPP

#include <memory>
#include <vector>
#include <map>

#include <SparqlParser/SparqlParserBaseVisitor.h>

#include <Dice/Sparql-Query/QueryNodes/SelectNodes/SelectNode.hpp>
#include <Dice/Sparql-Query/QueryNodes/SelectNodes/DefaultSelectNode.hpp>
#include <Dice/Sparql-Query/QueryNodes/SelectNodes/DistinctSelectNode.hpp>
#include <Dice/Sparql-Query/QueryNodes/SelectNodes/ReducedSelectNode.hpp>
#include <Dice/Sparql-Query/QueryNodes/leafNodes/TriplePatternNode.hpp>
#include <Dice/Sparql-Query/QueryNodes/SolutionDecorator.hpp>
#include <Dice/Sparql-Query/TriplePatternElement.hpp>
#include <Dice/Sparql-Query/QueryNodes/SpecialNodes/OptionalPatternNode.hpp>
#include <Dice/Sparql-Query/QueryNodes/GroupNode.hpp>

#include <Dice/rdf_parser/RDF/Term.hpp>
#include <Dice/rdf_parser/Parser/Turtle/Parsers/StringParser.hpp>


#include "SelectNodeType.hpp"
#include "SelectClause.hpp"
#include "PropertySetElement.hpp"
#include "Exceptions.hpp"


namespace SparqlParser::internal {
    using namespace SparqlQueryGraph::Nodes::GraphNodes;
    using namespace SparqlQueryGraph::Nodes::SelectNodes;
    using namespace SparqlQueryGraph::Nodes::Modifiers;
    using namespace SparqlQueryGraph::Nodes;
    class QueryGeneratorVisitor : public Dice::tentris::SparqlParserBase::SparqlParserBaseVisitor {

    private:
        std::map<std::string,std::string> prefixes;

    public:


        antlrcpp::Any visitQuery(Dice::tentris::SparqlParserBase::SparqlParser::QueryContext *ctx) override {

            //get the prefiexes
            prefixes=static_cast<std::map<std::string,std::string>>(visitPrologue(ctx->prologue()));

            //For now the parser only supports Select queries.
            if (ctx->selectQuery() != NULL) {
                std::shared_ptr<SelectNode> selectNode= visitSelectQuery(ctx->selectQuery());
                //create the select query
                return selectNode;
            }
            else
                return NotImplementedException();

        }

    public:

        antlrcpp::Any visitPrologue(Dice::tentris::SparqlParserBase::SparqlParser::PrologueContext *ctx) override {
            std::map<std::string, std::string> prefixes;
            if (ctx != nullptr) {
                if (ctx->baseDecl(0) != nullptr) {
                    prefixes["Base"] = ctx->baseDecl(0)->IRIREF()->getText();
                    prefixes["base"] = prefixes["Base"];
                    prefixes[""] = prefixes["Base"];
                }
                for (auto prefixStatement:ctx->prefixDecl()) {
                    std::string pname = prefixStatement->PNAME_NS()->getText();
                    pname=std::string(pname, 0, pname.size() - 1);
                    std::string iriRef = prefixStatement->IRIREF()->getText();
                    prefixes[pname] = std::string(iriRef, 1, iriRef.size() - 2);
                }
            }
            return prefixes;

        }

        antlrcpp::Any visitSelectQuery(Dice::tentris::SparqlParserBase::SparqlParser::SelectQueryContext *ctx) override {

            std::shared_ptr<SelectNode> selectNode;
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
                selectNode = std::make_shared<DistinctSelectNode>(queryNode, clause.selectVariables,prefixes);
            else if (clause.nodeType == SelectNodeType::REDUCED)
                selectNode = std::make_shared<ReducedSelectNode>(queryNode, clause.selectVariables,prefixes);
            else
                selectNode = std::make_shared<DefaultSelectNode>(queryNode, clause.selectVariables,prefixes);

            return selectNode;

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
            if(ctx->ASTERISK()!= nullptr)
                selectVariables.push_back(TripleVariable("*"));
            else {
                for (auto selectVariable:ctx->selectVariables()) {
                    selectVariables.push_back(TripleVariable(
                            std::string(selectVariable->getText(), 1, selectVariable->getText().size() - 1)));
                }
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

            if(ctx->groupGraphPatternSubList().empty()){
                if (ctx->triplesBlock() != nullptr) {
                    commandNode=visitTriplesBlock(ctx->triplesBlock());
                }
            }
            else{
                if((ctx->triplesBlock() == nullptr)&&(ctx->groupGraphPatternSubList().size()==1)){
                    commandNode=visitGroupGraphPatternSubList(ctx->groupGraphPatternSubList()[0]);
                }
                else{
                std::shared_ptr<GroupNode> groupNode = std::make_shared<GroupNode>();
                if (ctx->triplesBlock() != nullptr)
                    groupNode->addChild(visitTriplesBlock(ctx->triplesBlock()));
                for (auto &subList:ctx->groupGraphPatternSubList())
                    groupNode->addChild(visitGroupGraphPatternSubList(subList));
                commandNode = std::dynamic_pointer_cast<ICommandNode>(groupNode);
                }
            }
            return commandNode;

        }


    public:
        antlrcpp::Any visitGroupGraphPatternSubList(
                Dice::tentris::SparqlParserBase::SparqlParser::GroupGraphPatternSubListContext *ctx) override {

            std::shared_ptr<ICommandNode> commandNode;

            //Deal with the triplesBlock
            if (ctx->triplesBlock() != nullptr) {
                std::shared_ptr<GroupNode> groupNode = std::make_shared<GroupNode>();
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
            //The following logic is to add (deleted)whitespaces between the tokens in order to process it by the rdf_parser
            auto getTriplesSameSubjectPathText =[&](Dice::tentris::SparqlParserBase::SparqlParser::TriplesBlockContext *ctx) -> std::string {
                std::string text="";
                for(auto& it:ctx->triplesSameSubjectPath())
                    text+=static_cast<std::string>(visitTriplesSameSubjectPath(it))+" . ";
                return text;
            };
            rdf_parser::Turtle::parsers::StringParser<true> parser(getTriplesSameSubjectPathText(ctx),prefixes);
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

        antlrcpp::Any visitTriplesSameSubjectPath(
                Dice::tentris::SparqlParserBase::SparqlParser::TriplesSameSubjectPathContext *ctx) override {
            std::string result;
            if(ctx->varOrTerm() != nullptr)
                result= ctx->varOrTerm()->getText()+" "+static_cast<std::string>(visitPropertyListPathNotEmpty(ctx->propertyListPathNotEmpty()));
                else
                    result=static_cast<std::string>(visitTriplesNodePath(ctx->triplesNodePath()))+" "+static_cast<std::string>(visitPropertyListPath(ctx->propertyListPath()));
            return result;
        }


        antlrcpp::Any visitPropertyListPathNotEmpty(
                Dice::tentris::SparqlParserBase::SparqlParser::PropertyListPathNotEmptyContext *ctx) override {
            std::string result;
            std::string verb;
            if (ctx->verbPath() != nullptr)
                verb = ctx->verbPath()->getText();
            else
                verb = ctx->verbSimple()->getText();
            result = verb + " " + ctx->objectListPath()->getText();
            for (auto &it:ctx->propertyListPathNotEmptyList())
                result += " ; " + static_cast<std::string>(visitPropertyListPathNotEmptyList(it));
            return result;
        }

        antlrcpp::Any visitPropertyListPathNotEmptyList(
                Dice::tentris::SparqlParserBase::SparqlParser::PropertyListPathNotEmptyListContext *ctx) override {
            std::string result;
            if (ctx->verbPath() != nullptr)
                throw SparqlParser::internal::NotImplementedException();
            else
                result = ctx->verbSimple()->getText() + " " +ctx->objectList()->getText();
            return result;
        }

        antlrcpp::Any
        visitTriplesNodePath(Dice::tentris::SparqlParserBase::SparqlParser::TriplesNodePathContext *ctx) override {
            std::string result;
            if(ctx->collectionPath()!= nullptr)
                result=ctx->collectionPath()->getText();
            else
                result=static_cast<std::string>(visitBlankNodePropertyListPath(ctx->blankNodePropertyListPath()));
            return result;

        }

        antlrcpp::Any visitBlankNodePropertyListPath(
                Dice::tentris::SparqlParserBase::SparqlParser::BlankNodePropertyListPathContext *ctx) override {
            return "["+static_cast<std::string>(visitPropertyListPathNotEmpty(ctx->propertyListPathNotEmpty()))+"]";
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
            std::shared_ptr<ICommandNode> node=visitGroupGraphPattern(ctx->groupGraphPattern());
            std::shared_ptr<OptionalPatternNode> optionalNode=std::make_shared<OptionalPatternNode>(node);
            std::shared_ptr<ICommandNode> commandNode=std::dynamic_pointer_cast<ICommandNode>(optionalNode);
            return commandNode;


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
