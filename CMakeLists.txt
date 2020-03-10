cmake_minimum_required(VERSION 3.13)

project(sparql-parser VERSION 0.9.0)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(sparql-parser  main.cpp)
target_link_libraries(sparql-parser ${CONAN_LIBS})