#include "finite_automata.h"

FiniteAutomataState::FiniteAutomataState(): state(""), symbols("") {}

FiniteAutomataState::FiniteAutomataState(const FiniteAutomataState &fstate) {
    state = fstate.state;
    symbols = fstate.symbols;
}

string FiniteAutomataState::State() {
    return state;
}

void FiniteAutomataState::State(string state) {
    this->state = state;
}

string FiniteAutomataState::Symbols() {
    return symbols;
}

void FiniteAutomataState::Symbols(string symbols) {
    this->symbols = symbols;
}

const int FiniteAutomata::INITIAL_STATE = 1 << 1;

const int FiniteAutomata::FINAL_STATE = 1 << 0;

const char FiniteAutomata::EPSILON = '&';

FiniteAutomata::FiniteAutomataGenerator::FiniteAutomataGenerator(FiniteAutomata &f) : finite_automata(f), actual_states() {
    for (string state: f.getClosure(f.initial_state)) {
        FiniteAutomataState actualState;
        actualState.State(state);
        actualState.Symbols("");
        actual_states.push(actualState);
    }
}

bool FiniteAutomata::FiniteAutomataGenerator::operator!=(FiniteAutomata::FiniteAutomataGenerator)
{
    return !actual_states.empty() || !generated.empty();
}

FiniteAutomata::FiniteAutomataGenerator FiniteAutomata::FiniteAutomataGenerator::begin() const
{
    return *this;
}


FiniteAutomata::FiniteAutomataGenerator FiniteAutomata::FiniteAutomataGenerator::end() const
{
    return *this;
}


FiniteAutomata::FiniteAutomataGenerator &FiniteAutomata::FiniteAutomataGenerator::operator++() {
    next();
    return *this;
}

string FiniteAutomata::FiniteAutomataGenerator::operator*() {
    return value();
}

string FiniteAutomata::FiniteAutomataGenerator::value() {
    if (generated.empty()) {
        // If the set is empty, we may try to generate new
        // elements on demand
        next();
    }
    string s = generated.front();
    generated.pop();
    return s;
}

void FiniteAutomata::FiniteAutomataGenerator::next() {
    while(generated.empty() && !actual_states.empty()) {
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
                 if (symbol != EPSILON) {
                    nextString.append(1, symbol);
                 }
                 nextState.Symbols(nextString);
                 nextState.State(toState);
                 actual_states.push(nextState);
             }
        }
    }
}

FiniteAutomata::FiniteAutomata() {
    alphabet.insert(EPSILON);
}

FiniteAutomata::FiniteAutomata(const FiniteAutomata &f) {
    states = f.states;
    alphabet = f.alphabet;
    transitions = f.transitions;
    initial_state = f.initial_state;
    final_states = f.final_states;
}

bool FiniteAutomata::isDeterministic() const {
    for (string state: states) {
        if (!transitions.count(state)) {
            continue;
        }
        map<char, set<string> > stateTransitions = transitions.at(state);
        set<string> closure = getClosure(state);
        int closureSize = closure.size()-1;
        if (closureSize > 1) {
            return false;
        }
        for (char symbol: alphabet) {
            if (symbol == EPSILON || !stateTransitions.count(symbol)) {
                continue;
            }
            set<string> transition = stateTransitions.at(symbol);
            int exits = transition.size()+closureSize;
            if (exits > 1) {
                return false;
            }
        }
    }
    return true;
}

void FiniteAutomata::addSymbol(char symbol) {
    if ((symbol < '0' || symbol > '9') && (symbol < 'a' || symbol > 'z')) {
        throw FiniteAutomataException("Symbol should be between '0' and '9' or between 'a' and 'z'");
    }
    alphabet.insert(symbol);
}

void FiniteAutomata::addState(string state, int type) {
    if ((state.length() > 1 && state[0] == '-' && state[1] == '>') ||
        (state.length() > 2 && state[1] == '-' && state[2] == '>')) {
        type |= INITIAL_STATE;
        if (state[1] == '-' && state[2] == '>') {
            state.erase(1,2);
        } else {
            state.erase(0, 2);
        }
    }
    if (state.length() > 1 && state[0] == '*') {
        type |= FINAL_STATE;
        state.erase(0, 1);
    }
    if (!initial_state.empty() && type & INITIAL_STATE) {
        throw FiniteAutomataException("Initial State already defined");
    }
    states.insert(state);
    if (type & INITIAL_STATE) {
        initial_state = state;
    }
    if (type & FINAL_STATE) {
        final_states.insert(state);
    }
}

bool FiniteAutomata::hasState(string state) const  {
    return states.count(state);
}

bool FiniteAutomata::isInitialState(string state) const  {
    return initial_state == state;
}

bool FiniteAutomata::isFinalState(string state) const {
    return final_states.count(state);
}

bool FiniteAutomata::hasSymbol(char symbol) const {
    return alphabet.count(symbol);
}


bool FiniteAutomata::hasTransition(string source, char symbol, string target) {
    if (!hasState(source) || !hasState(target) || !hasSymbol(symbol)) {
        return false;
    }
    return transitions[source].count(symbol) &&
            transitions[source][symbol].count(target);
}

set<string> FiniteAutomata::getTransitions(string source, char symbol) {
    set<string> result;
    if (!hasState(source) || !hasSymbol(symbol)) {
        return result;
    }
    set<string> transition = transitions[source][symbol];
    result.insert(transition.begin(), transition.end());
    return result;
}

void FiniteAutomata::addTransition(string source, char symbol, string target) {
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

FiniteAutomata FiniteAutomata::determinize() const {
    if (initial_state.empty()) {
        throw FiniteAutomataException("Initial State should be defined to determinize automata");
    }
    FiniteAutomata result(*this);
    queue<set<string> > q;
    set<string> initialState = getClosure(initial_state);
    q.push(initialState);
    result.initial_state = "";
    while (!q.empty()) {
        set<string> states = q.front();
        q.pop();
        string stateName = formatStates(states);
        int type = 0;
        if (states == initialState) {
            type |= INITIAL_STATE;
        }
        for (string state: states) {
            if (result.final_states.count(state)) {
                type |= FINAL_STATE;
                break;
            }
        }
        result.addState(stateName, type);
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
                set<string> transition = result.transitions[state][symbol];
                newTransitions.insert(transition.begin(), transition.end());
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
    }
    return result.removeUnreachableStates();
}

FiniteAutomata FiniteAutomata::removeUnreachableStates() const {
    if (initial_state.empty()) {
        throw FiniteAutomataException("Initial State should be defined to remove unreachable states");
    }
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

FiniteAutomata FiniteAutomata::removeDeadStates() const {
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

FiniteAutomata FiniteAutomata::removeEquivalentStates() const {
    if (!isDeterministic()) {
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
    int i = 1;
    set<string> newStates, newFinalStates;
    string newInitialState;
    map<set<string>, string> newNames;
    map<string, set<string>> statesToEquivalenceClasses;
    result.transitions.clear();
    for (set<string> equivalenceClass: newEquivalenceClasses) {
        string stateName = "q"+to_string(i);
        stateName = formatStates(equivalenceClass, true);
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
            // stateName = "q0"; // First state is always "q0"
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
                if (transitions.count(state) &&
                    transitions.at(state).count(symbol) &&
                    !transitions.at(state).at(symbol).empty()
                ) {
                    for (string to: transitions.at(state).at(symbol)) {
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

bool FiniteAutomata::accepts(string s) {
    if (initial_state.empty()) {
        throw FiniteAutomataException("Initial State should be defined to check if string is accepted");
    }
    set <string> actualStates;
    set<string> initialState = getClosure(initial_state);
    actualStates.insert(initialState.begin(), initialState.end());
    for (char symbol: s) {
        if (!alphabet.count(symbol)) {
            return false;
        }
        set <string> nextStates;
        for (string state: actualStates) {
            set<string> toStates = transitions[state][symbol];
            for (string toState: toStates) {
                set<string> transition = getClosure(toState);
                nextStates.insert(transition.begin(), transition.end());
            }
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

bool FiniteAutomata::isComplete() const {
    for (const string& state: states) {
        if (!transitions.count(state) || transitions.at(state).size() < (alphabet.size()-1)) {
            return false;
        }
    }
    return true;
}

FiniteAutomata FiniteAutomata::complete() const {
    FiniteAutomata result = determinize();
    string errorState = result.findFreeName();
    result.addState(errorState);
    for (const char& symbol: alphabet) {
        if (symbol == EPSILON) {
            continue;
        }
        result.transitions[errorState][symbol].insert(errorState);
    }
    for (const string& state: result.states) {
        for (const char& symbol: alphabet) {
            bool isEmpty = !result.transitions.count(state);
            isEmpty = isEmpty || !result.transitions.at(state).count(symbol);
            isEmpty = isEmpty || result.transitions.at(state).at(symbol).empty();
            if (isEmpty && symbol != EPSILON) {
                result.transitions[state][symbol].insert(errorState);
            }
        }
    }
    return result;
}

FiniteAutomata FiniteAutomata::doUnion(FiniteAutomata other) const {
    FiniteAutomata result;
    map<string, string> statesMapping, otherStatesMapping;
    string initialState = result.findFreeName();
    int initialStateType = INITIAL_STATE;
    if (isFinalState(initial_state) || other.isFinalState(other.initial_state)){
        initialStateType |= FINAL_STATE;
    }
    result.addState(initialState, initialStateType);
    for (const char &symbol: alphabet) {
        if (symbol == EPSILON) {
            continue;
        }
        result.addSymbol(symbol);
    }
    for (const string &state: states) {
        string newName = result.findFreeName();
        statesMapping[state] = newName;
        int type = 0;
        if (isFinalState(state)) {
            type |= FINAL_STATE;
        }
        result.addState(newName, type);
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
        int type = 0;
        if (other.isFinalState(state)) {
            type |= FINAL_STATE;
        }
        result.addState(newName, type);
    }
    for (const string &state: states) {
        if (!transitions.count(state)) {
            continue;
        }
        for (const char &symbol: alphabet) {
            if (!transitions.at(state).count(symbol)) {
                continue;
            }
            set<string> transition = transitions.at(state).at(symbol);
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
    return result;
}

FiniteAutomata FiniteAutomata::doIntersection(FiniteAutomata other) const {
    FiniteAutomata result, l1(*this), l2(other);
    set<char> newAlphabet;
    newAlphabet.insert(alphabet.begin(), alphabet.end());
    newAlphabet.insert(other.alphabet.begin(), other.alphabet.end());
    l1.alphabet = newAlphabet;
    l2.alphabet = newAlphabet;
    l1 = l1.doComplement();
    l2 = l2.doComplement();
    result = l1.doUnion(l2);
    return result.doComplement();
}

FiniteAutomata FiniteAutomata::doComplement() const {
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

FiniteAutomata FiniteAutomata::doDifference(FiniteAutomata other) const {
    FiniteAutomata l1(*this), l2(other);
    l2 = l2.doComplement();
    return l1.doIntersection(l2);
}


string FiniteAutomata::toASCIITable() const {
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
        int largestTransition = 3;
        bool found = false;
        for (string state: states) {
            int length = 2;
            if (!transitions.count(state) || !transitions.at(state).count(symbol) || transitions.at(state).at(symbol).empty()) {
                length++;
            } else {
                string transition = formatStates(transitions.at(state).at(symbol), false);
                length += transition.length();
                found = true;
            }
            if (length > largestTransition) {
                largestTransition = length;
            }
        }
        if (found) {
            columnWidth[symbol] = largestTransition;
        }
    }
    int tableWidth = columnWidth.size()+2+largestState;
    for (auto column: columnWidth) {
        tableWidth += column.second;
    }
    result.append(tableWidth, '-');
    result.append("\n");
    result.append("| 𝛿");
    result.append(largestState-2, ' ');
    for (char symbol: alphabet) {
        bool found = false;
        for (string state: states) {
            if (transitions.count(state) && transitions.at(state).count(symbol) && !transitions.at(state).at(symbol).empty()) {
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
            if (!transitions.count(state) || !transitions.at(state).count(symbol) || transitions.at(state).at(symbol).empty()) {
                if (columnWidth.count(symbol)) {
                    result.append("| -");
                    result.append(columnWidth[symbol]-2, ' ');
                }
                continue;
            }
            result.append("| ");
            string transition = formatStates(transitions.at(state).at(symbol), false);
            result.append(transition);
            result.append(columnWidth[symbol]-transition.length()-1, ' ');
        }
        result.append("|\n");
        result.append(tableWidth, '-');
        result.append("\n");
    }
    return result;
}

FiniteAutomata::FiniteAutomataGenerator FiniteAutomata::generates() {
    return FiniteAutomataGenerator(*this);
}

set<string> FiniteAutomata::getClosure(string state) const {
    set<string> results = set<string>();
    queue<string> q;
    q.push(state);
    while (!q.empty()) {
        string s = q.front();
        q.pop();
        if (s.empty() || results.count(s)) {
            continue;
        }
        results.insert(s);
        if (!transitions.count(s) || !transitions.at(s).count(EPSILON)) {
            continue;
        }
        for (string state: transitions.at(s).at(EPSILON)) {
            q.push(state);
        }
    }
    return results;
}

string FiniteAutomata::formatStates(set<string> states, bool brackets) const {
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

void FiniteAutomata::setStates(set<string> newStates, set<string> finalStates) {
    for (string state: states) {
        if (newStates.count(state)) {
            continue;
        }
        transitions.erase(state);
    }
    states = newStates;
    final_states = finalStates;
}

void FiniteAutomata::setStates(set<string> newStates) {
    set <string> finalStates;
    for (string state: final_states) {
        if (!newStates.count(state)) {
            continue;
        }
        finalStates.insert(state);
    }
    setStates(newStates, finalStates);
}

string FiniteAutomata::findFreeName() const {
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
#ifdef FINITE_AUTOMATA_TEST
int main() {
    FiniteAutomata f;
    f.addSymbol('0');
    f.addSymbol('1');

    f.addState("*->q0");
    f.addState("*q1");
    f.addState("*q2");
    f.addState("*q3");
    f.addState("*q4");
    f.addState("*q5");
    f.addState("q6");
    f.addState("q8");
    f.addState("q9");
    f.addState("q10");
    f.addState("q11");
    f.addState("q12");
    f.addState("q13");
    f.addState("q14");
    f.addState("*q15");

    f.addTransition("q0", '0', "q1");
    f.addTransition("q0", '1', "q2");
    f.addTransition("q0", '0', "q7");
    f.addTransition("q0", '1', "q8");

    f.addTransition("q1", '0', "q3");
    f.addTransition("q1", '1', "q4");

    f.addTransition("q2", '0', "q1");
    f.addTransition("q2", '1', "q5");

    f.addTransition("q3", '0', "q6");
    f.addTransition("q3", '1', "q4");

    f.addTransition("q4", '0', "q1");
    f.addTransition("q4", '1', "q5");

    f.addTransition("q5", '0', "q1");
    f.addTransition("q5", '1', "q6");

    f.addTransition("q6", '0', "q6");
    f.addTransition("q6", '1', "q6");

    f.addTransition("q7", '0', "q7");
    f.addTransition("q7", '1', "q9");

    f.addTransition("q8", '0', "q7");
    f.addTransition("q8", '1', "q10");

    f.addTransition("q9", '0', "q7");
    f.addTransition("q9", '1', "q10");

    f.addTransition("q10", '0', "q11");
    f.addTransition("q10", '1', "q12");

    f.addTransition("q11", '0', "q10");
    f.addTransition("q11", '1', "q13");

    f.addTransition("q12", '0', "q14");
    f.addTransition("q12", '1', "q12");

    f.addTransition("q13", '0', "q15");
    f.addTransition("q13", '1', "q12");

    f.addTransition("q14", '0', "q15");
    f.addTransition("q14", '1', "q15");

    f.addTransition("q15", '0', "q15");
    f.addTransition("q15", '1', "q15");

    FiniteAutomata d = f.determinize();

    cout << "Antes: " << endl << f.toASCIITable() << endl;
    //cout << "Estados Inalcancaveis: " << endl << f.removeUnreachableStates().toASCIITable() << endl;
    //cout << "Estados Mortos: " << endl << f.removeUnreachableStates().removeDeadStates().toASCIITable() << endl;
    //cout << "Estados Equivalentes: " << endl << f.removeUnreachableStates().removeDeadStates().removeEquivalentStates().toASCIITable() << endl;
    //cout << "Completo: " << endl << f.removeUnreachableStates().removeDeadStates().removeEquivalentStates().complete().toASCIITable() << endl;
    //cout << "Determinizado: " << endl << d.toASCIITable() << endl;
    //cout << "T(f): " << endl << f.generates() << endl;
    //cout << "T(d): " << endl << d.generates() << endl;
    return 0;
}
#endif
