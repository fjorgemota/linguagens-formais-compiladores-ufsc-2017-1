#include <gtest/gtest.h>
#include "node.cpp"
#include "finite_automata.cpp"
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
    ASSERT_EQ(tree->getParent()->getType(), LAMBDA);

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
    ASSERT_EQ(tree->getParent()->getType(), LAMBDA);
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
    ASSERT_EQ(tree->getParent()->getType(), LAMBDA);
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
    ASSERT_EQ(tree->getParent()->getType(), LAMBDA);
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
    ASSERT_EQ(tree->getParent()->getType(), LAMBDA);
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
    ASSERT_EQ(tree->getParent()->getType(), LAMBDA);
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
    ASSERT_EQ(tree->getParent()->getType(), LAMBDA);
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
    ASSERT_EQ(tree->getParent()->getType(), LAMBDA);
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
    ASSERT_FALSE(tree->getLeft()->getRight()->getRight());
}

TEST_F(RegularExpressionTest, getTreeConcatenateSubExpression) {
    re = new RegularExpression("(ab)(cd)");

    Node *tree = re->getTree();

    ASSERT_EQ(tree->getValue(), '.');
    ASSERT_EQ(tree->getLeft()->getValue(), '.');
    ASSERT_EQ(tree->getLeft()->getLeft()->getValue(), 'a');
    ASSERT_EQ(tree->getLeft()->getLeft()->getParent(), tree->getLeft());
    ASSERT_EQ(tree->getLeft()->getRight()->getValue(), 'b');
    ASSERT_EQ(tree->getLeft()->getRight()->getParent(), tree);

    ASSERT_EQ(tree->getRight()->getLeft()->getValue(), 'c');
    ASSERT_EQ(tree->getRight()->getLeft()->getParent(), tree->getRight());
    ASSERT_EQ(tree->getRight()->getRight()->getValue(), 'd');
    ASSERT_EQ(tree->getRight()->getRight()->getParent()->getType(), LAMBDA);

    ASSERT_FALSE(tree->getParent());
    ASSERT_FALSE(tree->getLeft()->getParent());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getLeft()->getRight());
    ASSERT_FALSE(tree->getLeft()->getRight()->getLeft());
    ASSERT_FALSE(tree->getLeft()->getRight()->getRight());

    ASSERT_FALSE(tree->getRight()->getParent());
    ASSERT_FALSE(tree->getRight()->getLeft()->getLeft());
    ASSERT_FALSE(tree->getRight()->getLeft()->getRight());
    ASSERT_FALSE(tree->getRight()->getRight()->getLeft());
    ASSERT_FALSE(tree->getRight()->getRight()->getRight());
}

TEST_F(RegularExpressionTest, getTreeConcatenateMess) {
    re = new RegularExpression("(a|b)+++++*****?**+a");

    Node *tree = re->getTree();

    ASSERT_EQ(tree->getValue(), '.');
    ASSERT_EQ(tree->getLeft()->getValue(), '*');
    ASSERT_EQ(tree->getLeft()->getParent()->getType(), DOT);

    ASSERT_EQ(tree->getLeft()->getLeft()->getValue(), '|');
    ASSERT_EQ(tree->getLeft()->getLeft()->getLeft()->getValue(), 'a');
    ASSERT_EQ(tree->getLeft()->getLeft()->getLeft()->getParent()->getType(),
            UNION);

    ASSERT_EQ(tree->getLeft()->getLeft()->getRight()->getValue(), 'b');
    ASSERT_EQ(tree->getLeft()->getLeft()->getRight()->getParent()->getType(),
            STAR);

    ASSERT_EQ(tree->getRight()->getValue(), 'a');
    ASSERT_EQ(tree->getRight()->getParent()->getType(), LAMBDA);
}

TEST_F(RegularExpressionTest, getTreeEmpty) {
    re = new RegularExpression("");

    Node *tree = re->getTree();
    ASSERT_EQ(tree->getType(), LAMBDA);
}

TEST_F(RegularExpressionTest, getAutomataSimple) {
    re = new RegularExpression("(a|bc)*");

    FiniteAutomata f = re->getAutomata();

    ASSERT_TRUE(f.hasState("q0"));
    ASSERT_TRUE(f.isFinalState("q0"));
    ASSERT_TRUE(f.hasState("q1"));
    ASSERT_TRUE(f.hasTransition("q0", 'a', "q0"));
    ASSERT_TRUE(f.hasTransition("q0", 'b', "q1"));
    ASSERT_TRUE(f.hasTransition("q1", 'c', "q0"));
}

TEST_F(RegularExpressionTest, getAutomata6a) {
    re = new RegularExpression("(ab|ac)*a?|(ba?c)*");

    FiniteAutomata f = re->getAutomata();

    ASSERT_TRUE(f.hasState("q0"));
    ASSERT_TRUE(f.hasState("q1"));
    ASSERT_TRUE(f.hasState("q2"));
    ASSERT_TRUE(f.hasState("q3"));
    ASSERT_TRUE(f.hasState("q4"));
    ASSERT_TRUE(f.hasState("q5"));

    ASSERT_TRUE(f.isFinalState("q0"));
    ASSERT_TRUE(f.isFinalState("q1"));
    ASSERT_TRUE(f.isFinalState("q3"));
    ASSERT_TRUE(f.isFinalState("q5"));

    ASSERT_TRUE(f.hasTransition("q0", 'a', "q1"));
    ASSERT_TRUE(f.hasTransition("q0", 'b', "q2"));
    ASSERT_TRUE(f.hasTransition("q1", 'b', "q3"));
    ASSERT_TRUE(f.hasTransition("q1", 'c', "q3"));
    ASSERT_TRUE(f.hasTransition("q2", 'a', "q4"));
    ASSERT_TRUE(f.hasTransition("q2", 'c', "q5"));
    ASSERT_TRUE(f.hasTransition("q3", 'a', "q1"));
    ASSERT_TRUE(f.hasTransition("q4", 'c', "q5"));
    ASSERT_TRUE(f.hasTransition("q5", 'b', "q2"));
}

TEST_F(RegularExpressionTest, getAutomata6c) {
    re = new RegularExpression("1?1?(0?011?)*0?0?");

    FiniteAutomata f = re->getAutomata();

    ASSERT_TRUE(f.hasState("q0"));
    ASSERT_TRUE(f.hasState("q1"));
    ASSERT_TRUE(f.hasState("q2"));
    ASSERT_TRUE(f.hasState("q3"));
    ASSERT_TRUE(f.hasState("q4"));
    ASSERT_TRUE(f.hasState("q5"));

    ASSERT_TRUE(f.isFinalState("q0"));
    ASSERT_TRUE(f.isFinalState("q1"));
    ASSERT_TRUE(f.isFinalState("q2"));
    ASSERT_TRUE(f.isFinalState("q3"));
    ASSERT_TRUE(f.isFinalState("q4"));
    ASSERT_TRUE(f.isFinalState("q5"));

    ASSERT_TRUE(f.hasTransition("q0", '0', "q1"));
    ASSERT_TRUE(f.hasTransition("q0", '1', "q2"));
    ASSERT_TRUE(f.hasTransition("q1", '0', "q3"));
    ASSERT_TRUE(f.hasTransition("q1", '1', "q4"));
    ASSERT_TRUE(f.hasTransition("q2", '0', "q1"));
    ASSERT_TRUE(f.hasTransition("q2", '1', "q5"));
    ASSERT_TRUE(f.hasTransition("q3", '1', "q4"));
    ASSERT_TRUE(f.hasTransition("q4", '0', "q1"));
    ASSERT_TRUE(f.hasTransition("q4", '1', "q5"));
    ASSERT_TRUE(f.hasTransition("q5", '0', "q1"));
}

TEST_F(RegularExpressionTest, getAutomataEquivalence12a) {
    re = new RegularExpression("1?(01)*0?");
    FiniteAutomata f1 = re->getAutomata();

    re = new RegularExpression("0?(10)*1?");
    FiniteAutomata f2 = re->getAutomata();

    ASSERT_TRUE(f1.hasState("q0"));
    ASSERT_TRUE(f1.hasState("q1"));
    ASSERT_TRUE(f1.hasState("q2"));

    ASSERT_TRUE(f1.isFinalState("q0"));
    ASSERT_TRUE(f1.isFinalState("q1"));
    ASSERT_TRUE(f1.isFinalState("q2"));

    ASSERT_TRUE(f1.hasTransition("q0", '0', "q1"));
    ASSERT_TRUE(f1.hasTransition("q0", '1', "q2"));
    ASSERT_TRUE(f1.hasTransition("q1", '1', "q2"));
    ASSERT_TRUE(f1.hasTransition("q2", '0', "q1"));

    ASSERT_TRUE(f2.hasState("q0"));
    ASSERT_TRUE(f2.hasState("q1"));
    ASSERT_TRUE(f2.hasState("q2"));

    ASSERT_TRUE(f2.isFinalState("q0"));
    ASSERT_TRUE(f2.isFinalState("q1"));
    ASSERT_TRUE(f2.isFinalState("q2"));

    ASSERT_TRUE(f2.hasTransition("q0", '0', "q1"));
    ASSERT_TRUE(f2.hasTransition("q0", '1', "q2"));
    ASSERT_TRUE(f2.hasTransition("q1", '1', "q2"));
    ASSERT_TRUE(f2.hasTransition("q2", '0', "q1"));

    ASSERT_TRUE(f1.isEquivalent(f2));
    ASSERT_TRUE(f2.isEquivalent(f1));
}

TEST_F(RegularExpressionTest, getAutomataEquivalence12b) {
    re = new RegularExpression("1?1?(00?11?)*0?0?");
    FiniteAutomata f1 = re->getAutomata();

    re = new RegularExpression("(1|0)?((10)*(01)*)*(1|0)?");
    FiniteAutomata f2 = re->getAutomata();

    ASSERT_TRUE(f1.hasState("q0"));
    ASSERT_TRUE(f1.hasState("q1"));
    ASSERT_TRUE(f1.hasState("q2"));
    ASSERT_TRUE(f1.hasState("q3"));
    ASSERT_TRUE(f1.hasState("q4"));
    ASSERT_TRUE(f1.hasState("q5"));

    ASSERT_TRUE(f1.isFinalState("q0"));
    ASSERT_TRUE(f1.isFinalState("q1"));
    ASSERT_TRUE(f1.isFinalState("q2"));
    ASSERT_TRUE(f1.isFinalState("q3"));
    ASSERT_TRUE(f1.isFinalState("q4"));
    ASSERT_TRUE(f1.isFinalState("q5"));

    ASSERT_TRUE(f1.hasTransition("q0", '0', "q1"));
    ASSERT_TRUE(f1.hasTransition("q0", '1', "q2"));
    ASSERT_TRUE(f1.hasTransition("q1", '0', "q3"));
    ASSERT_TRUE(f1.hasTransition("q1", '1', "q4"));
    ASSERT_TRUE(f1.hasTransition("q2", '0', "q1"));
    ASSERT_TRUE(f1.hasTransition("q2", '1', "q5"));
    ASSERT_TRUE(f1.hasTransition("q3", '1', "q4"));
    ASSERT_TRUE(f1.hasTransition("q4", '0', "q1"));
    ASSERT_TRUE(f1.hasTransition("q4", '1', "q5"));
    ASSERT_TRUE(f1.hasTransition("q5", '0', "q1"));

    ASSERT_TRUE(f2.hasState("q0"));
    ASSERT_TRUE(f2.hasState("q1"));
    ASSERT_TRUE(f2.hasState("q2"));
    ASSERT_TRUE(f2.hasState("q3"));
    ASSERT_TRUE(f2.hasState("q4"));
    ASSERT_TRUE(f2.hasState("q5"));

    ASSERT_TRUE(f2.isFinalState("q0"));
    ASSERT_TRUE(f2.isFinalState("q1"));
    ASSERT_TRUE(f2.isFinalState("q2"));
    ASSERT_TRUE(f2.isFinalState("q3"));
    ASSERT_TRUE(f2.isFinalState("q4"));
    ASSERT_TRUE(f2.isFinalState("q5"));

    ASSERT_TRUE(f2.hasTransition("q0", '0', "q1"));
    ASSERT_TRUE(f2.hasTransition("q0", '1', "q2"));
    ASSERT_TRUE(f2.hasTransition("q1", '0', "q3"));
    ASSERT_TRUE(f2.hasTransition("q1", '1', "q2"));
    ASSERT_TRUE(f2.hasTransition("q2", '0', "q1"));
    ASSERT_TRUE(f2.hasTransition("q2", '1', "q4"));
    ASSERT_TRUE(f2.hasTransition("q3", '1', "q5"));
    ASSERT_TRUE(f2.hasTransition("q4", '0', "q5"));
    ASSERT_TRUE(f2.hasTransition("q5", '0', "q3"));
    ASSERT_TRUE(f2.hasTransition("q5", '1', "q4"));

    ASSERT_FALSE(f1.isEquivalent(f2));
    ASSERT_FALSE(f2.isEquivalent(f1));
}

TEST_F(RegularExpressionTest, getAutomataTest) {
    re = new RegularExpression("(a)*(b)");
    FiniteAutomata f = re->getAutomata();

    ASSERT_TRUE(f.hasState("q0"));
    ASSERT_TRUE(f.hasState("q1"));

    ASSERT_FALSE(f.isFinalState("q0"));
    ASSERT_TRUE(f.isFinalState("q1"));

    ASSERT_TRUE(f.hasTransition("q0", 'a', "q0"));
    ASSERT_TRUE(f.hasTransition("q0", 'b', "q1"));
    ASSERT_FALSE(f.hasTransition("q1", 'a', "q1"));
    ASSERT_FALSE(f.hasTransition("q1", 'a', "q0"));
    ASSERT_FALSE(f.hasTransition("q1", 'b', "q1"));
    ASSERT_FALSE(f.hasTransition("q1", 'b', "q0"));
}
