#ifndef SPARQL_PARSER_SELECTNODETYPE_HPP
#define SPARQL_PARSER_SELECTNODETYPE_HPP

namespace Dice::sparql_parser::internal {
	enum class SelectNodeType {
		DEFAULT,
		DISTINCT,
		REDUCED
	};
}

#endif//SPARQL_PARSER_SELECTNODETYPE_HPP
