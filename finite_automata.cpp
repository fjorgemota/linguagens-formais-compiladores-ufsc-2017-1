#include <set>
#include <string>
#include <map>
#include <queue>
#include <iostream>
#include <exception>

using namespace std;

const string EPSILON = "&";

class FiniteAutomataException : public runtime_error {
public:
    using runtime_error::runtime_error;
};


class FiniteAutomata {
public:
    FiniteAutomata() {
        addSymbol(EPSILON);
    }
    FiniteAutomata(FiniteAutomata &f) {
        states = f.states;
        alphabet = f.alphabet;
        transitions = f.transitions;
        initial_state = f.initial_state;
        final_states = f.final_states;
    }
    bool non_deterministic() {
        for (string state: states) {
            map<string, set<string> > stateTransitions = transitions[state];
            set<string> *closure = getClosure(state);
            int closureSize = closure->size()-1;
            delete closure;
            if (closureSize > 1) {
                return true;
            }
            for (string symbol: alphabet) {
                if (!stateTransitions.count(symbol) || symbol == EPSILON) {
                    continue;
                }
                int exits = stateTransitions[symbol].size()+closureSize;
                if (exits > 1) {
                    return true;
                }
            }
        }
        return false;
    }
    void addSymbol(string symbol) {
        if (symbol.empty()) {
            throw FiniteAutomataException("Symbol cannot be empty");
        }
        alphabet.insert(symbol);
    }

    void addState(string state, bool initialState=false, bool finalState=true) {
        if (!initial_state.empty() && initialState) {
            throw FiniteAutomataException("Initial State already defined");
        }
        states.insert(state);
        if (initialState) {
            initial_state = state;
        }
        if (finalState) {
            final_states.insert(state);
        }
    }

    void addTransition(string source, string symbol, string target) {
        if (states.count(source) == 0) {
            throw FiniteAutomataException("Source State is not a valid state");
        }
        if (alphabet.count(symbol) == 0) {
            throw FiniteAutomataException("Symbol not found in alphabet");
        }
        if (states.count(target) == 0) {
            throw FiniteAutomataException("Target State is not a valid state");
        }
        transitions[source][symbol].insert(target);
    }

    FiniteAutomata determinize() {
        if (initial_state.empty()) {

        }
    }
private:
    set<string>* getClosure(string state) {
        set<string> *results = new set<string>();
        queue<string> q;
        q.push(state);
        while (!q.empty()) {
            string s = q.front();
            q.pop();
            if (s.empty() || results->count(s) == 1) {
                continue;
            }
            results->insert(s);
            if (transitions[s].count(EPSILON)) {
                for (string state: transitions[s][EPSILON]) {
                    q.push(state);
                }
            }
        }
        return results;
    }
    string formatStates(set<string> states) {
        string s = "[";
        int c = states.size();
        for (string state: states) {
            s.append(state);
            c--;
            if (c > 0) {
                s.append(",");
            }
        }
        s.append("]");
        return s;
    }
    set<string> states;
    set<string> alphabet;
    map<string, map<string, set<string> > > transitions;
    string initial_state;
    set<string> final_states;
};

int main() {
    FiniteAutomata f;
    f.addSymbol("a");
    f.addSymbol("b");
    f.addState("q0", true, false);
    f.addState("q1", false, false);
    f.addState("q2", false, false);
    f.addTransition("q0", "a", "q1");
    f.addTransition("q0", "b", "q2");
    cout << f.non_deterministic() << endl;
    FiniteAutomata f2 = f;
    f2.addState("q3", false, false);
    f2.addTransition("q0", EPSILON, "q3");
    cout << f2.non_deterministic() << endl;
    return 0;
}
