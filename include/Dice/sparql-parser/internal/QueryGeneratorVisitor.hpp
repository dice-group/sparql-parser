#ifndef SPARQL_PARSER_QUERYGENERATORVISITOR_HPP
#define SPARQL_PARSER_QUERYGENERATORVISITOR_HPP

#include <memory>
#include <vector>

#include <robin_hood.h>

#include <Dice/RDF/Term.hpp>
#include <Dice/sparql-parser/internal/TriplesBlockStringParser.hpp>
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
			prefixes = visitPrologue(ctx->prologue()).as<decltype(prefixes)>();

			//For now the parser only supports Select queries.
			if (ctx->selectQuery() != nullptr) {
				//create the select query
				return visitSelectQuery(ctx->selectQuery());
			} else
				return NotImplementedException();
		}

		antlrcpp::Any visitPrologue(Dice::sparql_parser::base::SparqlParser::PrologueContext *ctx) override {
			robin_hood::unordered_map<std::string, std::string> local_prefixes;
			if (ctx != nullptr) {
				if (ctx->baseDecl(0) != nullptr) {
					local_prefixes["Base"] = ctx->baseDecl(0)->IRIREF()->getText();
					local_prefixes["base"] = local_prefixes["Base"];
					local_prefixes[""] = local_prefixes["Base"];
				}
				// TODO: "Base", "base" and "" must map "" -> given IRI
				for (auto prefixStatement : ctx->prefixDecl()) {
					std::string pname = prefixStatement->PNAME_NS()->getText();
					pname = std::string(pname, 0, pname.size() - 1);
					std::string iriRef = prefixStatement->IRIREF()->getText();
					local_prefixes[pname] = std::string(iriRef, 1, iriRef.size() - 2);
				}
			}
			return local_prefixes;
		}

		antlrcpp::Any
		visitSelectQuery(Dice::sparql_parser::base::SparqlParser::SelectQueryContext *ctx) override {

			std::shared_ptr<SelectNode> selectNode;

			//visit where clause
			std::shared_ptr<Node> node = visitWhereClause(ctx->whereClause()).as<std::shared_ptr<QueryNodes::QueryNode>>();

			//get all solution modifiers. visitSolutionModifier() must handle the order.
			auto solutionModifiers = visitSolutionModifier(
											 ctx->solutionModifier())
											 .as<std::vector<std::shared_ptr<SolutionModifier>>>();

			//deal with the solution modifiers
			//go through the modifiers and add them to the decorator
			for (auto const &solutionModifier : solutionModifiers)
				node = std::make_shared<SolutionDecorator>(node, solutionModifier);

			//deal with the SelectClause to define the SelectNode Type(Distinct,Reduced,Default) and to get the select variables
			auto clause = visitSelectClause(ctx->selectClause()).as<SelectClause>();

			//create the selectNode

			switch (clause.nodeType) {

				case SelectNodeType::DEFAULT:
					selectNode = std::make_shared<DefaultSelectNode>(node, clause.selectVariables, prefixes);
					break;
				case SelectNodeType::DISTINCT:
					selectNode = std::make_shared<DistinctSelectNode>(node, clause.selectVariables, prefixes);
					break;
				case SelectNodeType::REDUCED:
					selectNode = std::make_shared<ReducedSelectNode>(node, clause.selectVariables, prefixes);
					break;
			}

			return selectNode;
		}


		antlrcpp::Any
		visitSelectClause(Dice::sparql_parser::base::SparqlParser::SelectClauseContext *ctx) override {

			SelectClause selectClause;

			//deal with node type
			if (ctx->selectModifier() != nullptr)
				selectClause.nodeType = visitSelectModifier(ctx->selectModifier());
			else
				selectClause.nodeType = SelectNodeType::DEFAULT;

			//deal with the variables
			if (ctx->ASTERISK() != nullptr)
				selectClause.selectVariables.emplace_back("*");
			else {
				for (auto selectVariable : ctx->selectVariables()) {
					selectClause.selectVariables.emplace_back(
							std::string(selectVariable->getText(), 1, selectVariable->getText().size() - 1));
				}
			}

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
				throw NotImplementedException();
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
					queryNode = groupNode;
				}
			}
			return queryNode;
		}


		antlrcpp::Any visitGroupGraphPatternSubList(
				Dice::sparql_parser::base::SparqlParser::GroupGraphPatternSubListContext *ctx) override {

			std::shared_ptr<QueryNodes::QueryNode> queryNode;

			//Deal with the triplesBlock
			if (ctx->triplesBlock() != nullptr) {
				auto groupNode = std::make_shared<QueryNodes::GroupingNodes::RegularGroupingNode>();
				groupNode->addChild(visitGraphPatternNotTriples(ctx->graphPatternNotTriples()));
				groupNode->addChild(visitTriplesBlock(ctx->triplesBlock()));
				queryNode = groupNode;
			} else
				queryNode = visitGraphPatternNotTriples(ctx->graphPatternNotTriples());

			return queryNode;
		}

		antlrcpp::Any
		visitTriplesBlock(Dice::sparql_parser::base::SparqlParser::TriplesBlockContext *ctx) override {
			std::shared_ptr<QueryNodes::QueryNode> queryNode;
			std::vector<sparql::TriplePattern> elements;
			//The following logic is to add (deleted)whitespaces between the tokens in order to process it by the rdf_parser
			auto getTriplesSameSubjectPathText =
					[&](Dice::sparql_parser::base::SparqlParser::TriplesBlockContext *ctx) -> std::string {
				std::string text;
				for (auto &it : ctx->triplesSameSubjectPath())
					text += static_cast<std::string>(visitTriplesSameSubjectPath(it)) + " . ";
				return text;
			};

			sparql_parser::internal::TriplesBlockStringParser parser(getTriplesSameSubjectPathText(ctx), prefixes);
			for (const auto &triple_pattern : parser)
				elements.push_back(triple_pattern);

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
				queryNode = std::move(visitGroupOrUnionGraphPattern(ctx->groupOrUnionGraphPattern()).as<std::shared_ptr<QueryNodes::QueryNode>>());
			} else if (ctx->optionalGraphPattern() != nullptr) {
				queryNode = std::move(visitOptionalGraphPattern(ctx->optionalGraphPattern()).as<std::shared_ptr<QueryNodes::QueryNode>>());
			} else if (ctx->minusGraphPattern() != nullptr) {
				queryNode = std::move(visitMinusGraphPattern(ctx->minusGraphPattern()).as<std::shared_ptr<QueryNodes::QueryNode>>());
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
			auto queryNode = std::move(visitGroupGraphPattern(ctx->groupGraphPattern()).as<std::shared_ptr<QueryNodes::QueryNode>>());

			return std::static_pointer_cast<QueryNodes::QueryNode>(
					std::make_shared<OptionalPatternNode>(queryNode));
		}

		antlrcpp::Any visitGroupOrUnionGraphPattern(
				Dice::sparql_parser::base::SparqlParser::GroupOrUnionGraphPatternContext *ctx) override {
			std::shared_ptr<QueryNodes::QueryNode> queryNode;
			//if there is no union. then just visit the groupGraphPattern
			if (ctx->UNION().empty())
				queryNode = visitGroupGraphPattern(ctx->groupGraphPattern()[0]);
			//else we add visit all the groupGraphPatterns and insert the result nodes into a union node.
			else {
				auto unionNode = std::make_shared<QueryNodes::GroupingNodes::UnionGroupingNode>();
				for (auto &groupGraphPattern : ctx->groupGraphPattern())
					unionNode->addChild(visitGroupGraphPattern(groupGraphPattern));
				queryNode = unionNode;
			}
			return queryNode;
		}

		antlrcpp::Any visitMinusGraphPattern(base::SparqlParser::MinusGraphPatternContext *ctx) override {
			auto queryNode = std::move(visitGroupGraphPattern(ctx->groupGraphPattern()).as<std::shared_ptr<QueryNodes::QueryNode>>());
			return std::static_pointer_cast<QueryNodes::QueryNode>(
					std::make_shared<MinusPatternNode>(queryNode));
		}

		~QueryGeneratorVisitor() override = default;
		;
	};

}// namespace Dice::sparql_parser::internal

#endif//SPARQL_PARSER_QUERYGENERATORVISITOR_HPP
