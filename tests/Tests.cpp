#include <gtest/gtest.h>

#include "BasicBgpsTests.cpp"
#include "BasicOptionalPatternTests.cpp"
#include "ComplexOptionalPatternTests.cpp"
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
