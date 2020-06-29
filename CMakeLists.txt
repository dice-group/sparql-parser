cmake_minimum_required(VERSION 3.13)

project(sparql-parser VERSION 0.9.0)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


# define a header-only library
add_library(sparql-parser  INTERFACE)
add_library(Dice::sparql-parser  ALIAS sparql-parser include/Dice/Sparql-Parser/TripleObject.hpp)

target_include_directories(sparql-parser  INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${sparql-parser_INSTALL_CMAKE_DIR}>
        )
target_link_libraries(sparql-parser INTERFACE  ${CONAN_LIBS})


# testing
option(sparql-parser_BUILD_TESTS "Build sparql-parser tests." ON)
if(sparql-parser_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()