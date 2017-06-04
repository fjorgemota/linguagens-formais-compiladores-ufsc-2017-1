#include <gtest/gtest.h>
#include "finite_automata.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

class FiniteAutomataTest : public testing::Test {
    public:
        virtual void SetUp() {
            this->f = FiniteAutomata();
        }
    protected:
        FiniteAutomata f;
};

TEST_F(FiniteAutomataTest, addState) {
    ASSERT_FALSE(f.hasState("q0"));
    f.addState("q0");
    ASSERT_TRUE(f.hasState("q0"));
}


TEST_F(FiniteAutomataTest, addStateWithArrow) {
    ASSERT_FALSE(f.hasState("->q0"));
    ASSERT_FALSE(f.hasState("q0"));
    ASSERT_FALSE(f.isInitialState("q0"));
    f.addState("->q0");
    ASSERT_FALSE(f.hasState("->q0"));
    ASSERT_TRUE(f.hasState("q0"));
    ASSERT_TRUE(f.isInitialState("q0"));
}

TEST_F(FiniteAutomataTest, addStateWithAsterisk) {
    ASSERT_FALSE(f.hasState("*q0"));
    ASSERT_FALSE(f.hasState("q0"));
    ASSERT_FALSE(f.isFinalState("q0"));
    f.addState("*q0");
    ASSERT_FALSE(f.hasState("*q0"));
    ASSERT_TRUE(f.hasState("q0"));
    ASSERT_TRUE(f.isFinalState("q0"));
}


TEST_F(FiniteAutomataTest, addStateWithAsteriskAndArrow) {
    ASSERT_FALSE(f.hasState("->q0"));
    ASSERT_FALSE(f.hasState("*q0"));
    ASSERT_FALSE(f.hasState("*->q0"));
    ASSERT_FALSE(f.hasState("->*q0"));
    ASSERT_FALSE(f.hasState("q0"));
    ASSERT_FALSE(f.isFinalState("q0"));
    f.addState("*->q0");
    ASSERT_FALSE(f.hasState("->q0"));
    ASSERT_FALSE(f.hasState("*q0"));
    ASSERT_FALSE(f.hasState("*->q0"));
    ASSERT_FALSE(f.hasState("->*q0"));
    ASSERT_TRUE(f.hasState("q0"));
    ASSERT_TRUE(f.isInitialState("q0"));
    ASSERT_TRUE(f.isFinalState("q0"));
}


TEST_F(FiniteAutomataTest, addStateWithArrowAndAsterisk) {
    ASSERT_FALSE(f.hasState("->q0"));
    ASSERT_FALSE(f.hasState("*q0"));
    ASSERT_FALSE(f.hasState("*->q0"));
    ASSERT_FALSE(f.hasState("->*q0"));
    ASSERT_FALSE(f.hasState("q0"));
    ASSERT_FALSE(f.isFinalState("q0"));
    f.addState("->*q0");
    ASSERT_FALSE(f.hasState("->q0"));
    ASSERT_FALSE(f.hasState("*q0"));
    ASSERT_FALSE(f.hasState("*->q0"));
    ASSERT_FALSE(f.hasState("->*q0"));
    ASSERT_TRUE(f.hasState("q0"));
    ASSERT_TRUE(f.isInitialState("q0"));
    ASSERT_TRUE(f.isFinalState("q0"));
}

TEST_F(FiniteAutomataTest, addStateWithAlreadyAddedInitialState) {
    ASSERT_FALSE(f.hasState("q0"));
    f.addState("q0", FiniteAutomata::INITIAL_STATE);
    ASSERT_TRUE(f.hasState("q0"));
    ASSERT_THROW(f.addState("q0", FiniteAutomata::INITIAL_STATE), FiniteAutomataException);
}

TEST_F(FiniteAutomataTest, addSymbol) {
    ASSERT_FALSE(f.hasSymbol('a'));
    f.addSymbol('a');
    ASSERT_TRUE(f.hasSymbol('a'));
}

TEST_F(FiniteAutomataTest, addSymbolOutOfRange) {
    ASSERT_THROW(f.addSymbol('['), FiniteAutomataException);
}

TEST_F(FiniteAutomataTest, addTransition) {
    ASSERT_FALSE(f.hasTransition("q0", 'a', "q1"));
    f.addSymbol('a');
    f.addState("q0");
    f.addState("q1");
    f.addTransition("q0", 'a', "q1");
    ASSERT_TRUE(f.hasTransition("q0", 'a', "q1"));
}

TEST_F(FiniteAutomataTest, addTransitionWithoutSource) {
    ASSERT_FALSE(f.hasTransition("q0", 'a', "q1"));
    f.addSymbol('a');
    f.addState("q1");
    ASSERT_THROW(f.addTransition("q0", 'a', "q1"), FiniteAutomataException);
}

TEST_F(FiniteAutomataTest, addTransitionWithoutSymbol) {
    ASSERT_FALSE(f.hasTransition("q0", 'a', "q1"));
    f.addState("q0");
    f.addState("q1");
    ASSERT_THROW(f.addTransition("q0", 'a', "q1"), FiniteAutomataException);
}

TEST_F(FiniteAutomataTest, addTransitionWithoutTarget) {
    ASSERT_FALSE(f.hasTransition("q0", 'a', "q1"));
    f.addSymbol('a');
    f.addState("q0");
    ASSERT_THROW(f.addTransition("q0", 'a', "q1"), FiniteAutomataException);
}

TEST_F(FiniteAutomataTest, isInitialState) {
    ASSERT_FALSE(f.isInitialState("q0"));
    ASSERT_FALSE(f.isInitialState("q1"));
    f.addState("q0");
    f.addState("q1", FiniteAutomata::INITIAL_STATE);
    ASSERT_FALSE(f.isInitialState("q0"));
    ASSERT_TRUE(f.isInitialState("q1"));
}


TEST_F(FiniteAutomataTest, isFinalState) {
    ASSERT_FALSE(f.isFinalState("q0"));
    ASSERT_FALSE(f.isFinalState("q1"));
    ASSERT_FALSE(f.isFinalState("q2"));
    f.addState("q0");
    f.addState("q1", FiniteAutomata::INITIAL_STATE);
    f.addState("q2", FiniteAutomata::FINAL_STATE);
    ASSERT_FALSE(f.isFinalState("q0"));
    ASSERT_FALSE(f.isFinalState("q1"));
    ASSERT_TRUE(f.isFinalState("q2"));
}


TEST_F(FiniteAutomataTest, hasTransition) {
    ASSERT_FALSE(f.hasTransition("q0", 'a', "q1"));
    ASSERT_FALSE(f.hasTransition("q0", 'a', "q2"));
    f.addSymbol('a');
    f.addState("q0");
    f.addState("q1");
    f.addState("q2");
    f.addTransition("q0", 'a', "q1");
    ASSERT_TRUE(f.hasTransition("q0", 'a', "q1"));
    ASSERT_FALSE(f.hasTransition("q0", 'a', "q2"));
}


TEST_F(FiniteAutomataTest, getTransitions) {
    ASSERT_FALSE(f.getTransitions("q0", 'a').count("q0"));
    ASSERT_FALSE(f.getTransitions("q0", 'a').count("q1"));
    ASSERT_FALSE(f.getTransitions("q0", 'a').count("q2"));
    f.addSymbol('a');
    f.addState("q0");
    f.addState("q1");
    f.addState("q2");
    f.addTransition("q0", 'a', "q1");
    ASSERT_FALSE(f.getTransitions("q0", 'a').count("q0"));
    ASSERT_TRUE(f.getTransitions("q0", 'a').count("q1"));
    ASSERT_FALSE(f.getTransitions("q0", 'a').count("q2"));
    f.addTransition("q0", 'a', "q2");
    ASSERT_FALSE(f.getTransitions("q0", 'a').count("q0"));
    ASSERT_TRUE(f.getTransitions("q0", 'a').count("q1"));
    ASSERT_TRUE(f.getTransitions("q0", 'a').count("q2"));
}

TEST_F(FiniteAutomataTest, determinizeWithoutInitialState) {
    ASSERT_FALSE(f.isInitialState("q0"));
    f.addState("q0");
    ASSERT_FALSE(f.isInitialState("q0"));
    ASSERT_THROW(f.determinize(), FiniteAutomataException);
}


TEST_F(FiniteAutomataTest, determinize) {
    ASSERT_TRUE(f.isDeterministic());
    f.addSymbol('a');
    f.addSymbol('b');
    f.addState("->q0");
    f.addState("*q1");
    f.addState("q2");
    f.addTransition("q0", 'a', "q1");
    f.addTransition("q0", 'a', "q2");
    f.addTransition("q2", 'a', "q2");

    ASSERT_FALSE(f.isDeterministic());
    ASSERT_TRUE(f.hasState("q0"));
    ASSERT_TRUE(f.hasState("q1"));
    ASSERT_TRUE(f.hasState("q2"));
    ASSERT_TRUE(f.isFinalState("q1"));
    ASSERT_FALSE(f.isFinalState("q2"));
    ASSERT_TRUE(f.hasTransition("q0", 'a', "q1"));
    ASSERT_TRUE(f.hasTransition("q0", 'a', "q2"));
    FiniteAutomata d = f.determinize();
    ASSERT_TRUE(d.isDeterministic());
    ASSERT_FALSE(d.hasState("q0"));
    ASSERT_FALSE(d.hasState("q1"));
    ASSERT_FALSE(d.hasState("q2"));
    ASSERT_FALSE(d.isFinalState("q1"));
    ASSERT_FALSE(d.isFinalState("q2"));
    ASSERT_TRUE(d.hasState("[q0]"));
    ASSERT_TRUE(d.hasState("[q1,q2]"));
    ASSERT_TRUE(d.hasState("[q2]"));
    ASSERT_TRUE(d.isFinalState("[q1,q2]"));
    ASSERT_FALSE(d.hasTransition("q0", 'a', "q1"));
    ASSERT_FALSE(d.hasTransition("q0", 'a', "q2"));
    ASSERT_TRUE(d.hasTransition("[q0]", 'a', "[q1,q2]"));
    ASSERT_TRUE(d.hasTransition("[q1,q2]", 'a', "[q2]"));
}

TEST_F(FiniteAutomataTest, removeUnreachableStates) {
    f.addSymbol('a');
    f.addSymbol('b');
    f.addState("->q0");
    f.addState("*q1");
    f.addState("q2");
    f.addTransition("q0", 'a', "q1");
    f.addTransition("q0", 'b', "q1");
    ASSERT_TRUE(f.hasState("q0"));
    ASSERT_TRUE(f.hasState("q1"));
    ASSERT_TRUE(f.hasState("q2"));
    FiniteAutomata d = f.removeUnreachableStates();
    ASSERT_TRUE(d.hasState("q0"));
    ASSERT_TRUE(d.hasState("q1"));
    ASSERT_FALSE(d.hasState("q2"));
}


TEST_F(FiniteAutomataTest, removeDeadStates) {
    f.addSymbol('a');
    f.addSymbol('b');
    f.addState("->q0");
    f.addState("*q1");
    f.addState("q2");
    f.addState("q3");
    f.addTransition("q0", 'a', "q1");
    f.addTransition("q0", 'b', "q1");
    f.addTransition("q0", 'a', "q2");
    f.addTransition("q2", 'b', "q0");
    f.addTransition("q2", 'a', "q3");
    ASSERT_TRUE(f.hasState("q0"));
    ASSERT_TRUE(f.hasState("q1"));
    ASSERT_TRUE(f.hasState("q2"));
    ASSERT_TRUE(f.hasState("q3"));
    FiniteAutomata d = f.removeDeadStates();
    ASSERT_TRUE(d.hasState("q0"));
    ASSERT_TRUE(d.hasState("q1"));
    ASSERT_TRUE(d.hasState("q2"));
    ASSERT_FALSE(d.hasState("q3"));
}

TEST_F(FiniteAutomataTest, removeEquivalentStates) {
    f.addSymbol('a');
    f.addSymbol('b');
    f.addState("->q0");
    f.addState("q1");
    f.addState("q2");
    f.addState("*q3");
    f.addState("*q4");
    f.addTransition("q0", 'a', "q1");
    f.addTransition("q0", 'b', "q2");
    f.addTransition("q1", 'a', "q3");
    f.addTransition("q2", 'b', "q4");
    f.addTransition("q3", 'a', "q3");
    f.addTransition("q3", 'b', "q3");
    f.addTransition("q4", 'a', "q4");
    f.addTransition("q4", 'b', "q4");

    ASSERT_TRUE(f.hasState("q0"));
    ASSERT_TRUE(f.hasState("q1"));
    ASSERT_TRUE(f.hasState("q2"));
    ASSERT_TRUE(f.hasState("q3"));
    ASSERT_TRUE(f.hasState("q4"));
    FiniteAutomata d = f.removeEquivalentStates();

    ASSERT_FALSE(d.hasState("q0"));
    ASSERT_FALSE(d.hasState("q1"));
    ASSERT_FALSE(d.hasState("q2"));
    ASSERT_FALSE(d.hasState("q3"));
    ASSERT_FALSE(d.hasState("q4"));
    ASSERT_TRUE(d.hasState("[q0]"));
    ASSERT_TRUE(d.hasState("[q1]"));
    ASSERT_TRUE(d.hasState("[q2]"));
    ASSERT_TRUE(d.hasState("[q3,q4]"));
    ASSERT_TRUE(d.hasTransition("[q0]", 'a', "[q1]"));
    ASSERT_TRUE(d.hasTransition("[q0]", 'b', "[q2]"));
    ASSERT_TRUE(d.hasTransition("[q1]", 'a', "[q3,q4]"));
    ASSERT_TRUE(d.hasTransition("[q2]", 'b', "[q3,q4]"));
    ASSERT_TRUE(d.hasTransition("[q3,q4]", 'a', "[q3,q4]"));
    ASSERT_TRUE(d.hasTransition("[q3,q4]", 'b', "[q3,q4]"));
}


TEST_F(FiniteAutomataTest, removeEquivalentStatesNonDeterministic) {
    f.addSymbol('a');
    f.addSymbol('b');
    f.addState("->q0");
    f.addState("*q1");
    f.addState("q2");
    f.addTransition("q0", 'a', "q1");
    f.addTransition("q0", 'a', "q2");
    f.addTransition("q2", 'a', "q2");
    ASSERT_FALSE(f.isDeterministic());
    ASSERT_THROW(f.removeEquivalentStates(), FiniteAutomataException);
}
