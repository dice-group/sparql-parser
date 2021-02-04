#ifndef SPARQL_PARSER_QUERYGENERATORVISITOR_HPP
#define SPARQL_PARSER_QUERYGENERATORVISITOR_HPP

#include <memory>
#include <vector>

#include <robin_hood.h>

#include <Dice/RDF/Term.hpp>
#include <Dice/sparql-query/Nodes/QueryNodes/EmptyNode.hpp>
#include <Dice/sparql-query/Nodes/QueryNodes/GroupingNodes/FilteringDecorator.hpp>
#include <Dice/sparql-query/Nodes/QueryNodes/GroupingNodes/RegularGroupingNode.hpp>
#include <Dice/sparql-query/Nodes/QueryNodes/GroupingNodes/UnionGroupingNode.hpp>
#include <Dice/sparql-query/Nodes/QueryNodes/SelectNodes/DefaultSelectNode.hpp>
#include <Dice/sparql-query/Nodes/QueryNodes/SelectNodes/DistinctSelectNode.hpp>
#include <Dice/sparql-query/Nodes/QueryNodes/SelectNodes/ReducedSelectNode.hpp>
#include <Dice/sparql-query/Nodes/QueryNodes/SelectNodes/SelectNode.hpp>
#include <Dice/sparql-query/Nodes/QueryNodes/SpecialNodes/MinusPatternNode.hpp>
#include <Dice/sparql-query/Nodes/QueryNodes/SpecialNodes/OptionalPatternNode.hpp>
#include <Dice/sparql-query/Nodes/QueryNodes/leafNodes/TriplePatternNode.hpp>
#include <Dice/sparql-query/Nodes/SolutionDecorator.hpp>
#include <SparqlParser/SparqlParserBaseVisitor.h>
#include <Dice/sparql-parser/internal/TriplesBlockStringParser.hpp>


#include "Dice/sparql-parser/internal/Exceptions.hpp"
#include "Dice/sparql-parser/internal/SelectClause.hpp"
#include "Dice/sparql-parser/internal/SelectNodeType.hpp"


namespace Dice::sparql_parser::internal {

	using namespace sparql::Nodes::QueryNodes::SelectNodes;
	using namespace sparql::Nodes::QueryNodes::LeafNodes;
	using namespace sparql::Nodes::SolutionModifiers;
	using namespace sparql::Nodes::QueryNodes::SpecialNodes;
	using namespace sparql::Nodes;

	class QueryGeneratorVisitor : public Dice::sparql_parser::base::SparqlParserBaseVisitor {

	private:
		robin_hood::unordered_map<std::string, std::string> prefixes;

	public:
		antlrcpp::Any visitQuery(Dice::sparql_parser::base::SparqlParser::QueryContext *ctx) override {

			//get the prefixes
			prefixes = static_cast<robin_hood::unordered_map<std::string, std::string>>(visitPrologue(ctx->prologue()));

			//For now the parser only supports Select queries.
			if (ctx->selectQuery() != NULL) {
				std::shared_ptr<SelectNode> selectNode = visitSelectQuery(ctx->selectQuery());
				//create the select query
				return selectNode;
			} else
				return NotImplementedException();
		}

		antlrcpp::Any visitPrologue(Dice::sparql_parser::base::SparqlParser::PrologueContext *ctx) override {
			robin_hood::unordered_map<std::string, std::string> prefixes;
			if (ctx != nullptr) {
				if (ctx->baseDecl(0) != nullptr) {
					prefixes["Base"] = ctx->baseDecl(0)->IRIREF()->getText();
					prefixes["base"] = prefixes["Base"];
					prefixes[""] = prefixes["Base"];
				}
				for (auto prefixStatement : ctx->prefixDecl()) {
					std::string pname = prefixStatement->PNAME_NS()->getText();
					pname = std::string(pname, 0, pname.size() - 1);
					std::string iriRef = prefixStatement->IRIREF()->getText();
					prefixes[pname] = std::string(iriRef, 1, iriRef.size() - 2);
				}
			}
			return prefixes;
		}

		antlrcpp::Any
		visitSelectQuery(Dice::sparql_parser::base::SparqlParser::SelectQueryContext *ctx) override {

			std::shared_ptr<SelectNode> selectNode;
			std::shared_ptr<Node> node;

			//visit where clause
			std::shared_ptr<QueryNodes::QueryNode> queryNode = visitWhereClause(ctx->whereClause());

			//get all solution modifiers. visitSolutionModifier() must handle the order.
			std::vector<std::shared_ptr<SolutionModifier>> solutionModifiers = visitSolutionModifier(
					ctx->solutionModifier());

			//deal with the solution modifiers
			if (solutionModifiers.size() != 0) {

				//get the first modifier
				std::shared_ptr<SolutionModifier> firstModifier = solutionModifiers.front();
				solutionModifiers.pop_back();
				std::shared_ptr<SolutionDecorator> solutionDecorator = std::make_shared<SolutionDecorator>(queryNode,
																										   firstModifier);

				//go through the rest of the modifiers and add them to the decorator
				for (auto const &solutionModifier : solutionModifiers)
					solutionDecorator = std::make_shared<SolutionDecorator>(solutionDecorator, solutionModifier);

				node = solutionDecorator;
			} else
				node = queryNode;

			//deal with the SelectClause to define the SelectNode Type(Distinct,Reduced,Default) and to get the select variables
			SelectClause clause = visitSelectClause(ctx->selectClause());

			//create the selectNode
			if (clause.nodeType == SelectNodeType::DISTINCT)
				selectNode = std::make_shared<DistinctSelectNode>(node, clause.selectVariables, prefixes);
			else if (clause.nodeType == SelectNodeType::REDUCED)
				selectNode = std::make_shared<ReducedSelectNode>(node, clause.selectVariables, prefixes);
			else
				selectNode = std::make_shared<DefaultSelectNode>(node, clause.selectVariables, prefixes);

			return selectNode;
		}


		antlrcpp::Any
		visitSelectClause(Dice::sparql_parser::base::SparqlParser::SelectClauseContext *ctx) override {

			SelectClause selectClause;

			//deal with node type
			SelectNodeType nodeType;

			if (ctx->selectModifier() != nullptr)
				nodeType = visitSelectModifier(ctx->selectModifier());
			else
				nodeType = SelectNodeType::DEFAULT;

			selectClause.nodeType = nodeType;

			//deal with the variables
			std::vector<sparql::Variable> selectVariables;
			if (ctx->ASTERISK() != nullptr)
				selectVariables.push_back(sparql::Variable("*"));
			else {
				for (auto selectVariable : ctx->selectVariables()) {
					selectVariables.push_back(sparql::Variable(
							std::string(selectVariable->getText(), 1, selectVariable->getText().size() - 1)));
				}
			}
			selectClause.selectVariables = selectVariables;

			return selectClause;
		}

		antlrcpp::Any
		visitSelectModifier(Dice::sparql_parser::base::SparqlParser::SelectModifierContext *ctx) override {

			std::string nodeTypeRaw = ctx->getText();
			SelectNodeType nodeType;
			if (nodeTypeRaw == "DISTINCT")
				nodeType = SelectNodeType::DISTINCT;
			else if (nodeTypeRaw == "REDUCED")
				nodeType = SelectNodeType::REDUCED;
			else
				throw ParseException();
			return nodeType;
		}


		antlrcpp::Any
		visitSolutionModifier(Dice::sparql_parser::base::SparqlParser::SolutionModifierContext *ctx) override {
			std::vector<std::shared_ptr<SolutionModifier>> solutionModifiers;

			if (ctx->groupClause() != nullptr) {
				throw NotImplementedException();
			}

			if (ctx->havingClause() != nullptr) {
				throw NotImplementedException();
			}

			if (ctx->orderClause() != nullptr) {
				throw NotImplementedException();
			}

			if (ctx->limitOffsetClauses() != nullptr) {
				throw NotImplementedException();
			}

			return solutionModifiers;
		}


		antlrcpp::Any
		visitWhereClause(Dice::sparql_parser::base::SparqlParser::WhereClauseContext *ctx) override {
			return visitGroupGraphPattern(ctx->groupGraphPattern());
		}


		antlrcpp::Any
		visitGroupGraphPattern(Dice::sparql_parser::base::SparqlParser::GroupGraphPatternContext *ctx) override {
			std::shared_ptr<QueryNodes::QueryNode> queryNode;

			if (ctx->subSelect() != nullptr)
				queryNode = visitSubSelect(ctx->subSelect());
			else
				queryNode = visitGroupGraphPatternSub(ctx->groupGraphPatternSub());

			if (queryNode == nullptr)
				queryNode = std::make_shared<QueryNodes::EmptyNode>();
			return queryNode;
		}

		antlrcpp::Any
		visitGroupGraphPatternSub(
				Dice::sparql_parser::base::SparqlParser::GroupGraphPatternSubContext *ctx) override {

			std::shared_ptr<QueryNodes::QueryNode> queryNode;

			if (ctx->groupGraphPatternSubList().empty()) {
				if (ctx->triplesBlock() != nullptr) {
					queryNode = visitTriplesBlock(ctx->triplesBlock());
				} else {
					queryNode = std::make_shared<QueryNodes::EmptyNode>();
				}
			} else {
				if ((ctx->triplesBlock() == nullptr) && (ctx->groupGraphPatternSubList().size() == 1)) {
					queryNode = visitGroupGraphPatternSubList(ctx->groupGraphPatternSubList()[0]);
				} else {
					std::shared_ptr<QueryNodes::GroupingNodes::RegularGroupingNode> groupNode = std::make_shared<QueryNodes::GroupingNodes::RegularGroupingNode>();
					if (ctx->triplesBlock() != nullptr)
						groupNode->addChild(visitTriplesBlock(ctx->triplesBlock()));
					for (auto &subList : ctx->groupGraphPatternSubList())
						groupNode->addChild(visitGroupGraphPatternSubList(subList));
					queryNode = std::dynamic_pointer_cast<QueryNodes::QueryNode>(groupNode);
				}
			}
			return queryNode;
		}


		antlrcpp::Any visitGroupGraphPatternSubList(
				Dice::sparql_parser::base::SparqlParser::GroupGraphPatternSubListContext *ctx) override {

			std::shared_ptr<QueryNodes::QueryNode> queryNode;

			//Deal with the triplesBlock
			if (ctx->triplesBlock() != nullptr) {
				std::shared_ptr<QueryNodes::GroupingNodes::RegularGroupingNode> groupNode = std::make_shared<QueryNodes::GroupingNodes::RegularGroupingNode>();
				groupNode->addChild(visitGraphPatternNotTriples(ctx->graphPatternNotTriples()));
				groupNode->addChild(visitTriplesBlock(ctx->triplesBlock()));
				queryNode = std::dynamic_pointer_cast<QueryNodes::QueryNode>(groupNode);
			} else
				queryNode = visitGraphPatternNotTriples(ctx->graphPatternNotTriples());

			return queryNode;
		}

		antlrcpp::Any
		visitTriplesBlock(Dice::sparql_parser::base::SparqlParser::TriplesBlockContext *ctx) override {
			std::shared_ptr<QueryNodes::QueryNode> queryNode;
			std::vector<sparql::TriplePattern> elements;
			//The following logic is to add (deleted)whitespaces between the tokens in order to process it by the rdf_parser
			auto getTriplesSameSubjectPathText = [&](
														 Dice::sparql_parser::base::SparqlParser::TriplesBlockContext *ctx) -> std::string {
				std::string text = "";
				for (auto &it : ctx->triplesSameSubjectPath())
					text += static_cast<std::string>(visitTriplesSameSubjectPath(it)) + " . ";
				return text;
			};
			sparql_parser::internal::TriplesBlockStringParser parser(getTriplesSameSubjectPathText(ctx), prefixes);
			auto it = parser.begin();
			while (it) {
				//ToDo check this
				sparql::TriplePattern triple = (sparql::TriplePattern &&) * it;
				elements.push_back(triple);
				it++;
			}
			queryNode = std::make_shared<TriplePatternNode>(elements);
			return queryNode;
		}

		antlrcpp::Any visitTriplesSameSubjectPath(
				Dice::sparql_parser::base::SparqlParser::TriplesSameSubjectPathContext *ctx) override {
			std::string result;
			if (ctx->varOrTerm() != nullptr)
				result = ctx->varOrTerm()->getText() + " " +
						 static_cast<std::string>(visitPropertyListPathNotEmpty(ctx->propertyListPathNotEmpty()));
			else
				result = static_cast<std::string>(visitTriplesNodePath(ctx->triplesNodePath())) + " " +
						 static_cast<std::string>(visitPropertyListPath(ctx->propertyListPath()));
			return result;
		}


		antlrcpp::Any visitPropertyListPathNotEmpty(
				Dice::sparql_parser::base::SparqlParser::PropertyListPathNotEmptyContext *ctx) override {
			std::string result;
			std::string verb;
			if (ctx->verbPath() != nullptr)
				verb = ctx->verbPath()->getText();
			else
				verb = ctx->verbSimple()->getText();
			result = verb + " " + ctx->objectListPath()->getText();
			for (auto &it : ctx->propertyListPathNotEmptyList())
				result += " ; " + static_cast<std::string>(visitPropertyListPathNotEmptyList(it));
			return result;
		}

		antlrcpp::Any visitPropertyListPathNotEmptyList(
				Dice::sparql_parser::base::SparqlParser::PropertyListPathNotEmptyListContext *ctx) override {
			std::string result;
			if (ctx->verbPath() != nullptr)
				result = ctx->verbPath()->getText();
			else
				result = ctx->verbSimple()->getText();
			result = result + " " + ctx->objectList()->getText();
			return result;
		}

		antlrcpp::Any
		visitTriplesNodePath(Dice::sparql_parser::base::SparqlParser::TriplesNodePathContext *ctx) override {
			std::string result;
			if (ctx->collectionPath() != nullptr)
				result = ctx->collectionPath()->getText();
			else
				result = static_cast<std::string>(visitBlankNodePropertyListPath(ctx->blankNodePropertyListPath()));
			return result;
		}

		antlrcpp::Any visitBlankNodePropertyListPath(
				Dice::sparql_parser::base::SparqlParser::BlankNodePropertyListPathContext *ctx) override {
			return "[" + static_cast<std::string>(visitPropertyListPathNotEmpty(ctx->propertyListPathNotEmpty())) + "]";
		}

		antlrcpp::Any visitGraphPatternNotTriples(
				Dice::sparql_parser::base::SparqlParser::GraphPatternNotTriplesContext *ctx) override {

			std::shared_ptr<QueryNodes::QueryNode> queryNode;

			if (ctx->groupOrUnionGraphPattern() != nullptr) {
				queryNode = visitGroupOrUnionGraphPattern(ctx->groupOrUnionGraphPattern());
			} else if (ctx->optionalGraphPattern() != nullptr) {
				queryNode = visitOptionalGraphPattern(ctx->optionalGraphPattern());
			} else if (ctx->minusGraphPattern() != nullptr) {
				queryNode = visitMinusGraphPattern(ctx->minusGraphPattern());
			} else if (ctx->graphGraphPattern() != nullptr) {
				throw NotImplementedException();
			} else if (ctx->serviceGraphPattern() != nullptr) {
				throw NotImplementedException();
			} else if (ctx->filter() != nullptr) {
				throw NotImplementedException();
			} else if (ctx->bind() != nullptr) {
				throw NotImplementedException();
			} else {
				throw NotImplementedException();
			}


			return queryNode;
		}

		antlrcpp::Any visitOptionalGraphPattern(
				Dice::sparql_parser::base::SparqlParser::OptionalGraphPatternContext *ctx) override {
			std::shared_ptr<QueryNodes::QueryNode> queryNode = visitGroupGraphPattern(ctx->groupGraphPattern());
			std::shared_ptr<OptionalPatternNode> optionalNode = std::make_shared<OptionalPatternNode>(queryNode);
			std::shared_ptr<QueryNodes::QueryNode> innerNode = std::dynamic_pointer_cast<QueryNodes::QueryNode>(
					optionalNode);
			return innerNode;
		}

		antlrcpp::Any visitGroupOrUnionGraphPattern(
				Dice::sparql_parser::base::SparqlParser::GroupOrUnionGraphPatternContext *ctx) override {
			//if there is no union. then just visit the groupGraphPattern
			if (ctx->UNION().empty())
				return visitGroupGraphPattern(ctx->groupGraphPattern()[0]);
			//else we add visit all the groupGraphPatterns and insert the result nodes into a union node.
			else {
				std::shared_ptr<QueryNodes::GroupingNodes::UnionGroupingNode> unionNode = std::make_shared<QueryNodes::GroupingNodes::UnionGroupingNode>();
				for (auto &groupGraphPattern : ctx->groupGraphPattern())
					unionNode->addChild(visitGroupGraphPattern(groupGraphPattern));
				return unionNode;
			}
		}

		antlrcpp::Any visitMinusGraphPattern(base::SparqlParser::MinusGraphPatternContext *ctx) override {
			std::shared_ptr<QueryNodes::QueryNode> queryNode = visitGroupGraphPattern(ctx->groupGraphPattern());
			std::shared_ptr<MinusPatternNode> minusNode = std::make_shared<MinusPatternNode>(queryNode);
			std::shared_ptr<QueryNodes::QueryNode> innerNode = std::dynamic_pointer_cast<QueryNodes::QueryNode>(
					minusNode);
			return innerNode;
		}

		virtual ~QueryGeneratorVisitor(){};
	};

}// namespace Dice::sparql_parser::internal

#endif//SPARQL_PARSER_QUERYGENERATORVISITOR_HPP
