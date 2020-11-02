#include <gtest/gtest.h>

#include "GrammerTests.cpp"
#include "ParserTest.cpp"
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
