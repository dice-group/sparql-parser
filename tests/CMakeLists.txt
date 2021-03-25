
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.8.1
        GIT_SHALLOW TRUE
)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest ALIAS gtest)
add_library(GTest::Main ALIAS gtest_main)

include(GoogleTest)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)


find_package(sparql-query REQUIRED)

# add the executable for all tests
add_executable(tests Tests.cpp)
target_link_libraries(tests
        sparql-parser
        GTest::GTest
        GTest::Main
        )
set_property(TARGET tests PROPERTY CXX_STANDARD 20)


