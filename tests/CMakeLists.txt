cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

#include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#conan_basic_setup()


include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(sparql-parser-base REQUIRED)
find_package(rdf-parser REQUIRED)
find_package(sparql-queryGraph REQUIRED)

# add the executable for all tests
add_executable(tests Tests.cpp)
target_link_libraries(tests
        sparql-parser-base::sparql-parser-base
        sparql-parser
        )
set_property(TARGET tests PROPERTY CXX_STANDARD 17)


