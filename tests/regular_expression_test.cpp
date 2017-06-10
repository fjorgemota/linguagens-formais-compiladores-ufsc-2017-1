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

        virtual void TearDown() {
            delete this->re;
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

TEST_F(RegularExpressionTest, getTree) {
    re = new RegularExpression("(a|b)*");
    Node* tree = re->getTree();

    ASSERT_EQ(tree->getParent()->getValue(), 'L');
    ASSERT_EQ(tree->getValue(), '*');
    ASSERT_EQ(tree->getType(), STAR);

    ASSERT_EQ(tree->getLeft()->getValue(), '|');
    ASSERT_EQ(tree->getLeft()->getType(), UNION);

    ASSERT_EQ(tree->getLeft()->getLeft()->getParent()->getValue(), '|');
    ASSERT_EQ(tree->getLeft()->getLeft()->getValue(), 'a');
    ASSERT_EQ(tree->getLeft()->getLeft()->getType(), LEAF);

    ASSERT_EQ(tree->getLeft()->getRight()->getParent()->getValue(), '*');
    ASSERT_EQ(tree->getLeft()->getRight()->getValue(), 'b');
    ASSERT_EQ(tree->getLeft()->getRight()->getType(), LEAF);

    ASSERT_FALSE(tree->getRight());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getRight());
    ASSERT_FALSE(tree->getLeft()->getRight()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getRight()->getRight());
}
