#ifndef SPARQL_PARSER_EXCEPTIONS_HPP
#define SPARQL_PARSER_EXCEPTIONS_HPP

#include <exception>

namespace Dice::sparql_parser::internal {
	class ParseException : public std::exception {
	public:
		[[nodiscard]] const char *what() const noexcept override {
			return "The query can't be parsed.";
		}
	};


	class NotImplementedException : public std::exception {
	public:
		[[nodiscard]] const char *what() const noexcept override {
			return "This functionality is not implemented yet.";
		}
	};

}// namespace Dice::sparql_parser::internal
#endif//SPARQL_PARSER_EXCEPTIONS_HPP
