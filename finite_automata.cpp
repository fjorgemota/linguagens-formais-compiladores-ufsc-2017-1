#include "all.h"

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
            actual_states.push(state);
        }

        bool operator!=(FiniteAutomataGenerator _)
        {
            return !actual_states.empty() || !generated.empty();
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
                while (!actual_states.empty()) {
                    FiniteAutomataState actualState = actual_states.front();
                    actual_states.pop();
                    string state = actualState.State();
                    if (finite_automata.final_states.count(state)) {
                        generated.push(actualState.Symbols());
                    }
                    if (!finite_automata.transitions.count(state)) {
                        continue;
                    }
                    for (char symbol: finite_automata.alphabet) {
                         set<string> toStates = finite_automata.transitions[state][symbol];
                         for (string toState: toStates) {
                             FiniteAutomataState nextState;
                             string nextString;
                             nextString.append(actualState.Symbols());
                             nextString.append(1, symbol);
                             nextState.Symbols(nextString);
                             nextState.State(toState);
                             actual_states.push(nextState);
                         }
                    }
                }
            }
        }
        FiniteAutomata &finite_automata;
        queue<string> generated;
        queue<FiniteAutomataState> actual_states;
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
            for (const char &symbol: result.alphabet) {
                if (symbol == EPSILON) {
                    continue;
                }
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
            results.push_back(result.removeUnreachableStates());
            result = FiniteAutomata(result);
        }
        return results;
    }

    FiniteAutomata removeUnreachableStates() {
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
        result.setStates(newStates);
        return result;
    }

    FiniteAutomata removeDeadStates() {
        FiniteAutomata result(*this);
        set<string> newStates;
        queue<string> q;
        for (string state: result.final_states) {
            q.push(state);
        }
        while (!q.empty()) {
            string state = q.front();
            newStates.insert(state);
            q.pop();
            for (string fromState: result.states) {
                if (!result.transitions.count(fromState)) {
                    continue;
                }
                for (char symbol: result.alphabet) {
                    if (!result.transitions[fromState].count(symbol)) {
                        continue;
                    }
                    if (result.transitions[fromState][symbol].count(state)) {
                        if (newStates.count(fromState)) {
                            continue;
                        }
                        q.push(fromState);
                    }
                }
            }
        }
        result.setStates(newStates);
        return result;
    }

    FiniteAutomata removeEquivalentStates() {
        if (nonDeterministic()) {
            throw FiniteAutomataException("This method works only on deterministic finite automata");
        }
        FiniteAutomata result(*this);
        set<set<string> > oldEquivalenceClasses, newEquivalenceClasses;
        set<string> K, KF;
        K = result.final_states;
        for (string state: result.states) {
            if (K.count(state)) {
                continue;
            }
            KF.insert(state);
        }
        KF.insert("&");
        newEquivalenceClasses.insert(K);
        newEquivalenceClasses.insert(KF);
        while (oldEquivalenceClasses != newEquivalenceClasses) {
            oldEquivalenceClasses = newEquivalenceClasses;
            newEquivalenceClasses.clear();
            for (set<string> equivalenceClass: oldEquivalenceClasses) {
                map<list<int>, set<string>> equivalenceClasses;
                for (string state: equivalenceClass) {
                    list<int> foundEquivalenceClasses;
                    for (char symbol: result.alphabet) {
                        set<string> transition = result.transitions[state][symbol];
                        int i = 0;
                        for (set<string> foundEquivalenceClass: oldEquivalenceClasses) {
                            bool found = false;
                            if (transition.empty()) {
                                if (foundEquivalenceClass.count("&")) {
                                    foundEquivalenceClasses.push_back(i);
                                    found = true;
                                }
                            } else {
                                for (string toState: transition) {
                                    if (foundEquivalenceClass.count(toState)) {
                                        foundEquivalenceClasses.push_back(i);
                                        found = true;
                                        break;
                                    }
                                }
                            }
                            if (found) {
                                break;
                            }
                            i++;
                        }
                    }
                    equivalenceClasses[foundEquivalenceClasses].insert(state);
                }
                for (auto &item: equivalenceClasses) {
                    set<string> equivalenceClass = item.second;
                    newEquivalenceClasses.insert(equivalenceClass);
                }
            }
        }
        int i = 0;
        set<string> newStates, newFinalStates;
        string newInitialState;
        map<set<string>, string> newNames;
        map<string, set<string>> statesToEquivalenceClasses;
        for (set<string> equivalenceClass: newEquivalenceClasses) {
            string stateName = "q"+to_string(i);
            bool isFinal = false;
            for (string state: equivalenceClass) {
                if (result.final_states.count(state)) {
                    isFinal = true;
                }
                if (state == "&" && equivalenceClass.size() == 1) {
                    stateName = "-";
                }
                statesToEquivalenceClasses[state] = equivalenceClass;
            }
            if (stateName == "-") {
                continue;
            }
            if (equivalenceClass.count(result.initial_state)) {
                newInitialState = stateName;
            }
            if (isFinal) {
                newFinalStates.insert(stateName);
            }
            newNames[equivalenceClass] = stateName;
            newStates.insert(stateName);
            i++;
        }
        for (set<string> equivalenceClass: newEquivalenceClasses) {
            string stateName = newNames[equivalenceClass];
            for (string state: equivalenceClass) {
                for (char symbol: result.alphabet) {
                    string toState;
                    if (result.transitions[state].count(symbol) &&
                            !result.transitions[state][symbol].empty()) {
                        for (string to: result.transitions[state][symbol]) {
                            toState = to;
                            // Just get the first element and break the loop..
                            break;
                        }
                    } else {
                        toState = "&";
                    }
                    set<string> toEquivalenceClass = statesToEquivalenceClasses[toState];
                    if (toState == "&" && toEquivalenceClass.size() == 1) {
                        // Epsilon without equivalent states..just ignore
                        continue;
                    }
                    toState = newNames[toEquivalenceClass];
                    result.transitions[stateName][symbol].insert(toState);
                }
            }
        }
        result.initial_state = newInitialState;
        result.setStates(newStates, newFinalStates);
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
    
    bool isComplete() {
        for (const string& state: states) {
            if (!transitions.count(state)) {
                return false;
            }
            for (const char& symbol: alphabet) {
                if (!transitions[state].count(symbol)) {
                    return false;
                }    
            }
        }
        return true;
    }
    FiniteAutomata complete() {
        FiniteAutomata result(*this);
        string errorState = findFreeName();
        result.addState(errorState, false, true);
        for (const char& symbol: alphabet) {
            result.transitions[errorState][symbol].insert(errorState); 
        }
        for (const string& state: states) {
            for (const char& symbol: alphabet) {
                if (!transitions[state].count(symbol)) {
                    transitions[state][symbol].insert(errorState);
                }
            }
        }
        return result;
        
    }
    
    FiniteAutomata doUnion(FiniteAutomata other) {
        FiniteAutomata result;
        map<string, string> statesMapping, otherStatesMapping;
        string initialState = result.findFreeName();
        result.addState(initialState, true, false);
        for (const char &symbol: alphabet) {
            if (symbol == EPSILON) {
                continue;
            }
            result.addSymbol(symbol);
        }
        for (const string &state: states) {
            string newName = result.findFreeName();
            statesMapping[state] = newName;
            result.addState(newName, false, false);
        }
        for (const char &symbol: other.alphabet) {
            if (symbol == EPSILON) {
                continue;
            }
            result.addSymbol(symbol);
        }
        for (const string &state: other.states) {
            string newName = result.findFreeName();
            otherStatesMapping[state] = newName;
            result.addState(newName, false, false);
        }
        string finalState = result.findFreeName();
        result.addState(finalState, false, true);
        for (const string &state: states) {
            for (const char &symbol: alphabet) {
                set<string> transition = transitions[state][symbol];
                for (const string &toState: transition) {
                    result.addTransition(statesMapping[state], symbol, statesMapping[toState]);
                }
            }
        }
        for (const string &state: other.states) {
            for (const char &symbol: other.alphabet) {
                set<string> transition = other.transitions[state][symbol];
                for (const string &toState: transition) {
                    result.addTransition(otherStatesMapping[state], symbol, otherStatesMapping[toState]);
                }
            }
        }
        result.addTransition(initialState, EPSILON, statesMapping[initial_state]);
        result.addTransition(initialState, EPSILON, otherStatesMapping[other.initial_state]);
        for (const string &state: final_states) {
            result.addTransition(statesMapping[state], EPSILON, finalState);
        }
        for (const string &state: other.final_states) {
            result.addTransition(otherStatesMapping[state], EPSILON, finalState);
        }
        
        return result;
    }
    
    FiniteAutomata doIntersection(FiniteAutomata other) {
        FiniteAutomata result, l1(*this), l2(other);
        l1 = l1.doComplement();
        l2 = l2.doComplement();
        result = l1.doUnion(l2);
        return result.doComplement();
    }
    
    FiniteAutomata doComplement() {
        FiniteAutomata result = complete();
        set<string> newFinalStates;
        for (const string &state: result.states) {
            if (!result.final_states.count(state)) {
                newFinalStates.insert(state);
            }
        }
        result.final_states = newFinalStates;
        return result;
    }
    
    FiniteAutomata doDifference(FiniteAutomata other) {
        FiniteAutomata l1(*this), l2(other);
        l2 = l2.doComplement();
        return l1.doUnion(l2);
        
    }
    string toASCIITable() {
        string result;
        map<char, int> columnWidth;
        int largestState = 5;
        for (string state: states) {
            int length = state.length()+2;
            if (initial_state == state) {
                length += 2;
            }
            if (final_states.count(state)) {
                length += 1;
            }
            if (length > largestState) {
                largestState = length;
            }
        }
        for (char symbol: alphabet) {
            int largestTransition = 5;
            bool found = false;
            for (string state: states) {
                if (!transitions[state][symbol].empty()) {
                    found = true;
                }
                string transition = formatStates(transitions[state][symbol], false);
                int length = transition.length()+3;
                if (length > largestTransition) {
                    largestTransition = length;
                }
            }
            if (found) {
                columnWidth[symbol] = largestTransition;
            }
        }
        int tableWidth = alphabet.size()+1+largestState;
        for (auto column: columnWidth) {
            tableWidth += column.second;
        }
        result.append(tableWidth, '-');
        result.append("\n");
        result.append("| &");
        result.append(largestState-2, ' ');
        for (char symbol: alphabet) {
            bool found = false;
            for (string state: states) {
                if (!transitions[state][symbol].empty()) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                continue;
            }
            result.append("| ");
            result.append(1, symbol);
            result.append(columnWidth[symbol]-2, ' ');
        }
        result.append("|\n");
        result.append(tableWidth, '-');
        result.append("\n");
        queue<string> q;
        q.push(initial_state);
        for (string state: states) {
            if (final_states.count(state) || state == initial_state) {
                continue;
            }
            q.push(state);
        }
        for (string state: final_states) {
            if (state == initial_state) {
                continue;
            }
            q.push(state);
        }
        while (!q.empty()) {
            string state = q.front();
            q.pop();
            result.append("| ");
            int margin = 1;
            if (final_states.count(state)) {
                result.append("*");
                margin++;
            }
            if (initial_state == state) {
                result.append("->");
                margin += 2;
            }
            result.append(state);
            result.append(largestState-margin-state.length(), ' ');
            for (char symbol: alphabet) {
                if (transitions[state][symbol].empty()) {
                    if (columnWidth.count(symbol)) {
                        result.append("| -");
                        result.append(columnWidth[symbol]-2, ' ');
                    }
                    continue;
                }
                result.append("| ");
                string transition = formatStates(transitions[state][symbol], false);
                result.append(transition);
                result.append(columnWidth[symbol]-transition.length()-1, ' ');
            }
            result.append("|\n");
            result.append(tableWidth, '-');
            result.append("\n");
        }
        return result;
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
    string formatStates(set<string> states, bool brackets=true) {
        string s;
        if (brackets) {
            s = "[";
        }
        int c = states.size();
        for (string state: states) {
            s.append(state);
            c--;
            if (c > 0) {
                s.append(",");
            }
        }
        if (brackets) {
            s.append("]");
        }
        return s;
    }

    void setStates(set<string> newStates, set<string> finalStates) {
        for (string state: states) {
            if (newStates.count(state)) {
                continue;
            }
            transitions.erase(state);
        }
        states = newStates;
        final_states = finalStates;
    }

    void setStates(set<string> newStates) {
        set <string> finalStates;
        for (string state: final_states) {
            if (!newStates.count(state)) {
                continue;
            }
            finalStates.insert(state);
        }
        setStates(newStates, finalStates);
    }
    
    string findFreeName() {
        int i = 0;
        bool isFree = false;
        string name;
        do {
            name = "q" + to_string(i);
            isFree = !states.count(name);
            i++;
        } while (!isFree);
        return name;
    }
    set<string> states;
    set<char> alphabet;
    map<string, map<char, set<string> > > transitions;
    string initial_state;
    set<string> final_states;
};

#ifdef FINITE_AUTOMATA_TEST
int main() {
    FiniteAutomata f, m;
    f.addSymbol('a');
    f.addSymbol('b');
    f.addState("q0", true, false);
    f.addState("q1", false, false);
    f.addState("q2", false, true);
    f.addTransition("q0", 'a', "q1");
    f.addTransition("q1", 'b', "q2");
    cout << "ab:" << endl << f.toASCIITable() << endl;
    m.addSymbol('a');
    m.addSymbol('b');
    m.addState("q0", true, false);
    m.addState("q1", false, false);
    m.addState("q2", false, true);
    m.addTransition("q0", 'b', "q1");
    m.addTransition("q1", 'a', "q2");
    cout << "ba:" << endl << m.toASCIITable() << endl;
    cout << "(ab|ba):" << endl << f.doUnion(m).toASCIITable() << endl;
    f = f.doUnion(m).determinize().back().removeUnreachableStates().removeDeadStates().removeEquivalentStates();
    cout << "(ab|ba) minimizado:" << endl << f.toASCIITable() << endl;
    for(string s: f.generates()) {
        cout << '"' << s << '"' << endl;
    }
//    f.addState("[S]", true, false);
//    f.addState("[AD]", false, false);
//    f.addState("[E]", false, true);
//    f.addState("[ABD]", false, false);
//    f.addState("[CE]", false, true);
//    f.addState("[ABE]", false, true);
//    f.addSymbol('0');
//    f.addSymbol('1');
//    f.addTransition("[S]", '0', "[AD]");
//    f.addTransition("[S]", '1', "[E]");
//    f.addTransition("[AD]", '0', "[ABD]");
//    f.addTransition("[AD]", '1', "[E]");
//    f.addTransition("[E]", '0', "[E]");
//    f.addTransition("[E]", '1', "[E]");
//    f.addTransition("[ABD]", '0', "[ABD]");
//    f.addTransition("[ABD]", '1', "[CE]");
//    f.addTransition("[CE]", '0', "[ABE]");
//    f.addTransition("[CE]", '1', "[E]");
//    f.addTransition("[ABE]", '0', "[ABE]");
//    f.addTransition("[ABE]", '1', "[CE]");

    f = FiniteAutomata();

    f.addState("q0", true, true);
    f.addState("q1", false, false);
    f.addState("q3", false, false);
    f.addState("q4", false, false);
    f.addState("q5", false, false);
    f.addState("q6", false, true);
    f.addSymbol('a');
    f.addSymbol('b');
    f.addSymbol('c');
//    f.addTransition("q0", 'a', "q1");
//    f.addTransition("q1", 'b', "q3");
//    f.addTransition("q3", 'b', "q4");
//    f.addTransition("q4", 'a', "q5");
//    f.addTransition("q5", 'a', "q6");

    f.addTransition("q0", 'a', "q1");
    f.addTransition("q0", 'b', "q1");
    f.addTransition("q0", 'b', "q3");
    f.addTransition("q0", 'c', "q3");
    f.addTransition("q1", 'b', "q0");
    f.addTransition("q1", 'b', "q6");
    f.addTransition("q1", 'c', "q4");
    f.addTransition("q1", 'c', "q6");
    f.addTransition("q3", 'a', "q5");
    f.addTransition("q3", 'a', "q6");
    f.addTransition("q3", 'b', "q0");
    f.addTransition("q3", 'b', "q6");
    f.addTransition("q3", 'b', "q0");
    f.addTransition("q4", 'a', "q1");
    f.addTransition("q4", 'b', "q1");
    f.addTransition("q4", 'b', "q3");
    f.addTransition("q5", 'b', "q1");
    f.addTransition("q5", 'b', "q3");
    f.addTransition("q5", 'c', "q3");
    f = f.determinize().back();

    // cout << f.toASCIITable();
    // cout << "Remoção de estados inalcancaveis:" << endl;
    // cout << f.removeUnreachableStates().toASCIITable();
    // cout << "Remoção de estados inalcancaveis+mortos:" << endl;
    // cout << f.removeUnreachableStates().removeDeadStates().toASCIITable();
    // cout << "Remoção de estados inalcancaveis+mortos+equivalentes:" << endl;
    // cout << f.removeUnreachableStates().removeDeadStates().removeEquivalentStates().toASCIITable();

    // cout << f.toASCIITable();

//    cout << f.nonDeterministic() << endl;
//    list<FiniteAutomata> results = f.determinize();
//    cout << results.size() << endl;
//    cout << results.back().getStates() << endl;
//    cout << results.back().nonDeterministic() << endl;
//    for (string s: f.generates()) {
//        cout << '"' << s << '"' << endl;
//    }
//    results.back().removeEquivalentStates();
    return 0;
}
#endif