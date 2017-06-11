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

TEST_F(RegularExpressionTest, getTreeSimple) {
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

TEST_F(RegularExpressionTest, getTree) {
    re = new RegularExpression("1?1?(0?011?)*0?0?");
    Node *tree = re->getTree();

    ASSERT_EQ(tree->getValue(), '.');
    ASSERT_EQ(tree->getType(), DOT);

    ASSERT_EQ(tree->getLeft()->getValue(), '?');
    ASSERT_EQ(tree->getLeft()->getType(), QUESTION);
    ASSERT_EQ(tree->getLeft()->getParent()->getValue(), '.');
    ASSERT_EQ(tree->getLeft()->getParent()->getType(), DOT);

    ASSERT_EQ(tree->getLeft()->getLeft()->getValue(), '1');
    ASSERT_EQ(tree->getLeft()->getLeft()->getType(), LEAF);
    ASSERT_EQ(tree->getLeft()->getLeft()->getParent()->getValue(), '?');
    ASSERT_EQ(tree->getLeft()->getLeft()->getParent()->getType(), QUESTION);

    ASSERT_EQ(tree->getRight()->getValue(), '.');
    ASSERT_EQ(tree->getRight()->getType(), DOT);

    ASSERT_EQ(tree->getRight()->getLeft()->getValue(), '?');
    ASSERT_EQ(tree->getRight()->getLeft()->getType(), QUESTION);
    ASSERT_EQ(tree->getRight()->getLeft()->getParent()->getType(), DOT);
    ASSERT_EQ(tree->getRight()->getLeft()->getParent()->getValue(), '.');

    ASSERT_EQ(tree->getRight()->getLeft()->getLeft()->getValue(), '1');
    ASSERT_EQ(tree->getRight()->getLeft()->getLeft()->getType(), LEAF);
    ASSERT_EQ(tree->getRight()->getLeft()->getLeft()->getParent()->getType(),
            QUESTION);
    ASSERT_EQ(tree->getRight()->getLeft()->getLeft()->getParent()->getValue(),
            '?');
    
    ASSERT_EQ(tree->getRight()->getRight()->getLeft()->getValue(), '*');
    ASSERT_EQ(tree->getRight()->getRight()->getLeft()->getType(), STAR);
    ASSERT_EQ(tree->getRight()->getRight()->getLeft()->getParent()->getValue(),
            '.');
    ASSERT_EQ(tree->getRight()->getRight()->getLeft()->getParent()->getType(),
            DOT);

    ASSERT_EQ(tree->getRight()->getRight()->getLeft()->getLeft()->getValue(),
            '.');
    ASSERT_EQ(tree->getRight()->getRight()->getLeft()->getLeft()->getType(),
            DOT);
    
    ASSERT_EQ(
        tree->getRight()->getRight()->getLeft()->getLeft()->getLeft()->getValue(),
        '?');
    ASSERT_EQ(
        tree->getRight()->getRight()->getLeft()->getLeft()->getLeft()->getType(),
        QUESTION);

    ASSERT_EQ(
        tree->getRight()->getRight()->getLeft()->getLeft()->getLeft()->getLeft()->getType(),
        LEAF);
    ASSERT_EQ(
        tree->getRight()->getRight()->getLeft()->getLeft()->getLeft()->getLeft()->getValue(),
        '0');
    ASSERT_EQ(
        tree->getRight()->getRight()->getLeft()->getLeft()->getLeft()->getLeft()->getParent()->getValue(),
        '?');
    ASSERT_EQ(
        tree->getRight()->getRight()->getLeft()->getLeft()->getLeft()->getLeft()->getParent()->getType(),
        QUESTION);
}

TEST_F(RegularExpressionTest, getTreeStarStar) {
    re = new RegularExpression("(aa)**");

    Node *tree = re->getTree();

    ASSERT_EQ(tree->getValue(), '*');
    ASSERT_EQ(tree->getLeft()->getValue(), '.');
    ASSERT_EQ(tree->getLeft()->getLeft()->getValue(), 'a');
    ASSERT_EQ(tree->getLeft()->getRight()->getValue(), 'a');

    ASSERT_FALSE(tree->getRight());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getRight());
    ASSERT_FALSE(tree->getLeft()->getRight()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getRight()->getRight());
}

TEST_F(RegularExpressionTest, getTreeStarPlus) {
    re = new RegularExpression("(bb)*+");

    Node *tree = re->getTree();

    ASSERT_EQ(tree->getValue(), '*');
    ASSERT_EQ(tree->getParent()->getValue(), 'L');

    ASSERT_EQ(tree->getLeft()->getValue(), '.');
    ASSERT_EQ(tree->getLeft()->getLeft()->getValue(), 'b');
    ASSERT_EQ(tree->getLeft()->getLeft()->getParent()->getValue(), '.');
    ASSERT_EQ(tree->getLeft()->getRight()->getValue(), 'b');
    ASSERT_EQ(tree->getLeft()->getRight()->getParent()->getValue(), '*');

    ASSERT_FALSE(tree->getLeft()->getParent());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getRight());
    ASSERT_FALSE(tree->getLeft()->getRight()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getRight()->getRight());
}

TEST_F(RegularExpressionTest, getTreeStarQuestion) {
    re = new RegularExpression("(cc)*?");

    Node *tree = re->getTree();

    ASSERT_EQ(tree->getValue(), '*');
    ASSERT_EQ(tree->getParent()->getValue(), 'L');
    ASSERT_EQ(tree->getLeft()->getValue(), '.');
    ASSERT_EQ(tree->getLeft()->getLeft()->getValue(), 'c');
    ASSERT_EQ(tree->getLeft()->getLeft()->getParent()->getValue(), '.');
    ASSERT_EQ(tree->getLeft()->getRight()->getValue(), 'c');
    ASSERT_EQ(tree->getLeft()->getRight()->getParent()->getValue(), '*');

    ASSERT_FALSE(tree->getRight());
    ASSERT_FALSE(tree->getLeft()->getParent());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getRight());
    ASSERT_FALSE(tree->getLeft()->getRight()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getRight()->getLeft());
}

TEST_F(RegularExpressionTest, getTreePlusPlus) {
    re = new RegularExpression("(dd)++");

    Node *tree = re->getTree();

    ASSERT_EQ(tree->getValue(), '+');
    ASSERT_EQ(tree->getParent()->getValue(), 'L');
    ASSERT_EQ(tree->getLeft()->getValue(), '.');
    ASSERT_EQ(tree->getLeft()->getLeft()->getValue(), 'd');
    ASSERT_EQ(tree->getLeft()->getLeft()->getParent()->getValue(), '.');
    ASSERT_EQ(tree->getLeft()->getRight()->getValue(), 'd');
    ASSERT_EQ(tree->getLeft()->getRight()->getParent()->getValue(), '+');

    ASSERT_FALSE(tree->getRight());
    ASSERT_FALSE(tree->getLeft()->getParent());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getRight());
    ASSERT_FALSE(tree->getLeft()->getRight()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getRight()->getLeft());
}

TEST_F(RegularExpressionTest, getTreePlusStar) {
    re = new RegularExpression("(ee)+*");

    Node *tree = re->getTree();

    ASSERT_EQ(tree->getValue(), '*');
    ASSERT_EQ(tree->getParent()->getValue(), 'L');
    ASSERT_EQ(tree->getLeft()->getValue(), '.');
    ASSERT_EQ(tree->getLeft()->getLeft()->getValue(), 'e');
    ASSERT_EQ(tree->getLeft()->getLeft()->getParent()->getValue(), '.');
    ASSERT_EQ(tree->getLeft()->getRight()->getValue(), 'e');
    ASSERT_EQ(tree->getLeft()->getRight()->getParent()->getValue(), '*');

    ASSERT_FALSE(tree->getRight());
    ASSERT_FALSE(tree->getLeft()->getParent());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getRight());
    ASSERT_FALSE(tree->getLeft()->getRight()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getRight()->getLeft());
}

TEST_F(RegularExpressionTest, getTreePlusQuestion) {
    re = new RegularExpression("(ff)+?");

    Node *tree = re->getTree();

    ASSERT_EQ(tree->getValue(), '*');
    ASSERT_EQ(tree->getParent()->getValue(), 'L');
    ASSERT_EQ(tree->getLeft()->getValue(), '.');
    ASSERT_EQ(tree->getLeft()->getLeft()->getValue(), 'f');
    ASSERT_EQ(tree->getLeft()->getLeft()->getParent()->getValue(), '.');
    ASSERT_EQ(tree->getLeft()->getRight()->getValue(), 'f');
    ASSERT_EQ(tree->getLeft()->getRight()->getParent()->getValue(), '*');

    ASSERT_FALSE(tree->getRight());
    ASSERT_FALSE(tree->getLeft()->getParent());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getRight());
    ASSERT_FALSE(tree->getLeft()->getRight()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getRight()->getLeft());
}

TEST_F(RegularExpressionTest, getTreeQuestionPlus) {
    re = new RegularExpression("(gg)?+");

    Node *tree = re->getTree();

    ASSERT_EQ(tree->getValue(), '*');
    ASSERT_EQ(tree->getParent()->getValue(), 'L');
    ASSERT_EQ(tree->getLeft()->getValue(), '.');
    ASSERT_EQ(tree->getLeft()->getLeft()->getValue(), 'g');
    ASSERT_EQ(tree->getLeft()->getLeft()->getParent()->getValue(), '.');
    ASSERT_EQ(tree->getLeft()->getRight()->getValue(), 'g');
    ASSERT_EQ(tree->getLeft()->getRight()->getParent()->getValue(), '*');

    ASSERT_FALSE(tree->getRight());
    ASSERT_FALSE(tree->getLeft()->getParent());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getRight());
    ASSERT_FALSE(tree->getLeft()->getRight()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getRight()->getLeft());
}

TEST_F(RegularExpressionTest, getTreeQuestionStar) {
    re = new RegularExpression("(hh)?*");

    Node *tree = re->getTree();

    ASSERT_EQ(tree->getValue(), '*');
    ASSERT_EQ(tree->getParent()->getValue(), 'L');
    ASSERT_EQ(tree->getLeft()->getValue(), '.');
    ASSERT_EQ(tree->getLeft()->getLeft()->getValue(), 'h');
    ASSERT_EQ(tree->getLeft()->getLeft()->getParent()->getValue(), '.');
    ASSERT_EQ(tree->getLeft()->getRight()->getValue(), 'h');
    ASSERT_EQ(tree->getLeft()->getRight()->getParent()->getValue(), '*');

    ASSERT_FALSE(tree->getRight());
    ASSERT_FALSE(tree->getLeft()->getParent());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getRight());
    ASSERT_FALSE(tree->getLeft()->getRight()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getRight()->getLeft());
}

TEST_F(RegularExpressionTest, getTreeQuestionQuestion) {
    re = new RegularExpression("(ii)??");

    Node *tree = re->getTree();

    ASSERT_EQ(tree->getValue(), '?');
    ASSERT_EQ(tree->getParent()->getValue(), 'L');
    ASSERT_EQ(tree->getLeft()->getValue(), '.');
    ASSERT_EQ(tree->getLeft()->getLeft()->getValue(), 'i');
    ASSERT_EQ(tree->getLeft()->getLeft()->getParent()->getValue(), '.');
    ASSERT_EQ(tree->getLeft()->getRight()->getValue(), 'i');
    ASSERT_EQ(tree->getLeft()->getRight()->getParent()->getValue(), '?');

    ASSERT_FALSE(tree->getRight());
    ASSERT_FALSE(tree->getLeft()->getParent());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getRight());
    ASSERT_FALSE(tree->getLeft()->getRight()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getRight()->getLeft());
}