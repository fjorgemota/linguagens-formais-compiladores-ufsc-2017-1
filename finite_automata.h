#ifndef FINITE_AUTOMATA_H
#define FINITE_AUTOMATA_H

#include "all.h"


class FiniteAutomataException : public runtime_error {
public:
    using runtime_error::runtime_error;
};

class FiniteAutomataState {
    public:
        FiniteAutomataState();
        FiniteAutomataState(const FiniteAutomataState &fstate);
        string State();
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

    bool hasState(string state);

    bool hasSymbol(char symbol);

    bool isFinalState(string state);

    bool isInitialState(string state);

    bool isSymbol(char symbol);

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

    string toASCIITable();

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
