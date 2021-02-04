# Tentris sparql parser

This is the SPARQL parser used by [Tentris](https://github.com/dice-group/tentris). It uses [ANTLR4](https://github.com/antlr/antlr4),
It supports currently parsing select basic Sparql queries with optional. It generates a [Sparql Query](https://github.com/dice-group/sparql-query) to provide a flexible representation of the parsed query.


## how to build
### prerequisites

install conan, cmake and a C++20 compiler

add conan remotes
```
conan remote add dice "https://api.bintray.com/conan/dice-group/tentris"

```

### build
The build has been tested with gcc 10 compiler.
```
mkdir build
cd build
conan install .. --build=missing --settings compiler.libcxx="libstdc++11" --settings compiler=gcc --settings compiler.version=10
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
make -j
```

### conan 

recipe : sparql-parser/0.2.0@dice-group/stable

```
conan remote add dice "https://api.bintray.com/conan/dice-group/tentris"

```

## tests

To compile the tests, run 
`cmake -G "Unix Makefiles" -DSPARQL_PARSER_BUILD_TESTS=ON .. `


### Usage

The parser is easy to use, for parsing queries include the header `<Dice/sparql-parser/Parser.hpp>` and use the static function `Dice::sparql_parser::parseSelectQuery(std::string query)` which returns `std::shared_ptr<SelectNode>`.
 The information about the parsed query can be accessed through the returned pointer.

Example :

```c++
std::string query{
    "SELECT ?s ?t ?e WHERE"
    " {?s <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://localhost/vocabulary/bench/Journal> ."
    " ?s <http://purl.org/dc/elements/1.1/title> ?t ."
    " OPTIONAL{?s <http://swrc.ontoware.org/ontology#editor> ?e}}"};
std::shared_ptr<SelectNode> selectNode=Dice::sparql_parser::parseSelectQuery(query);
auto operands=selectNode->getOperands();
auto bgps=selectNode->getBgps();
auto  prefixes=selectNode->getPrefixes();
auto subscriptResult=selectNode->getSubscriptResult();
auto selectVariables=selectNode->getSelectVariables();
auto selectModifier=selectNode->getSelectModifier();
```
