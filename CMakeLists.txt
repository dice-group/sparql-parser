cmake_minimum_required(VERSION 3.13)

project(sparql-parser
        VERSION 0.1.0
        LANGUAGES CXX
        )

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# installation directories
set(sparql-parser_INSTALL_INCLUDE_DIR "include" CACHE STRING "The installation include directory")
set(sparql-parser_INSTALL_CMAKE_DIR "share/sparql-parser/cmake" CACHE STRING "The installation cmake directory")

# define a header-only library
add_library(sparql-parser  INTERFACE)
add_library(Dice::sparql-parser  ALIAS sparql-parser )

target_include_directories(sparql-parser  INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${sparql-parser_INSTALL_CMAKE_DIR}>
        )
target_link_libraries(sparql-parser INTERFACE  ${CONAN_LIBS})

# require C++17
target_compile_features(sparql-parser INTERFACE cxx_std_17)

# Make package findable
configure_file(cmake/dummy-config.cmake.in sparql-parser-config.cmake @ONLY)

# Enable version checks in find_package
include(CMakePackageConfigHelpers)
write_basic_package_version_file(sparql-parser-config-version.cmake COMPATIBILITY SameMajorVersion)

# install and export target
install(TARGETS sparql-parser EXPORT sparql-parser-targets)

install(EXPORT sparql-parser-targets
        FILE sparql-parser-config.cmake
        NAMESPACE Dice::
        DESTINATION ${sparql-parser_INSTALL_CMAKE_DIR}
        )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sparql-parser-config-version.cmake DESTINATION ${sparql-parser_INSTALL_CMAKE_DIR})
install(DIRECTORY include/ DESTINATION ${sparql-parser_INSTALL_INCLUDE_DIR})


# testing
option(sparql-parser_BUILD_TESTS "Build sparql-parser tests.")
if(sparql-parser_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
unset(sparql-parser_BUILD_TESTS CACHE)