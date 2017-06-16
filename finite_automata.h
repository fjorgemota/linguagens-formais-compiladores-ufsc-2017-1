#ifndef FINITE_AUTOMATA_H
#define FINITE_AUTOMATA_H

#include <all.h>

/*!
 * Exception that is emitted when an invalid operation is done in the
 * Finite Automata
 */
class FiniteAutomataException : public runtime_error {
public:
    using runtime_error::runtime_error;
};

/*!
 * Class that stores a state of the generator of the FiniteAutomata
 *
 * @see FiniteAutomata::FiniteAutomataGenerator
 */
class FiniteAutomataState {
    public:
        /*!
         * Initialize an empty FiniteAutomataState
         */
        FiniteAutomataState();

        /*!
         * Copy an existing FiniteAutomataState into a new object
         */
        FiniteAutomataState(const FiniteAutomataState &fstate);

        /*!
         * Return the State that this object represents
         *
         * @return The actual state of the automata
         */
        string State();

        /*!
         * Set the actual state that this object represents
         *
         * @param state The actual state of this object
         */
        void State(string state);

        string Symbols();
        void Symbols(string symbols);
    private:
        string state;
        string symbols;
};

class FiniteAutomata {
    class FiniteAutomataGenerator {
        public:
        FiniteAutomataGenerator(FiniteAutomata &f);

        bool operator!=(FiniteAutomataGenerator);

        FiniteAutomataGenerator begin() const;

        FiniteAutomataGenerator end() const;

        FiniteAutomataGenerator &operator++();

        string operator*();

        string value();
        private:
        void next();
        FiniteAutomata &finite_automata;
        queue<string> generated;
        queue<FiniteAutomataState> actual_states;
    };
public:
    FiniteAutomata();

    FiniteAutomata(const FiniteAutomata &f);

    bool isDeterministic() const;

    void addSymbol(char symbol);

    void addState(string state, int type = 0);

    bool hasState(string state) const;

    bool hasSymbol(char symbol) const;

    bool isFinalState(string state) const;

    bool isInitialState(string state) const;

    bool isSymbol(char symbol) const;

    bool hasTransition(string source, char symbol, string target);

    set<string> getTransitions(string source, char symbol);

    void addTransition(string source, char symbol, string target);

    FiniteAutomata determinize() const;

    FiniteAutomata removeUnreachableStates() const;

    FiniteAutomata removeDeadStates() const;

    FiniteAutomata removeEquivalentStates() const;

    bool accepts(string s);

    bool isComplete() const;

    FiniteAutomata complete() const;

    FiniteAutomata doUnion(FiniteAutomata other) const;

    FiniteAutomata doIntersection(FiniteAutomata other) const;

    FiniteAutomata doComplement() const;

    FiniteAutomata doDifference(FiniteAutomata other) const;

    string toASCIITable() const;

    FiniteAutomataGenerator generates();

    const static int FINAL_STATE;
    const static int INITIAL_STATE;
    const static char EPSILON;
private:
    set<string> getClosure(string state) const;
    string formatStates(set<string> states, bool brackets=true) const;

    void setStates(set<string> newStates, set<string> finalStates);

    void setStates(set<string> newStates);

    string findFreeName() const;

    set<string> states;
    set<char> alphabet;
    map<string, map<char, set<string> > > transitions;
    string initial_state;
    set<string> final_states;
};
#endif // FINITE_AUTOMATA_H
