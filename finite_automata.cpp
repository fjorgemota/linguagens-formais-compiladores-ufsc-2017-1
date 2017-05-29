#include <set>
#include <string>
#include <map>
#include <queue>
#include <iostream>
#include <list>
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
    FiniteAutomata(const FiniteAutomata &f) {
        states = f.states;
        alphabet = f.alphabet;
        transitions = f.transitions;
        initial_state = f.initial_state;
        final_states = f.final_states;
    }

    bool nonDeterministic() {
        for (string state: states) {
            map<string, set<string> > stateTransitions = transitions[state];
            set<string> closure = getClosure(state);
            int closureSize = closure.size()-1;
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

    list<FiniteAutomata> determinize() {
        if (initial_state.empty()) {
            throw FiniteAutomataException("Initial State should be defined to determinize automata");
        }
        FiniteAutomata result(*this);
        queue<set<string> > q;
        set<string> initialState = getClosure(initial_state);
        q.push(initialState);
        result.initial_state = "";
        list<FiniteAutomata> results;
        while (!q.empty()) {
            set<string> states = q.front();
            q.pop();
            string stateName = formatStates(states);
            bool isFinal = false;
            for (string state: states) {
                if (result.final_states.count(state)) {
                    isFinal = true;
                    break;
                }
            }
            result.addState(stateName, states == initialState, isFinal);
            map<string, set<string> > stateTransitions;
            stateTransitions = result.transitions[stateName];
            for (string symbol: result.alphabet) {
                set<string> newTransitions;
                for (string state: states) {
                    if (!result.transitions.count(state)) {
                        continue;
                    }
                    set<string> tr = result.transitions[state][symbol];
                    newTransitions.insert(tr.begin(), tr.end());
                }
                for (string new_transition: newTransitions) {
                    set<string> closures = getClosure(new_transition);
                    newTransitions.insert(closures.begin(), closures.end());
                }
                if (newTransitions.empty()) {
                    continue;
                }
                string newTransitionsName = formatStates(newTransitions);
                stateTransitions[symbol].insert(newTransitionsName);
                if (result.states.count(newTransitionsName)) {
                    continue;
                }
                q.push(newTransitions);
            }
            result.transitions[stateName] = stateTransitions;
            results.push_back(result.removeDeadStates());
            result = FiniteAutomata(result);
        }
        return results;
    }

    FiniteAutomata removeDeadStates() {
        FiniteAutomata result(*this);
        set<string> oldStates, newStates;
        oldStates.insert(result.initial_state);
        while (oldStates != newStates) {
            newStates = oldStates;
            for (string state: oldStates) {
                if (!result.transitions.count(state)) {
                    continue;
                }
                for (string symbol: result.alphabet) {
                    if (!result.transitions[state].count(symbol)) {
                        continue;
                    }
                    set<string> transition = result.transitions[state][symbol];
                    oldStates.insert(transition.begin(), transition.end());
                }
            }
        }
        for (string state: result.states) {
            if (newStates.count(state)) {
                continue;
            }
            result.transitions.erase(state);
        }
        result.states = newStates;
        set <string> finalStates;
        for (string state: result.final_states) {
            if (!newStates.count(state)) {
                continue;
            }
            finalStates.insert(state);
        }
        result.final_states = finalStates;
        return result;
    }

    string getStates() {
        return formatStates(states);
    }

private:
    set<string> getClosure(string state) {
        set<string> results = set<string>();
        queue<string> q;
        q.push(state);
        while (!q.empty()) {
            string s = q.front();
            q.pop();
            if (s.empty() || results.count(s) == 1) {
                continue;
            }
            results.insert(s);
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
    f.addState("q0", true, true);
    f.addState("q1", false, false);
    f.addState("q3", false, false);
    f.addState("q4", false, false);
    f.addState("q5", false, false);
    f.addState("q6", false, true);
    f.addSymbol("a");
    f.addSymbol("b");
    f.addSymbol("c");
    f.addTransition("q0", "a", "q1");
    f.addTransition("q0", "b", "q1");
    f.addTransition("q0", "b", "q3");
    f.addTransition("q0", "c", "q3");
    f.addTransition("q1", "b", "q0");
    f.addTransition("q1", "b", "q6");
    f.addTransition("q1", "c", "q4");
    f.addTransition("q1", "c", "q6");
    f.addTransition("q3", "a", "q5");
    f.addTransition("q3", "a", "q6");
    f.addTransition("q3", "b", "q0");
    f.addTransition("q3", "b", "q6");
    f.addTransition("q3", "b", "q0");
    f.addTransition("q4", "a", "q1");
    f.addTransition("q4", "b", "q1");
    f.addTransition("q4", "b", "q3");
    f.addTransition("q5", "b", "q1");
    f.addTransition("q5", "b", "q3");
    f.addTransition("q5", "c", "q3");
    cout << f.nonDeterministic() << endl;
    list<FiniteAutomata> results = f.determinize();
    cout << results.size() << endl;
    cout << results.back().getStates() << endl;
    return 0;
}
