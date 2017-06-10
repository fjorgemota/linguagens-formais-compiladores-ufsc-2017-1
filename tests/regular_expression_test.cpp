#include <gtest/gtest.h>
#include "node.cpp"
#include "regular_expression.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

class RegularExpressionTest : public testing::Test {
    public:
        virtual void SetUp() {
            this->re = new RegularExpression("(a|b)*");
        }
    protected:
        RegularExpression *re;
};

TEST_F(RegularExpressionTest, getLessPriority) {
    string r = "(a|b)*";
    ASSERT_EQ(re->getLessPriority(r), 5);

    r = "(a.b|a.c)*.a?|(b.a?.c)*";
    ASSERT_EQ(re->getLessPriority(r), 13);

    r = "1?.1?(0?.0.1.1?)*.0?.0?";
    ASSERT_EQ(re->getLessPriority(r), 2);

    r = "b?.(a|b.c)*";
    ASSERT_EQ(re->getLessPriority(r), 2);

    r = "(a.b|a.c)*.a";
    ASSERT_EQ(re->getLessPriority(r), 10);

    r = "1?.(0.1)*.0?";
    ASSERT_EQ(re->getLessPriority(r), 2);

    r = "1?.1?.(0.0?.1.1?)*.0?.0?";
    ASSERT_EQ(re->getLessPriority(r), 2);

    r = "(1|0)?.((1.0)*.(0.1)*)*.(1|0)?";
    ASSERT_EQ(re->getLessPriority(r), 6);
}
