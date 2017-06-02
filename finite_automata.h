#ifndef FINITE_AUTOMATA_H
#define FINITE_AUTOMATA_H

#include "all.h"

const char EPSILON = '&';

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

    bool nonDeterministic();

    void addSymbol(char symbol);

    void addState(string state, bool initialState=false, bool finalState=false);

    void addTransition(string source, char symbol, string target);

    FiniteAutomata determinize();

    FiniteAutomata removeUnreachableStates();

    FiniteAutomata removeDeadStates();

    FiniteAutomata removeEquivalentStates();

    bool accepts(string s);

    bool isComplete();
    FiniteAutomata complete();

    FiniteAutomata doUnion(FiniteAutomata other);

    FiniteAutomata doIntersection(FiniteAutomata other);

    FiniteAutomata doComplement();

    FiniteAutomata doDifference(FiniteAutomata other);

    string toASCIITable();

    FiniteAutomataGenerator generates();

private:
    set<string> getClosure(string state);
    string formatStates(set<string> states, bool brackets=true);

    void setStates(set<string> newStates, set<string> finalStates);

    void setStates(set<string> newStates);

    string findFreeName();

    set<string> states;
    set<char> alphabet;
    map<string, map<char, set<string> > > transitions;
    string initial_state;
    set<string> final_states;
};
#endif // FINITE_AUTOMATA_H
