#include <set>
#include <string>
#include <map>
#include <queue>
#include <iostream>
#include <list>
#include <exception>
#include <iterator>

using namespace std;

const char EPSILON = '&';

class FiniteAutomataException : public runtime_error {
public:
    using runtime_error::runtime_error;
};

class FiniteAutomataState {
    public:
    FiniteAutomataState() : state(""), symbols("") {}
    FiniteAutomataState(const FiniteAutomataState &fstate) {
        state = fstate.state;
        symbols = fstate.symbols;
    }
    string State() {
        return state;
    }
    void State(string state) {
        this->state = state;
    }
    string Symbols() {
        return symbols;
    }
    void Symbols(string symbols) {
        this->symbols = symbols;
    }
    bool operator< (const FiniteAutomataState &right) const
    {
        return state < right.state && symbols < right.symbols;
    }
    private:
    string state;
    string symbols;
};
class FiniteAutomata {
    class FiniteAutomataEnd {};
    class FiniteAutomataGenerator {
        public:
        FiniteAutomataGenerator(FiniteAutomata &f) : finite_automata(f) {
            FiniteAutomataState state;
            state.State(f.initial_state);
            state.Symbols("");
            actual_states.insert(state);
        }

        bool operator!=(FiniteAutomataGenerator _)
        {
            return !(generated.empty() && actual_states.empty());
        }

        FiniteAutomataGenerator begin() const
        {
            return *this;
        }

        FiniteAutomataGenerator end() const
        {
            return *this;
        }

        FiniteAutomataGenerator &operator++() {
            next();
            return *this;
        }

        string operator*() {
            return value();
        }

        string value() {
            if (generated.empty()) {
                // If the set is empty, we may try to generate new
                // elements on demand
                next();
            }
            string s = generated.front();
            generated.pop();
            return s;
        }
        private:
        void next() {
            while(generated.empty() && !actual_states.empty()) {
                set<FiniteAutomataState> nextStates;
                for (FiniteAutomataState actualState: actual_states) {
                    string state = actualState.State();
                    if (finite_automata.final_states.count(state)) {
                        generated.push(actualState.Symbols());
                    }
                    if (!finite_automata.transitions.count(state)) {
                        continue;
                    }
                    for (char symbol: finite_automata.alphabet) {
                         set<string> toStates = finite_automata.transitions.at(state)[symbol];
                         for (string toState: toStates) {
                             FiniteAutomataState nextState;
                             nextState.Symbols(actualState.Symbols()+symbol);
                             nextState.State(toState);
                             nextStates.insert(nextState);
                         }
                    }
                };
                actual_states = nextStates;
            }
        }
        FiniteAutomata &finite_automata;
        queue<string> generated;
        set<FiniteAutomataState> actual_states;
    };
public:
    FiniteAutomata() {
        alphabet.insert(EPSILON);
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
            map<char, set<string> > stateTransitions = transitions[state];
            set<string> closure = getClosure(state);
            int closureSize = closure.size()-1;
            if (closureSize > 1) {
                return true;
            }
            for (char symbol: alphabet) {
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
    void addSymbol(char symbol) {
        if ((symbol < '0' || symbol > '9') && (symbol < 'a' || symbol > 'z')) {
            throw FiniteAutomataException("Symbol should be between '0' and '9' or between 'a' and 'z'");
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

    void addTransition(string source, char symbol, string target) {
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
            map<char, set<string> > stateTransitions;
            stateTransitions = result.transitions[stateName];
            for (char symbol: result.alphabet) {
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
        set<string> newStates;
        queue<string> q;
        q.push(result.initial_state);
        while (!q.empty()) {
            string state = q.front();
            newStates.insert(state);
            q.pop();
            if (!result.transitions.count(state)) {
                continue;
            }
            for (char symbol: result.alphabet) {
                if (!result.transitions[state].count(symbol)) {
                    continue;
                }
                set<string> transition = result.transitions[state][symbol];
                for (string toState: transition) {
                    if (newStates.count(toState)) {
                        continue;
                    }
                    q.push(toState);
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

    bool accepts(string s) {
        set <string> actualStates;
        actualStates.insert(initial_state);
        for (char symbol: s) {
            set <string> nextStates;
            for (string state: actualStates) {
                set<string> tr = transitions[state][symbol];
                nextStates.insert(tr.begin(), tr.end());
            }
            actualStates = nextStates;
        }
        for (string state: actualStates) {
            if (final_states.count(state)) {
                return true;
            }
        }
        return false;
    }

    FiniteAutomataGenerator generates() {
        return FiniteAutomataGenerator(*this);
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
    set<char> alphabet;
    map<string, map<char, set<string> > > transitions;
    string initial_state;
    set<string> final_states;
};

#ifdef FINITE_AUTOMATA_TEST
int main() {
    FiniteAutomata f;
    f.addState("q0", true, true);
    f.addState("q1", false, false);
    f.addState("q3", false, false);
    f.addState("q4", false, false);
    f.addState("q5", false, false);
    f.addState("q6", false, true);
    f.addSymbol('a');
    f.addSymbol('b');
    f.addSymbol('c');
    f.addTransition("q0", 'a', "q1");
    f.addTransition("q1", 'b', "q3");
    f.addTransition("q3", 'b', "q4");
    f.addTransition("q4", 'a', "q5");
    f.addTransition("q5", 'a', "q6");

//    f.addTransition("q0", 'a', "q1");
//    f.addTransition("q0", 'b', "q1");
//    f.addTransition("q0", 'b', "q3");
//    f.addTransition("q0", 'c', "q3");
//    f.addTransition("q1", 'b', "q0");
//    f.addTransition("q1", 'b', "q6");
//    f.addTransition("q1", 'c', "q4");
//    f.addTransition("q1", 'c', "q6");
//    f.addTransition("q3", 'a', "q5");
//    f.addTransition("q3", 'a', "q6");
//    f.addTransition("q3", 'b', "q0");
//    f.addTransition("q3", 'b', "q6");
//    f.addTransition("q3", 'b', "q0");
//    f.addTransition("q4", 'a', "q1");
//    f.addTransition("q4", 'b', "q1");
//    f.addTransition("q4", 'b', "q3");
//    f.addTransition("q5", 'b', "q1");
//    f.addTransition("q5", 'b', "q3");
//    f.addTransition("q5", 'c', "q3");
    cout << f.nonDeterministic() << endl;
    list<FiniteAutomata> results = f.determinize();
    cout << results.size() << endl;
    cout << results.back().getStates() << endl;
    cout << results.back().nonDeterministic() << endl;
    for (string s: f.generates()) {
        cout << '"' << s << '"' << endl;
    }
    return 0;
}
#endif
