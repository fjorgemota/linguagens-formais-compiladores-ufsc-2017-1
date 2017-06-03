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
