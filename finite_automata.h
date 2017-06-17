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

        /*!
         * Return the symbols (in a string) used to reach this state
         *
         * @return The string that allowed to reach this state
         */
        string Symbols();

        /*!
         * Set the symbols used to reach this state
         *
         * @param symbols The string that allowed to reach this state
         */
        void Symbols(string symbols);
    private:
        string state; //!< The actual state of this object
        string symbols; //!< The string used to reach the state of this object
};

/*!
 * This class has the purpose to represent an finite automata and allow to do
 * operations with it.
 *
 * Please note that all the operations that can be done in this class that
 * may modify the automata are returned as new automata, with the exception of
 * simpler operations like adding a state, a symbol or a transaction.
 */
class FiniteAutomata {
    /*!
     * This class provides an iterator based on a FiniteAutomata, which
     * generates all the strings that the FiniteAutomata accepts on demand.
     */
    class FiniteAutomataGenerator {
        public:
        /*!
         * Creates a new generator based on a FiniteAutomata
         */
        FiniteAutomataGenerator(FiniteAutomata &f);

        /*!
         * Allow to check if the generator already finished. Always return true
         * on a finite automata that has loops.
         *
         * @return true if the generator already finished, false otherwise.
         */
        bool operator!=(FiniteAutomataGenerator);

        /*!
         * Returns the start of the automata, or...in other words, return the
         * generator itself (just to be compatible with the C++ iterator
         * protocol)
         *
         * @return The generator itself
         */
        FiniteAutomataGenerator begin() const;

        /*!
         * Returns the end of the automata, or...in other words, return the
         * generator itself (just to be compatible with the C++ iterator
         * protocol)
         *
         * @return The generator itself
         */
        FiniteAutomataGenerator end() const;

        /*!
         * Advances the generator while there are states to execute and
         * there are no accepted strings generated yet, checking first if there
         * is some string already generated or if it needs to jump to other
         * states.
         *
         * This method can block forever if the finite automata does not
         * accept any strings.
         *
         * @see FiniteAutomata::FiniteAutomataGenerator::next
         *
         * @return The object itself
         */
        FiniteAutomataGenerator &operator++();

        /*!
         * Synonym for the value() method.
         *
         * @see FiniteAutomata::FiniteAutomataGenerator::next
         * @see FiniteAutomata::FiniteAutomataGenerator::value
         *
         * @return A string that is accepted by the Finite Automata
         */
        string operator*();

        private:
        /*!
         * Returns a string that is accepted by the Finite Automata, or try
         * to generate one if none is available.
         *
         * May block forever if the finite automata does not accept any strings.
         *
         * @see FiniteAutomata::FiniteAutomataGenerator::value
         *
         * @return A string that is accepted by the Finite Automata
         */
        string value();

        /*!
         * Advances the generator while there are states to execute and
         * there are no accepted strings generated yet, checking first if there
         * is some string already generated or if it needs to jump to other
         * states.
         *
         * This method can block forever if the finite automata does not
         * accept any strings.
         */
        void next();

        FiniteAutomata &finite_automata; //!< The Finite Automata itself
        queue<string> generated; //! A queue of generated strings
        queue<FiniteAutomataState> actual_states; //!< A queue of the actual states
    };
public:
    /*!
     * Constructs an empty Finite Automata, without any states or transitions,
     * and only with epsilon in the alphabet
     */
    FiniteAutomata();

    /*!
     * Copy an existing Finite Automata into a new Finite Automata object
     */
    FiniteAutomata(const FiniteAutomata &f);

    /*!
     * Check if this finite automata is deterministic
     *
     * see
     * @return true if this finite automata is deterministic, false otherwise
     */
    bool isDeterministic() const;

    /*!
     * Add a symbol to the alphabet of this Finite Automata
     *
     * @see FiniteAutomata::hasSymbol
     * @see FiniteAutomata::getAlphabet
     * @throw FiniteAutomataException If the symbol is invalid
     *
     * @param symbol The symbol to be added to the alphabet
     */
    void addSymbol(char symbol);

    /*!
     * Add a state to the list of states of this Finite Automata
     *
     * @see FiniteAutomata::hasState
     * @see FiniteAutomata::isFinalState
     * @see FiniteAutomata::isInitialState
     * @throw FiniteAutomataException If this state is initial and the automata
     * already has an initial state
     */
    void addState(string state, int type = 0);

    /*!
     * Check if a state already exists in the Finite Automata
     *
     * @param state The name of the state to check for existence
     * @return true if the state exists in the finite automata, false otherwise
     */
    bool hasState(string state) const;

    /*!
     * Check if a symbol already exists in the alphabet of the Finite Automata
     *
     * @param symbol The symbol to check for existence
     * @return true if the symbol exists in the alphabet, false otherwise
     */
    bool hasSymbol(char symbol) const;

    /*!
     * Return the set of states of this finite automata
     *
     * @return The set of states of this finite automata
     */
    set<string> getStates() const;

    /*!
     * Return the set of symbols of the alphabet of this finite automata
     *
     * @return The set of symbols of the alphabet of this finite automata
     */
    set<char> getAlphabet() const;

    /*!
     * Return if this finite automata is empty, or, in other words, if it
     * has final states
     *
     * @return true if the finite automata is empty, false otherwise
     */
    bool isEmpty() const;

    /*!
     * Return if this finite automata is equivalent to the finite automata
     * passed into the parameter
     *
     * @param  other The other automata to compare with this automata
     * @return       true if this automata is equivalent to the automata passed
     * into the parameter
     */
    bool isEquivalent(FiniteAutomata other) const;

    /*!
     * Return if this automata is contained in the finite automata passed
     * into the parameter
     *
     * @param  other The other automata to compare with this automata
     * @return       true if this automata is contained in the automata passed
     * into the parameter
     */
    bool isContained(FiniteAutomata other) const;

    /*!
     * Check if a state is final in the Finite Automata
     *
     * @param state The state to check if it is final
     * @return true if the state is final, false otherwise
     */
    bool isFinalState(string state) const;

    /*!
     * Check if a state is initial in the Finite Automata
     *
     * @param state The state to check if it is initial
     * @return true if the state is initial, false otherwise
     */
    bool isInitialState(string state) const;

    /*!
     * Check if there is a transition provided by a symbol between two states in
     * the finite automata
     *
     * @param source The source state to check
     * @param symbol The symbol to check if there is a transition
     * @param target The target state that will be reached via the symbol
     *
     * @return true if such a transaction exists, false otherwise
     */
    bool hasTransition(string source, char symbol, string target);

    /*!
     * Return the set of transitions that can be done from a state and a symbol
     *
     * @param source The source state for the symbols
     * @param symbol The symbol that the transition must refer
     * @return The set of states that will be reachable from a state and a
     * symbol
     */
    set<string> getTransitions(string source, char symbol);

    /*!
     * Add a transition to the finite automata
     *
     * @param source The source state for the transition
     * @param symbol The symbol that will trigger that transition
     * @param target The target state of the transition
     */
    void addTransition(string source, char symbol, string target);

    /*!
     * Determinize and return the deterministic finite automata
     *
     * @return The deterministic finite automata
     */
    FiniteAutomata determinize() const;

    /*!
     * Remove the unreachable states from the finite automata, returning a
     * new finite automata without unreachable states.
     *
     * @return The new finite automata without unreachable states
     */
    FiniteAutomata removeUnreachableStates() const;

    /*!
     * Remove the dead states from the finite automata, returning a
     * new finite automata without dead states.
     *
     * @return The new finite automata without dead states
     */
    FiniteAutomata removeDeadStates() const;

    /*!
     * Remove the equivalent states from the finite automata, returning a
     * new finite automata without equivalent states.
     *
     * @return The new finite automata without equivalent states
     */
    FiniteAutomata removeEquivalentStates() const;

    /*!
     * Check if a string is accepted by the finite automata
     *
     * @param s the string to be checked
     * @return true if the string is accepted, false otherwise
     */
    bool accepts(string s);

    /*!
     * Check if a finite automata is complete, or, in other words, if it
     * does not have states with empty transitions
     *
     * @return true if the finite automata is complete, false otherwise
     */
    bool isComplete() const;

    /*!
     * Return a new deterministic finite automata state with an error state
     * and without empty transitions.
     *
     * @return A complete and deterministic finite automata
     */
    FiniteAutomata complete() const;

    /*!
     * Do the union of the finite automata represented by this object with the
     * finite automata provided by the argument and return the new finite
     * automata that represents the union between these two finite automatas
     *
     * @param other The other finite automata do to the union with this automata
     * @return The union between this and other finite automatas
     */
    FiniteAutomata doUnion(FiniteAutomata other) const;

    /*!
     * Do the intersection of the finite automata represented by this object
     * with the finite automata provided by the argument and return the new
     * finite automata that represents the intersection between these two
     * finite automatas
     *
     * @param other The other finite automata do to the intersection with this
     * automata
     * @return The intersection between this and other finite automatas
     */
    FiniteAutomata doIntersection(FiniteAutomata other) const;

    /*!
     * Return the complement of this finite automata
     *
     * @return The complement of this finite automata
     */
    FiniteAutomata doComplement() const;

    /*!
     * Do the difference of the finite automata represented by this object
     * with the finite automata provided by the argument and return the new
     * finite automata that represents the difference between these two
     * finite automatas
     *
     * @param other The other finite automata do to the difference with this
     * automata
     * @return The difference between this and other finite automatas
     */
    FiniteAutomata doDifference(FiniteAutomata other) const;

    /*!
     * Return a representation of this finite automata in the format of an
     * ASCII table ready to be printed
     *
     * @return The representation of this finite automata in an ASCII table
     */
    string toASCIITable() const;

    /*!
     * Returns an iterator that generates strings that this finite automata
     * accepts
     *
     * @see FiniteAutomata::FiniteAutomataGenerator
     * @return An iterator that generates strings that the finite automata
     * accepts
     */
    FiniteAutomataGenerator generates();

    const static int FINAL_STATE; //!< Constant used to represent a final state
    const static int INITIAL_STATE; //!< Constant used to represent a initial state
    const static char EPSILON; //!< Constant that represent the Epsilon char
private:
    /*!
     * Returns a set of the states reachable by epsilon transitions in this
     * finite automata
     *
     * @param  state    The state to start the processing
     * @return          The set of the states reachable via epsilon transitions
     */
    set<string> getClosure(string state) const;

    /*!
     * Return a string representing a set of states, allowing the user to put
     * it into brackets or not
     *
     * @param  states   The set of states to represent
     * @param  brackets If the representation should have a bracket or not
     * @return          The representation of the set of states
     */
    string formatStates(set<string> states, bool brackets=true) const;

    /*!
     * Set the new states and final states of this finite automata, deleting any
     * transitions from states that are not in these sets
     *
     * @param newStates   The new set of states of this finite automata
     * @param finalStates The new set of final states of this finite automata
     */
    void setStates(set<string> newStates, set<string> finalStates);

    /*!
     * Set the new sstates of this finite automata, deleting any transitions
     * from states that are not in these sets
     *
     * @param newStates The new set of states of this finite automata
     */
    void setStates(set<string> newStates);

    /*!
     * Return a name that is free in this finite automata, in other words, a
     * name that is not used by any states.
     *
     * @return A free name to use in a new state
     */
    string findFreeName() const;

    set<string> states; //!< The set of states of this finite automata
    set<char> alphabet; //!< The set of symbols of the alphabet of this FA
    map<string, map<char, set<string> > > transitions; //!< The transitions of this AF
    string initial_state; //!< The initial state of this finite automata
    set<string> final_states; //!< The final states of this finite automata
};
#endif // FINITE_AUTOMATA_H
