#include "split.h"

#include "gtest/gtest.h"

using StrVec = std::vector<std::string>;

TEST(TestSplitString, testSplit) {
    auto tokens = split("");
    ASSERT_EQ(tokens, StrVec{});
    tokens = split("1");
    ASSERT_EQ(tokens, StrVec{"1"});
    tokens = split("1,2");
    StrVec expected_result{"1", "2"};
    ASSERT_EQ(tokens, expected_result);
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
